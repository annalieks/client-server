cmake_minimum_required(VERSION 3.15)
project(client)

set(CMAKE_CXX_STANDARD 20)

SET (BOOST_ROOT "/usr/include/boost_1_73_0")
SET (Boost_INCLUDE_DIR "/usr/include/boost_1_73_0")
SET (BOOST_LIBRARY_DIR "/usr/include/boost_1_73_0")

find_package(Boost 1.73.0 REQUIRED filesystem)
if(Boost_FOUND)
    list(APPEND CLIENT_SOURCES
            client/src/sources/client.cpp
            client/src/headers/connection.hpp
            client/src/sources/main.cpp
            client/src/sources/connection.cpp
            http_parser/src/sources/request.cpp
            http_parser/src/sources/response.cpp
            http_parser/src/sources/message.cpp)

    list(APPEND SERVER_SOURCES
            server/src/sources/server.cpp
            server/src/sources/connection.cpp
            server/src/sources/server_main.cpp
            server/src/sources/data_file.cpp
            server/src/sources/request_manager.cpp
            http_parser/src/sources/request.cpp
            http_parser/src/sources/response.cpp
            http_parser/src/sources/message.cpp)

    add_executable(client ${CLIENT_SOURCES})
    add_executable(server ${SERVER_SOURCES})
    target_include_directories(client PUBLIC
            ${Boost_INCLUDE_DIRS}
            single_include
            client/include
            http_parser/include
            client/src/headers)
    target_include_directories(server PUBLIC
            ${Boost_INCLUDE_DIRS}
            single_include
            server/include
            http_parser/include
            server/src/headers)
    target_link_libraries(client ${Boost_LIBRARIES} pthread)
    target_link_libraries(server ${Boost_LIBRARIES} pthread)
endif()