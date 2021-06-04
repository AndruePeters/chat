# build the UI application

# add an interface library with settings common both to models and controllers
add_library(client_ui_shared_properties INTERFACE)
target_include_directories(client_ui_shared_properties INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(client_ui_shared_properties INTERFACE Qt6::Core)

# build these libraries
add_subdirectory(src/controllers)
add_subdirectory(src/models)

# create an interface library to keep things parallel to the book, but this will get removed
# once I better learn this system
add_library(cm-lib INTERFACE)
target_link_libraries(cm-lib INTERFACE ui_controllers ui_models)
target_compile_definitions(cm-lib INTERFACE CMLIB_LIBRARY)

if(ANDROID)
    add_library(cm-ui SHARED
            src/main.cpp
            views.qrc
            )
else()
    add_executable(cm-ui
            src/client_main.cpp
            views.qrc
            )
endif()

target_compile_definitions(cm-ui PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(cm-ui PRIVATE Qt6::Core Qt6::Quick Qt6::Qml project_options project_warnings cm-lib)
