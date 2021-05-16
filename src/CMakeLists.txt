
set (CMAKE_INCLUDE_CURRENT_DIR  ON)

add_executable(chat_client client.cpp shared_state.cpp websocket_session.cpp)
target_include_directories(chat_client SYSTEM PRIVATE ${Boost_INCLUDE_DIRS} )
target_link_libraries(chat_client PRIVATE pthread)


add_executable(chat_server server.cpp listener.cpp http_session.cpp websocket_session.cpp shared_state.cpp)
target_include_directories(chat_server SYSTEM PRIVATE ${Boost_INCLUDE_DIRS} )
target_link_libraries(chat_server PRIVATE pthread)
