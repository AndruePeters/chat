//
// Created by druep on 4/27/2021.
//

/// Probably need to add a queue similar to the server
/// so that we can send the messages
#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "message.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <spdlog/spdlog.h>

namespace Client {

namespace beast     = boost::beast;    // from <boost/beast.hpp>
namespace http      = beast::http;    // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;    // from <boost/beast/websocket.hpp>
namespace net       = boost::asio;    // from <boost/asio.hpp>
using tcp           = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>

/// report a failure
void fail(beast::error_code ec, char const* what);

/// Sends a WebSocket message and prints the response
class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver;
    beast::flat_buffer buffer;
    websocket::stream<beast::tcp_stream> ws;
    std::vector<std::shared_ptr<const std::string>> queue;
    std::string host;
    std::string port;

  public:
    explicit session(net::io_context& ioc);

    ~session() = default;

    template<class Body, class Allocator>
    void run(http::request<Body, http::basic_fields<Allocator>> req);

    void send(const std::shared_ptr<const std::string>& ss);

    void setHost(std::string hst);
    void setPort(std::string prt);

    void connect();

  private:
    void on_send(const std::shared_ptr<const std::string>& ss);

    void on_accept(beast::error_code ec);

    void on_read(beast::error_code ec, std::size_t bytesTransferred);

    void on_write(beast::error_code ec, std::size_t bytesTransferred);

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results)
    {
      if (ec) return fail(ec, "resolve");

      beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

      beast::get_lowest_layer(ws).async_connect(
        results,
        beast::bind_front_handler(&session::on_connect, shared_from_this()) );

    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
    {
        if (ec) return fail(ec, "connect");

      // turn off the timeout on the tcp_stream because
      // the websocket stream has its own timeout system
      beast::get_lowest_layer(ws).expires_never();

      // set suggested timeout settings for the websocket
      ws.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

      // set a decorator to change the User-Agent of the handshake
      ws.set_option(websocket::stream_base::decorator(
              [](websocket::request_type& req) {
                  req.set(http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " chat-client");
              }));

      // update the host string. This will provide the value of the
      // host HTTP header during the websocket handshake
      // the guide references: https://tools.ietf.org/html/rfc7230#section-5.4
      host += ':' + std::to_string(ep.port());

      // now do the handshake
      ws.async_handshake(host, "/", beast::bind_front_handler(&session::on_handshake, shared_from_this()));
    }

    void on_handshake(beast::error_code ec)
    {
      if (ec) return fail(ec, "handshake");
      spdlog::info("Successfully completed handshake.");
    }
};

template<class Body, class Allocator>
void session::run(http::request<Body, http::basic_fields<Allocator>> req)
{
    // Set suggested timeout settings for the websocket
    ws.set_option(
      websocket::stream_base::timeout::suggested(
        beast::role_type::client));

    // Set a decorator to change the Server of the handshake
    /*ws.set_option(websocket::stream_base::decorator(
      [](websocket::response_type& res) {
          res.set(http::field::client,
            std::string(BOOST_BEAST_VERSION_STRING) + " websocket-chat-multi");
      })); */

    // Accept the websocket handshake
    ws.async_accept(
      req,
      beast::bind_front_handler(
        &session::on_accept,
        shared_from_this()));
}
} /// Client
#endif //CHAT_CLIENT_H
