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

    /// Sets the host for this session
    void setHost(std::string host);

    /// Sets the port for this session
    void setPort(std::string port);

    /// Connect to the server
    void connect();

    /// Send the message
    /// This should ultimately be changed to accept a Message object
    /// Or a JSON object. Not sure at the moment
    void send(const std::shared_ptr<const std::string>& ss);

  private:
    void on_send(const std::shared_ptr<const std::string>& ss);

    void on_read(beast::error_code ec, std::size_t bytesTransferred);

    void on_write(beast::error_code ec, std::size_t bytesTransferred);

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results);

    /// Get's called by on_resolve() after on_resolve()
    /// Need to change the name of this function or the public facing connect()
    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);

    void on_handshake(beast::error_code ec);
};

} /// Client
#endif //CHAT_CLIENT_H
