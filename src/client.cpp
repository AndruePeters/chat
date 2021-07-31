#include "client.h"

#include <message.h>
#include <websocket.h>

#include <chrono>
#include <thread>


#include <spdlog/spdlog.h>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

int main()
{
    using namespace std::chrono_literals;

    /// hardcoded for temporary testing
    const auto host = "0.0.0.0";
    const auto port = "8080";

    /// everything will eventually be a message object
    /// convert from Message to a json string
    const Message msg = {"Drue", "Sam", "you have mail"};
    const nlohmann::json msgJson = to_json(msg);
    const std::string msgJsonStr = msgJson.dump();


    /// Create our websocket
    WebSocket ws(host);

    ws.onMessage = [](MessageEvent&& msgEvent) {
        spdlog::info("Message Received: {}", msgEvent.data);
    };

    ws.onClose = [](CloseEvent&& closeEvent) {
        spdlog::info("Closing the connection.\n");
    };

    ws.onOpen = [](OpenEvent&& openEvent) {
        spdlog::info("Websocket connection opened.");
    };

    ws.onError = [](ErrorEvent&& errorEvent) {
        spdlog::error("{} {}", errorEvent.what, errorEvent.ec.message());
    };

    /// this thread sends the same periodically
    /// used to test keeping a websocket alive while sending multiple messages
    std::thread messageRepeater( [&] {
        int counter = 0;
        while (true) {
            std::this_thread::sleep_for(2s);
            if (counter == 10) {
                ws.close();
                return;
            }
            ws.send(msgJsonStr);
            ++counter;
        }
    });

    messageRepeater.join();
    return 0;
}