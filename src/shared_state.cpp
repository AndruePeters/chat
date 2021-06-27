#include "shared_state.h"
#include "client.h"
#include "websocket_session.h"

#include "message.h"

#include <spdlog/spdlog.h>

shared_state::shared_state(std::string doc_root) : doc_root(std::move(doc_root))
{
}

void shared_state::join(websocket_session* session)
{
    std::lock_guard<std::mutex> lock(mutex);
    sessions.insert(session);
}

void shared_state::leave(websocket_session* session)
{
    std::lock_guard<std::mutex> lock(mutex);
    sessions.erase(session);
}

// broadcast a message to all websocket client sessions
void shared_state::send(std::string message)
{
    spdlog::info("Incoming JSON string: {}", message);

    nlohmann::json val = nlohmann::json::parse(message);
    Message msg  = to_message(val);

    // make a local list of all the weak pointers representing the sessions so that we an do the
    // actual sending without holding the mutex
    auto v = std::vector<std::weak_ptr<websocket_session>>();
    {
        std::lock_guard<std::mutex> lock(mutex);
        v.reserve(sessions.size());
        for (auto p : sessions) {
            v.emplace_back(p->weak_from_this());
        }
    }

    const auto ss = std::make_shared<const std::string>(msg.message);

    // for each session in our local list, try to acquire a strong pointer. If successful,
    // then send the message on that session
    for (const auto& wp : v) {
        if (auto strongPointer = wp.lock()) {
            strongPointer->send(ss);
        }
    }
}
