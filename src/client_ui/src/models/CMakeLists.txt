# Build the ui models

add_library(ui_models STATIC client.cpp activeChatModel.cpp)
target_include_directories(ui_models PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(ui_models SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ui_models PUBLIC client_ui_shared_properties)