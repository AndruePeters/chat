#ifndef WEBSOCKET_H
#define WEBSOCKET_H

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

#include <spdlog/spdlog.h>

namespace Network::Client {

namespace net = boost::asio;

class WebSocketImpl;

/// Provides an API to the websocket inspired by JavaScript
class WebSocket
{

    net::io_context ioContext{};
    std::thread ioContextThread{};
    std::shared_ptr<WebSocketImpl> webSocketImpl = nullptr;

  public:
    std::function<void(OpenEvent&&)> onOpen       = [](OpenEvent&&) {};
    std::function<void(MessageEvent&&)> onMessage = [](MessageEvent&&) {};
    std::function<void(CloseEvent&&)> onClose     = [](CloseEvent&&) {};
    std::function<void(ErrorEvent&&)> onError     = [](ErrorEvent&&) {};

    WebSocket(std::string_view url, std::string_view port);
    ~WebSocket();

    void send(std::string message);

    void close();
};

}
#endif    //WEBSOCKET_H
