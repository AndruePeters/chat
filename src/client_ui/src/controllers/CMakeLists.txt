# build the user interface controllers and make them a static library

add_library(ui_controllers STATIC 
    main_controller.cpp
    settings_controller.cpp
)

target_link_libraries(ui_controllers PUBLIC client_ui_shared_properties Qt6::Qml)