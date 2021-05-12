//
// Created by druep on 4/27/2021.
//

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

namespace Client {


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


/// report a failure
void fail(beast::error_code ec, char const* what);

/// Sends a WebSocket message and prints the response
class session : public std::enable_shared_from_this<session> {
    tcp::resolver resolver;
    websocket::stream <beast::tcp_stream> ws;
    beast::flat_buffer buffer;
    std::string host;
    std::string text;

public:
    explicit session(net::io_context& ioc);

    void run(char const* host, char const* port, char const* text);

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results);

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);

    void on_handshake(beast::error_code ec);

    void on_write(beast::error_code ec, std::size_t bytes_transferred);

    void on_read(beast::error_code ec, std::size_t bytes_transferred);

    void on_close(beast::error_code ec);
};

} /// Client
#endif //CHAT_CLIENT_H
