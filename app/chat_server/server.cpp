#include "listener.h"
#include "shared_state.h"

#include <spdlog/spdlog.h>

#include <boost/asio/signal_set.hpp>
#include <boost/smart_ptr.hpp>

#include <vector>

namespace beast     = boost::beast;    // from <boost/beast.hpp>
namespace http      = beast::http;    // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;    // from <boost/beast/websocket.hpp>
namespace net       = boost::asio;    // from <boost/asio.hpp>
using tcp           = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>

int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 5) {
        spdlog::error("Usage: {} <address> <port> <doc_root> <threads>\n"
          "Example:\n"
          "    {} 0.0.0.0 8080 . 5\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }
    auto address       = net::ip::make_address(argv[1]);
    auto port          = static_cast<unsigned short>(std::atoi(argv[2]));
    auto doc_root      = argv[3];
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    // The io_context is required for all I/O
    net::io_context ioc;

    // Create and launch a listening port
    std::make_shared<Server::listener>(
      ioc,
      tcp::endpoint{ address, port },
      std::make_shared<shared_state>(doc_root))
      ->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
      [&ioc](boost::system::error_code const&, int) {
          // Stop the io_context. This will cause run()
          // to return immediately, eventually destroying the
          // io_context and any remaining handlers in it.
          ioc.stop();
      });

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i)
        v.emplace_back(
          [&ioc] {
              ioc.run();
          });
    ioc.run();

    // (If we get here, it means we got a SIGINT or SIGTERM)

    // Block until all the threads exit
    for (auto& t : v)
        t.join();

    return EXIT_SUCCESS;
}
