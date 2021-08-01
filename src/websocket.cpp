#include <websocket.h>
#include <websocket_impl.h>

#include <thread>

namespace Network::Client {


WebSocket::WebSocket(std::string_view url)
{
    this->webSocketImpl = std::make_shared<WebSocketImpl>(*this, ioContext, url, "8080");
    webSocketImpl->run();
    ioContextThread = std::thread(
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

}