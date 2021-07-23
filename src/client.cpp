#include "client.h"

#include <message.h>
#include <iostream>

#include <spdlog/spdlog.h>

namespace Client {

// Report a failure
void fail(beast::error_code ec, char const* what)
{
    // Don't report these
    if (ec == net::error::operation_aborted || ec == websocket::error::closed) return;

    spdlog::error("{}:{}", what, ec.message());
}

session::session(net::io_context& ioc)
  : resolver(net::make_strand(ioc)), ws(net::make_strand(ioc))
{
}

void session::on_accept(beast::error_code ec)
{
    if (ec) return fail(ec, "accept");

    ws.async_read(buffer, beast::bind_front_handler(&session::on_read, shared_from_this()));
}


void session::on_read(beast::error_code ec, std::size_t)
{
    if (ec) return fail(ec, "read");

    // send to all connections
    const std::string msg = beast::buffers_to_string(buffer.data());
    //ws.send(std::make_shared<const std::string>(msg));
    spdlog::info("Recieved message: {}", msg);
    buffer.consume(buffer.size());

    ws.async_read(buffer, beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void session::send(const std::shared_ptr<const std::string>& ss)
{
    // post our work to the strand, this ensures
    // that members of 'this' will not be accessed concurrently
    net::post(ws.get_executor(), beast::bind_front_handler(&session::on_send, shared_from_this(), ss));
}



void session::on_send(const std::shared_ptr<const std::string>& ss)
{
    queue.push_back(ss);

    // check if we're already writing
    if (queue.size() > 1) {
        return;
    }

    // since we are not writing, send this immediately
    ws.async_write(net::buffer(*queue.front()),
      beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void session::on_write(beast::error_code ec, std::size_t)
{
    if (ec) return fail(ec, "write");

    // remove the string from the queue
    queue.erase(queue.begin());

    // send the message if any
    if (!queue.empty()) {
        ws.async_write(net::buffer(*queue.front()),
          beast::bind_front_handler(&session::on_write, shared_from_this()));
    }
}

void session::setHost(std::string hst)
{
    this->host = std::move(hst);
}

void session::setPort(std::string prt)
{
    this->port = std::move(prt);
}

} /// Client
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
int main()
{
    const auto host = "0.0.0.0";
    const auto port = "8080";
    const auto text = "new message";

    const Message msg = {"Drue", "Sam", "hello"};

    net::io_context ioc;
    auto wsSession = std::make_shared<Client::session>(ioc); //->run(host, port, msg);
    wsSession->setHost(host);
    wsSession->setPort(port);
    wsSession->send(std::make_shared<std::string>("Hello world."));

    ioc.run();
    return 0;
}