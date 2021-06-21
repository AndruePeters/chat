#include "shared_state.h"
#include "client.h"


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

int main()
{
    const auto host = "0.0.0.0";
    const auto port = "8080";
    const auto text = "new message";

    /*net::io_context ioc;

    std::string userInput;
    while (std::getline(std::cin, userInput)) {
        if (userInput == "quit") {
            break;
        }
        ioc.run();
    }
*/
    return 0;
}