
set (CMAKE_INCLUDE_CURRENT_DIR  ON)

function(target_link_libraries_system target)
    set(options PRIVATE PUBLIC INTERFACE)
    cmake_parse_arguments(TLLS "${options}" "" "" ${ARGN})
    foreach(op ${options})
        if(TLLS_${op})
            set(scope ${op})
        endif()
    endforeach(op)
    set(libs ${TLLS_UNPARSED_ARGUMENTS})

    foreach(lib ${libs})
        get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
        if(lib_include_dirs)
            if(scope)
                target_include_directories(${target} SYSTEM ${scope} ${lib_include_dirs})
            else()
                target_include_directories(${target} SYSTEM PRIVATE ${lib_include_dirs})
            endif()
        else()
            message("Warning: ${lib} doesn't set INTERFACE_INCLUDE_DIRECTORIES. No include_directories set.")
        endif()
        if(scope)
            target_link_libraries(${target} ${scope} ${lib})
        else()
            target_link_libraries(${target} ${lib})
        endif()
    endforeach()
endfunction(target_link_libraries_system)



add_library(server_utilities STATIC http_session.cpp shared_state.cpp websocket_session.cpp listener.cpp websocket.cpp websocket_impl.cpp)

target_include_directories(server_utilities PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/server_utilities>
    $<INSTALL_INTERFACE:include/server_utilities>

    SYSTEM ${Boost_INCLUDE_DIRS}
)

target_include_directories(server_utilities PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    SYSTEM ${Boost_INCLUDE_DIRS}
)


target_link_libraries(server_utilities PUBLIC project_options project_warnings )
target_include_directories(server_utilities SYSTEM PRIVATE ${Boost_INCLUDE_DIRS} )
target_link_libraries_system(server_utilities PUBLIC CONAN_PKG::spdlog CONAN_PKG::fmt Qt6::Core CONAN_PKG::nlohmann_json)


add_executable(chat_client_cli client.cpp )
target_include_directories(chat_client_cli SYSTEM PRIVATE ${Boost_INCLUDE_DIRS} )
target_link_libraries(chat_client_cli PUBLIC server_utilities pthread CONAN_PKG::spdlog CONAN_PKG::fmt CONAN_PKG::nlohmann_json)

