

add_library(cm-lib STATIC
        src/models/client.cpp
        src/controllers/main-controller.cpp
        )

target_include_directories(cm-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
target_link_libraries(cm-lib PRIVATE Qt6::Core  )
target_compile_definitions(cm-lib PRIVATE CMLIB_LIBRARY)

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
