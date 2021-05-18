//
// Created by druep on 4/27/2021.
//

#ifndef BEAST_MESSAGE_H
#define BEAST_MESSAGE_H

#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <string>

/// the most basic message
struct Message {
    std::string from;
    std::string to;
    std::string message;
};

/// Helper function for tag_invoke
template<class T>
void extract( const boost::json::object& obj, T& t, boost::string_view key )
{
    t = boost::json::value_to<T>( obj.at( key ) );
}

/// Serializes a Message object
void tag_invoke( boost::json::value_from_tag, boost::json::value& jv, const Message& message )
{
    jv = {
            { "from" , message.from },
            { "to",    message.to },
            { "message", message.message }
    };
}

/// de-Serializes a json object to Message
Message tag_invoke( boost::json::value_to_tag< Message >, boost::json::value const& jv )
{
    Message m;
    boost::json::object const& obj = jv.as_object();
    extract( obj, m.from, "from" );
    extract (obj, m.to, "to");
    extract( obj, m.message, "message" );
    return m;
}

#endif //BEAST_MESSAGE_H
