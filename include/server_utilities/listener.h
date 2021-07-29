#ifndef LISTENER_H
#define LISTENER_H

#include <memory>
#include <string>

#include <boost/beast.hpp>
#include <boost/asio.hpp>


class shared_state;

namespace Server {

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

/// report a failure
void fail(beast::error_code ec, char const* what);


class listener : public std::enable_shared_from_this<listener> {
    net::io_context& ioc;
    tcp::acceptor acceptor;
    std::shared_ptr<shared_state> state_;

    void on_accept(beast::error_code ec, tcp::socket socket);
    void fail(beast::error_code ec, char const* what);

public:
    listener(net::io_context& ioc, tcp::endpoint endpoint, const std::shared_ptr<shared_state>& state);

    // start accepting incoming connections
    void run();
};
}
#endif