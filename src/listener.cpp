#include "listener.h"
#include "http_session.h"

#include <spdlog/spdlog.h>

namespace Server {

listener::listener(net::io_context& ioc, tcp::endpoint endpoint, const std::shared_ptr<shared_state>& state)
  : ioc(ioc), acceptor(ioc), state(state)
{
    beast::error_code ec;

    acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        fail(ec, "open");
        return;
    }

    // allow address reuse
    acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "set_option");
        return;
    }

    // bind to the server address
    acceptor.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind");
        return;
    }

    // start listening for connections
    acceptor.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen");
        return;
    }
}

void listener::run()
{
    // the new connection gets its own strand
    acceptor.async_accept(
      net::make_strand(ioc),
      beast::bind_front_handler(
        &listener::on_accept, shared_from_this()));
}

void listener::fail(beast::error_code ec, char const* what)
{
    if (ec == net::error::operation_aborted) return;

    spdlog::error("{}:{}", what, ec.message());
}

void listener::on_accept(beast::error_code ec, tcp::socket socket)
{
    if (ec) {
        return fail(ec, "accept");
    } else {
        std::make_shared<http_session>(std::move(socket), state)->run();
    }

    acceptor.async_accept(net::make_strand(ioc), beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}


}    // namespace Server