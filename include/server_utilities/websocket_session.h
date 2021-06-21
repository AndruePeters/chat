//
// Created by druep on 4/27/2021.
// Example I kind of what to base off of: https://github.com/boostorg/beast/blob/develop/example/websocket/server/chat-multi/websocket_session.hpp
//

#ifndef CHAT_WEBSOCKET_SESSION_H
#define CHAT_WEBSOCKET_SESSION_H

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <vector>


class shared_state;

namespace beast     = boost::beast;    // from <boost/beast.hpp>
namespace http      = beast::http;    // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;    // from <boost/beast/websocket.hpp>
namespace net       = boost::asio;    // from <boost/asio.hpp>
using tcp           = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>


/// report a failure
void fail(beast::error_code ec, char const* what);

/// Sends a WebSocket message and prints the response
class websocket_session : public std::enable_shared_from_this<websocket_session>
{
    beast::flat_buffer buffer;
    websocket::stream<beast::tcp_stream> ws;
    std::shared_ptr<shared_state> state;
    std::vector<std::shared_ptr<const std::string>> queue;

  public:
    websocket_session(tcp::socket&& socket, const std::shared_ptr<shared_state>& state);

    ~websocket_session();

    template<class Body, class Allocator>
    void run(http::request<Body, http::basic_fields<Allocator>> req);

    void send(const std::shared_ptr<const std::string>& ss);

  private:
    void on_send(const std::shared_ptr<const std::string>& ss);

    void on_accept(beast::error_code ec);

    void on_read(beast::error_code ec, std::size_t bytesTransferred);

    void on_write(beast::error_code ec, std::size_t bytesTransferred);
};

template<class Body, class Allocator>
void websocket_session::run(http::request<Body, http::basic_fields<Allocator>> req)
{
    // Set suggested timeout settings for the websocket
    ws.set_option(
      websocket::stream_base::timeout::suggested(
        beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws.set_option(websocket::stream_base::decorator(
      [](websocket::response_type& res) {
          res.set(http::field::server,
            std::string(BOOST_BEAST_VERSION_STRING) + " websocket-chat-multi");
      }));

    // Accept the websocket handshake
    ws.async_accept(
      req,
      beast::bind_front_handler(
        &websocket_session::on_accept,
        shared_from_this()));
}

#endif    //CHAT_CLIENT_H
