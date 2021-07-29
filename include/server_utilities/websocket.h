#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <cstdlib>
#include <functional>
#include <memory>
#include <string_view>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>


namespace beast     = boost::beast;    // from <boost/beast.hpp>
namespace http      = beast::http;    // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;    // from <boost/beast/websocket.hpp>
namespace net       = boost::asio;    // from <boost/asio.hpp>
using tcp           = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>

class WebSocket;

struct OpenEvent {

};

struct MessageEvent {
    std::string data;
};

struct CloseEvent {
    int reason = 0;
    int code = 0;
};

struct ErrorEvent {
    beast::error_code ec = websocket::error::closed;
    std::string what { };
};

class WebSocketImpl : public std::enable_shared_from_this<WebSocketImpl> {
  private:
    beast::flat_buffer buffer; // buffer used for messages
    websocket::stream<beast::tcp_stream> webSocket; // our websocket stream
    std::vector<std::shared_ptr<const std::string>> queue; // send message queue
    tcp::resolver resolver; // resolver for the endpoint

  public:
    std::function<void(OpenEvent&&)>& onOpenUserHandler;
    std::function<void(MessageEvent&&)>& onMessageUserHandler;
    std::function<void(CloseEvent&&)>& onCloseUserHandler;
    std::function<void(ErrorEvent&&)>& onErrorUserHandler;

  private:
    std::string host;
    std::string port;

  public:
    WebSocketImpl(WebSocket& socketWrapper, net::io_context& ioc, std::string_view host, std::string_view port);
    void send(const std::shared_ptr<const std::string>& ss);
    void close();

  private:
    void fail(beast::error_code ec, char const* what);
    void onSend(const std::shared_ptr<const std::string>& ss);
    void onRead(beast::error_code ec, std::size_t bytesTransferred);
    void onWrite(beast::error_code ec, std::size_t bytesTransferred);
    void onResolve(beast::error_code ec, tcp::resolver::results_type results);
    void onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
    void onHandshake(beast::error_code ec);
    void onClose(beast::error_code ec);
};

/// Provides an API to the websocket inspired by JavaScript
class WebSocket {

    static net::io_context ioContext;
    WebSocketImpl webSocketImpl;

  public:
    std::function<void(OpenEvent&&)> onOpen = [](OpenEvent&&) { };
    std::function<void(MessageEvent&&)> onMessage = [](MessageEvent&&) { };
    std::function<void(CloseEvent&&)> onClose = [](CloseEvent&&) { };
    std::function<void(ErrorEvent&&)> onError = [](ErrorEvent&&) { } ;

    WebSocket(std::string_view url);

    void send(std::string message);

    void close();
};


#endif    //WEBSOCKET_H
