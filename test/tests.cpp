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

/// helper method to serialize a message
std::string serialize(const Message& message)
{
    return boost::json::serialize ( boost::json::value_from(message) );
}

TEST_CASE("Message Serialization", "[serialize]")
{
    REQUIRE( serialize(messages[0])  == R"({"from":"Sam","to":"Drue","message":"Poop is brown."})" );
    REQUIRE( serialize(messages[1])  == R"({"from":"Sam","to":"Drue","message":"Toe jam is stinky."})" );
    REQUIRE( serialize(messages[2])  == R"({"from":"Drue","to":"Sam","message":"This is not going online."})" );
}
