
set (CMAKE_INCLUDE_CURRENT_DIR  ON)

add_library(server_utilities STATIC http_session.cpp shared_state.cpp websocket_session.cpp listener.cpp websocket.cpp)

target_include_directories(server_utilities PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/server_utilities>
    $<INSTALL_INTERFACE:include/server_utilities>
    ${Boost_INCLUDE_DIRS}
)

target_include_directories(server_utilities PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${Boost_INCLUDE_DIRS}
)


target_link_libraries(server_utilities PUBLIC CONAN_PKG::spdlog CONAN_PKG::fmt Qt6::Core CONAN_PKG::nlohmann_json)


add_executable(chat_client_cli client.cpp )
target_include_directories(chat_client_cli SYSTEM PRIVATE ${Boost_INCLUDE_DIRS} )
target_link_libraries(chat_client_cli PUBLIC server_utilities pthread CONAN_PKG::spdlog CONAN_PKG::fmt CONAN_PKG::nlohmann_json)

