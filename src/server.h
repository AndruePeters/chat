#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "message.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace Server {

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class session: public std::enable_shared_from_this<session> {
    websocket::stream<beast::tcp_stream> ws;
    beast::flat_buffer buffer;

public:
    explicit session(tcp::socket&& socket);

    void run();
    void on_run();
    void on_accept(beast::error_code ec);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
};

class listener : public std::enable_shared_from_this<listener> {
    net::io_context& ioc;
    tcp::acceptor acceptor;

public:
    listener(net::io_context& ioc, tcp::endpoint endpoint);
    void run();

private:
    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
};


} /// Server
#endif