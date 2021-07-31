#include <websocket.h>

#include <thread>

WebSocketImpl::WebSocketImpl(WebSocket& socketWrapper, net::io_context& ioc, std::string_view host, std::string_view port)
  : webSocket(net::make_strand(ioc)),  resolver(net::make_strand(ioc)),
    onOpenUserHandler(socketWrapper.onOpen), onMessageUserHandler(socketWrapper.onMessage), onCloseUserHandler(socketWrapper.onClose), onErrorUserHandler(socketWrapper.onError),
    host(host), port(port)
{

}

void WebSocketImpl::send(const std::shared_ptr<const std::string>& ss)
{
    net::post(webSocket.get_executor(), beast::bind_front_handler(&WebSocketImpl::onSend, shared_from_this(), ss));
}

void WebSocketImpl::close()
{
    webSocket.async_close(beast::websocket::close_code::normal, beast::bind_front_handler(&WebSocketImpl::onClose, shared_from_this()));
}

void WebSocketImpl::fail(beast::error_code ec, char const* what)
{
    /// If an operation failed because of closing, then just send the onClose event
    if (ec == websocket::error::closed) {
        net::post(webSocket.get_executor(), [this] {
           CloseEvent ce;
           onCloseUserHandler(std::move(ce));
        });

        return;
    }

    /// Send any other error to the users callback
    net::post(webSocket.get_executor(), [this, ec, what] {
       ErrorEvent ee { ec, what };
       onErrorUserHandler(std::move(ee));
    });
}

void WebSocketImpl::run()
{
    resolver.async_resolve(host, port, beast::bind_front_handler(&WebSocketImpl::onResolve, shared_from_this()));
}

void WebSocketImpl::onResolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if (ec) {
        return fail(ec, "resolve");
    }

    beast::get_lowest_layer(webSocket).expires_after(std::chrono::seconds(30));
    beast::get_lowest_layer(webSocket).async_connect( results, beast::bind_front_handler(&WebSocketImpl::onConnect, shared_from_this()));
}

void WebSocketImpl::onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
{
    if (ec) {
        return fail (ec, "connect");
    }

    beast::get_lowest_layer(webSocket).expires_never();

    webSocket.set_option(websocket::stream_base::decorator(
      [](websocket::request_type& req) {
             req.set(http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) );
      }));

    host += ':' + std::to_string(ep.port());

    webSocket.async_handshake(host, "/", beast::bind_front_handler(&WebSocketImpl::onHandshake, shared_from_this()));
}

void WebSocketImpl::onHandshake(beast::error_code ec)
{
    if (ec) {
        return fail(ec, "handshake");
    }

    net::post([this]() {
        OpenEvent oe;
        this->onOpenUserHandler(std::move(oe));
    });

    net::post(webSocket.get_executor(), beast::bind_front_handler(&WebSocketImpl::onRead, shared_from_this(), ec, 0));
}

void WebSocketImpl::onRead(beast::error_code ec, std::size_t bytesTransferred)
{
    using namespace std::chrono_literals;

    if (ec) {
        return fail(ec, "read");
    }

    net::post([this] {
        MessageEvent msgEvent { beast::buffers_to_string(this->buffer.data()) };
        onMessageUserHandler(std::move(msgEvent));
    });

    spdlog::info("Read message of size: {}", buffer.size());
    buffer.consume(buffer.size());
    webSocket.async_read(buffer, beast::bind_front_handler(&WebSocketImpl::onRead, shared_from_this()));
}


void WebSocketImpl::onClose(beast::error_code ec)
{
    spdlog::info("onClose() was called");

    // report errors other than close
    if (ec && ec != websocket::error::closed) {
        return fail(ec, "close");
    }

    net::post(webSocket.get_executor(), [this] {
       CloseEvent ce;
       onCloseUserHandler(std::move(ce));
    });
}


void WebSocketImpl::onSend(const std::shared_ptr<const std::string>& ss)
{
    queue.push_back(ss);

    if (queue.size() > 1) {
        return;
    }

    webSocket.async_write(net::buffer(*queue.front()), beast::bind_front_handler(&WebSocketImpl::onWrite, shared_from_this()));
}


void WebSocketImpl::onWrite(beast::error_code ec, std::size_t bytesTransferred)
{
    if (ec) {
        return fail(ec, "write");
    }

    queue.erase(queue.begin());

    if (! queue.empty() ) {
        webSocket.async_write(net::buffer(*queue.front()),
          beast::bind_front_handler(&WebSocketImpl::onWrite, shared_from_this()));
    }
}


WebSocket::WebSocket(std::string_view url)
{
    this->webSocketImpl = std::make_shared<WebSocketImpl>(*this, ioContext, url, "8080");
    webSocketImpl->run();
    ioContextThread =  std::thread(
      [this, url]() {
        ioContext.run();
    });
}


void WebSocket::send(std::string message)
{
    webSocketImpl->send(std::make_shared<std::string>(message));
}


void WebSocket::close()
{
    webSocketImpl->close();
}

WebSocket::~WebSocket()
{
    ioContext.stop();
    ioContextThread.join();
}