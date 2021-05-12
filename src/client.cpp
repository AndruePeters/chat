#include "client.h"

#include <message.h>

#include <iostream>

namespace Client {

// Report a failure
void fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}


session::session(net::io_context& ioc) : resolver(net::make_strand(ioc)), ws(net::make_strand(ioc))
{

}

void session::run(const char* host, const char* port, const char* text)
{
    this->host = host;
    this->text = text;

    /// what does bind_front_handler actually do?
    resolver.async_resolve(host, port, beast::bind_front_handler(&session::on_resolve, shared_from_this()));
}


void session::on_resolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if (ec) return fail(ec, "resolve");

    /// set the timeout for the operation
    beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address from the lookup
    beast::get_lowest_layer(ws).async_connect(
            results,
            beast::bind_front_handler(&session::on_connect, shared_from_this()) );
}

void session::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
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

void session::on_handshake(beast::error_code ec)
{
    if (ec) return fail(ec, "handshake");

    ws.async_write(net::buffer(text), beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void session::on_write(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "write");

    // read a message into the buffer
    ws.async_read(buffer, beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void session::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec) return fail(ec, "read");

    ws.async_close(websocket::close_code::normal, beast::bind_front_handler(&session::on_close, shared_from_this()));
}

void session::on_close(beast::error_code ec)
{
    if (ec) return fail(ec, "close");

    // if we get here then the connection is closed gracefully

    // the make_printable() function helps print a ConstBufferSequence
    std::cout << beast::make_printable(buffer.data()) << std::endl;
}

} /// Client

int main()
{
    return 0;
}