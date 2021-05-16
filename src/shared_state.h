#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>

// forward declaration
class websocket_session;


// represents the shared server state
class shared_state {
    const std::string doc_root;

    // this mutex synchronizes all access to sessions
    std::mutex mutex;

    // keep a list of all the connected clients
    std::unordered_set<websocket_session *> sessions;

public:
    explicit shared_state(std::string doc_root);

    const std::string &getDocRoot() const noexcept {
        return doc_root;
    }

    void join(websocket_session *session);

    void leave(websocket_session *session);

    void send(std::string message);
};


#endif