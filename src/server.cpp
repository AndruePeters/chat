#include <server.h>

namespace Server {
void fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

session::session(tcp::socket&& socket) : ws(std::move(socket))
{

}

void session::run()
{
    std::cout << "status: run" << std::endl;

    net::dispatch(ws.get_executor(), beast::bind_front_handler(&session::on_run, shared_from_this()));
}

void session::on_run()
{
    std::cout << "status: on_run" << std::endl;

    // set the suggested timeout settings for the websocket
    ws.set_option( websocket::stream_base::timeout::suggested(beast::role_type::server) );
    ws.set_option( websocket::stream_base::decorator(
            [](websocket::response_type& res) {
                res.set(http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-async");
            }));

    ws.async_accept( beast::bind_front_handler(&session::on_accept, shared_from_this()));
}

void session::on_accept(beast::error_code ec)
{
    std::cout << "status: on_accept()" << std::endl;
    if (ec) return fail(ec, "accept");

    do_read();
}

void session::do_read()
{
    std::cout << "status: do_read()" << std::endl;

    ws.async_read(buffer, beast::bind_front_handler( &session::on_read, shared_from_this()));
}
void session::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    std::cout << "status: on_read" << std::endl;
    boost::ignore_unused(bytes_transferred);

    if (ec == websocket::error::closed) return;
    if (ec) fail(ec, "read");

    ws.text(ws.got_text());
    std::cout << beast::make_printable(buffer.data()) << std::endl;

    ws.async_write(buffer.data(), beast::bind_front_handler( &session::on_write, shared_from_this()));
}

void session::on_write( beast::error_code ec, std::size_t bytes_transferred)
{
    std::cout << "status: on_write()" << std::endl;
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "write");

    buffer.consume(buffer.size());

    do_read();
}


listener::listener( net::io_context& ioc, tcp::endpoint endpoint): ioc(ioc), acceptor(ioc)
{
    beast::error_code ec;

    acceptor.open(endpoint.protocol(), ec);

    if (ec) {
        fail(ec, "open");
        return;
    }

    acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "set_option");
        return;
    }

    acceptor.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind");
        return;
    }

    acceptor.listen( net::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen");
        return;
    }
}

void listener::run()
{
    do_accept();
}

void listener::do_accept()
{
    acceptor.async_accept(net::make_strand(ioc), beast::bind_front_handler( &listener::on_accept, shared_from_this()));
}

void listener::on_accept(beast::error_code ec, tcp::socket socket)
{
    if (ec) {
        fail(ec, "accept");
        return;
    }

    std::make_shared<session>(std::move(socket))->run();
    do_accept();
}

}
int main()
{
    return 0;
}