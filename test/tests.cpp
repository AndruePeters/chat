#include <catch2/catch.hpp>

#include <message.h>
#include <string>
#include <vector>

/// let's test serialization with this interesting vector of messages
const std::vector<Message> messages {
        {"Sam", "Drue", "Poop is brown."},
        {"Sam", "Drue", "Toe jam is stinky."},
        {"Drue", "Sam", "This is not going online."}
};

const std::vector<std::string> messageStrings {
    R"({"from":"Sam","to":"Drue","message":"Poop is brown."})",
    R"({"from":"Sam","to":"Drue","message":"Toe jam is stinky."})",
    R"({"from":"Drue","to":"Sam","message":"This is not going online."})"
};

/// helper method to serialize a message
std::string serialize(const Message& message)
{
    const auto json = to_json(message);
    return json.dump();
}

Message deserialize(const std::string& str)
{
    nlohmann::json val = nlohmann::json::parse(str);
    return to_message(val);
}

TEST_CASE("Message Serialization", "[serialize]")
{
    REQUIRE( serialize(messages[0])  == R"({"from":"Sam","to":"Drue","message":"Poop is brown."})" );
    REQUIRE( serialize(messages[1])  == R"({"from":"Sam","to":"Drue","message":"Toe jam is stinky."})" );
    REQUIRE( serialize(messages[2])  == R"({"from":"Drue","to":"Sam","message":"This is not going online."})" );
}

TEST_CASE("Message Parsing", "[deserialize]")
{
    Message m0 = deserialize(messageStrings[0]);
    Message m1 = deserialize(messageStrings[1]);
    Message m2 = deserialize(messageStrings[2]);

    REQUIRE( m0 == messages[0] );
    REQUIRE( m1 == messages[1] );
    REQUIRE( m2 == messages[2] );
}
