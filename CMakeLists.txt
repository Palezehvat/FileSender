cmake_minimum_required(VERSION 3.15)

project(FileSender VERSION 1.0.0)
set(CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(transport senders/files senders/text security protocol logs handlers handlers/files handlers/text core core/client core/server)

find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

include(FetchContent)

FetchContent_Declare(
    libsodium
    GIT_REPOSITORY https://github.com/jedisct1/libsodium.git
    GIT_TAG 1.0.21-RELEASE
)

FetchContent_MakeAvailable(libsodium)

enable_testing()

add_subdirectory(tests)

add_library(libsodium_security
    STATIC
    security/libsodium_security.cpp
)

add_library(key_exchange
    STATIC
    security/key_exchange.cpp
)

add_library(transport
    STATIC
    transport/tcp_transport.cpp
)

add_library(file_reader
    STATIC
    senders/files/file_reader.cpp
)

add_library(file_sender
    STATIC
    senders/files/file_sender.cpp
)

add_library(text_sender
    STATIC
    senders/text/text_sender.cpp
)

add_library(protocol 
    STATIC
    protocol/packet_serializer.cpp
)

add_library(logger 
    STATIC
    logs/logger.cpp
)

add_library(file_handler
    STATIC
    handlers/files/file_handler.cpp
)

add_library(packet_dispatcher
    STATIC
    core/packet_dispatcher.cpp
)

add_library(server
    STATIC
    core/server/server.cpp
)

add_library(client
    STATIC
    core/client/client.cpp
)

target_link_libraries(logger PRIVATE spdlog::spdlog)
target_link_libraries(transport PRIVATE logger)
target_link_libraries(protocol PRIVATE logger)
target_link_libraries(key_exchange PRIVATE logger sodium)
target_link_libraries(libsodium_security PRIVATE logger sodium key_exchange)
target_link_libraries(file_sender PRIVATE logger protocol)
target_link_libraries(file_reader PRIVATE logger file_sender)
target_link_libraries(text_sender PRIVATE logger protocol)
target_link_libraries(file_handler PRIVATE logger)
target_link_libraries(packet_dispatcher PRIVATE logger)
target_link_libraries(server PRIVATE logger packet_dispatcher protocol file_handler key_exchange libsodium_security)
target_link_libraries(client PRIVATE logger file_reader text_sender key_exchange sodium libsodium_security)

add_executable(app main.cpp)
target_link_libraries(app logger transport client server)