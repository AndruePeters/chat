//
// Created by druep on 7/31/2021.
//

#ifndef CHAT_EVENTS_H
#define CHAT_EVENTS_H

#include <string>
#include <boost/beast/core/error.hpp>
#include <boost/beast/websocket/error.hpp>

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
    boost::beast::error_code ec = boost::beast::websocket::error::closed;
    std::string what { };
};

#endif    //CHAT_EVENTS_H
