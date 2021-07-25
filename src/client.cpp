#include "client.h"

#include <message.h>

#include <chrono>
#include <thread>


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

void session::connect()
{
    resolver.async_resolve(this->host, this->port, beast::bind_front_handler(&session::on_resolve, shared_from_this()));
}


void session::setHost(std::string hst)
{
    this->host = std::move(hst);
}

void session::setPort(std::string prt)
{
    this->port = std::move(prt);
}

void session::send(const std::shared_ptr<const std::string>& ss)
{
    spdlog::debug("Attempting to send {}", *ss);
    // post our work to the strand, this ensures
    // that members of 'this' will not be accessed concurrently
    net::post(ws.get_executor(), beast::bind_front_handler(&session::on_send, shared_from_this(), ss));
}

void session::on_send(const std::shared_ptr<const std::string>& ss)
{
    spdlog::debug("Sending message");
    queue.push_back(ss);

    // check if we're already writing
    if (queue.size() > 1) {
        return;
    }

    // since we are not writing, send this immediately
    ws.async_write(net::buffer(*queue.front()),
                   beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void session::on_read(beast::error_code ec, std::size_t)
{
    if (ec) return fail(ec, "read");
    spdlog::debug("Read incoming message.");

    const std::string msg = beast::buffers_to_string(buffer.data());
    spdlog::info("Received message: {}", msg);
    buffer.consume(buffer.size());

    ws.async_read(buffer, beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void session::on_write(beast::error_code ec, std::size_t)
{
    if (ec) return fail(ec, "write");
    spdlog::debug("Write to buffer successful");

    // remove the string from the queue
    queue.erase(queue.begin());

    // send the message if any
    if (!queue.empty()) {
        ws.async_write(net::buffer(*queue.front()),
          beast::bind_front_handler(&session::on_write, shared_from_this()));
    }
}

void session::on_resolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if (ec) return fail(ec, "resolve");
    spdlog::debug("Resolve successful.");
    beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

    beast::get_lowest_layer(ws).async_connect(
      results,
      beast::bind_front_handler(&session::on_connect, shared_from_this()) );
}

void session::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
{
    if (ec) return fail(ec, "connect");
    spdlog::debug("Connected to server.");

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
    spdlog::debug("Handshake successful.");
}


} /// Client
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
int main()
{
    using namespace std::chrono_literals;
    using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

    /// hardcoded for temporary testing
    const auto host = "0.0.0.0";
    const auto port = "8080";

    /// everything will eventually be a message object
    /// convert from Message to a json string
    const Message msg = {"Drue", "Sam", "hello"};
    const nlohmann::json msgJson = to_json(msg);
    const std::string msgJsonStr = msgJson.dump();

    /// main io context
    net::io_context ioc;

    /// work guard prevents ioc from ending too early
    work_guard_type workGuard(ioc.get_executor());

    /// now we create a session!
    auto wsSession = std::make_shared<Client::session>(ioc); //->run(host, port, msg);
    wsSession->setHost(host);
    wsSession->setPort(port);
    wsSession->connect();

    /// this thread sends the same periodically
    /// used to test keeping a websocket alive while sending multiple messages
    std::thread messageRepeater( [&] {
        while (true) {
            std::this_thread::sleep_for(10s);
            wsSession->send(std::make_shared<std::string>(msgJsonStr));
        }
    });

    /// start our work
    ioc.run();
    return 0;
}