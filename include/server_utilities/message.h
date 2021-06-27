//
// Created by druep on 4/27/2021.
//

#ifndef BEAST_MESSAGE_H
#define BEAST_MESSAGE_H

#include <string>
#include <nlohmann/json.hpp>

/// the most basic message
struct Message {
    std::string from;
    std::string to;
    std::string message;

    bool operator==(const Message&m1) const = default;
};

nlohmann::json to_json(const Message& message)
{
    return {
        { "from", message.from },
        { "to", message.to },
        { "message", message.message }
    };
}

Message to_message(const nlohmann::json& j)
{
    Message message;
    message.from = j["from"];
    message.to = j["to"];
    message.message = j["message"];
    return message;
}
#endif //BEAST_MESSAGE_H
