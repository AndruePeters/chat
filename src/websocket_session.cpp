#include "websocket_session.h"
#include "shared_state.h"

#include <spdlog/spdlog.h>

// Report a failure
void fail(beast::error_code ec, char const* what)
{
    // Don't report these
    if (ec == net::error::operation_aborted || ec == websocket::error::closed) return;

    spdlog::error("{}:{}", what, ec.message());
}

websocket_session::websocket_session(tcp::socket&& socket, const std::shared_ptr<shared_state>& state)
  : ws(std::move(socket)), state(state)
{
}

websocket_session::~websocket_session()
{
    state->leave(this);
}


void websocket_session::on_accept(beast::error_code ec)
{
    if (ec) return fail(ec, "accept");

    state->join(this);

    ws.async_read(buffer, beast::bind_front_handler(&websocket_session::on_read, shared_from_this()));
}


void websocket_session::on_read(beast::error_code ec, std::size_t)
{
    if (ec) return fail(ec, "read");
    spdlog::debug("on_read");

    // send to all connections
    state->send(beast::buffers_to_string(buffer.data()));

    buffer.consume(buffer.size());

    ws.async_read(buffer, beast::bind_front_handler(&websocket_session::on_read, shared_from_this()));
}

void websocket_session::send(const std::shared_ptr<const std::string>& ss)
{
    // post our work to the strand, this ensures
    // that members of 'this' will not be accessed concurrently
    net::post(ws.get_executor(), beast::bind_front_handler(&websocket_session::on_send, shared_from_this(), ss));
}

void websocket_session::on_send(const std::shared_ptr<const std::string>& ss)
{
    queue.push_back(ss);

    // check if we're already writing
    if (queue.size() > 1) {
        return;
    }

    // since we are not writing, send this immediately
    ws.async_write(net::buffer(*queue.front()),
      beast::bind_front_handler(&websocket_session::on_write, shared_from_this()));
}

void websocket_session::on_write(beast::error_code ec, std::size_t)
{
    if (ec) return fail(ec, "write");

    // remove the string from the queue
    queue.erase(queue.begin());

    // send the message if any
    if (!queue.empty()) {
        ws.async_write(net::buffer(*queue.front()),
          beast::bind_front_handler(&websocket_session::on_write, shared_from_this()));
    }
}
