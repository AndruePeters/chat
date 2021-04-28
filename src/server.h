#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "message.h"

#include <vector>
/// This server needs to handle clients
class ChatServer
{
    std::vector<int> clients;
};
#endif