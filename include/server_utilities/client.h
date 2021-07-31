//
// Created by druep on 4/27/2021.
//

/// Probably need to add a queue similar to the server
/// so that we can send the messages
#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "message.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <spdlog/spdlog.h>

namespace Client {

namespace beast     = boost::beast;    // from <boost/beast.hpp>
namespace http      = beast::http;    // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;    // from <boost/beast/websocket.hpp>
namespace net       = boost::asio;    // from <boost/asio.hpp>
using tcp           = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>


} /// Client
#endif //CHAT_CLIENT_H
