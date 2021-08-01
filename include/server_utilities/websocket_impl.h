//
// Created by druep on 8/1/2021.
//

#ifndef CHAT_WEBSOCKET_IMPL_H
#define CHAT_WEBSOCKET_IMPL_H

#include <events.h>

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <functional>
#include <memory>
#include <string_view>
#include <string>
#include <thread>
#include <vector>


namespace Network::Client {

namespace beast     = boost::beast;    // from <boost/beast.hpp>
namespace http      = beast::http;    // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;    // from <boost/beast/websocket.hpp>
namespace net       = boost::asio;    // from <boost/asio.hpp>
using tcp           = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>

class WebSocket;

class WebSocketImpl : public std::enable_shared_from_this<WebSocketImpl>
{
    friend class WebSocket;

    beast::flat_buffer buffer;    // buffer used for messages
    websocket::stream<beast::tcp_stream> webSocket;    // our websocket stream
    std::vector<std::shared_ptr<const std::string>> queue;    // send message queue
    tcp::resolver resolver;    // resolver for the endpoint

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
    void run();
    void onSend(const std::shared_ptr<const std::string>& ss);
    void onRead(beast::error_code ec, std::size_t bytesTransferred);
    void onWrite(beast::error_code ec, std::size_t bytesTransferred);
    void onResolve(beast::error_code ec, tcp::resolver::results_type results);
    void onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
    void onHandshake(beast::error_code ec);
    void onClose(beast::error_code ec);
};


}

#endif    //CHAT_WEBSOCKET_IMPL_H
