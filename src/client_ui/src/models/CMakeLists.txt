# Build the ui models

add_library(ui_models STATIC client.cpp messageBoxCppModel.cpp)
target_link_libraries(ui_models PUBLIC client_ui_shared_properties)