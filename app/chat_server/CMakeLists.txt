add_executable(chat_server_cli server.cpp)
target_include_directories(chat_server_cli SYSTEM PRIVATE ${Boost_INCLUDE_DIRS} )
target_link_libraries(chat_server_cli PRIVATE server_utilities pthread CONAN_PKG::spdlog CONAN_PKG::fmt)
