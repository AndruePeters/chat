//
// Created by druep on 4/27/2021.
//

#ifndef CHAT_USER_H
#define CHAT_USER_H

#include "message.h"

#include <string_view>

/// This version of a chat user only has a sender string and can send a message from the sender
class ChatUser {
    std::string sender = "anonymous";
public:
    void sendMessage(std::string_view message);
};

#endif //CHAT_USER_H
