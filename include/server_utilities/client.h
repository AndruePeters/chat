//
// Created by druep on 4/27/2021.
// Example I kind of what to base off of: https://github.com/boostorg/beast/blob/develop/example/websocket/server/chat-multi/websocket_session.hpp
//

#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>

#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <vector>



namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



#endif //CHAT_CLIENT_H
