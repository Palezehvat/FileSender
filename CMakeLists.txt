cmake_minimum_required(VERSION 3.15)

project(FileSender VERSION 1.0.0)
set(CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(logs messengers protocol security transport)

find_package(spdlog REQUIRED)

add_library(logger 
    STATIC
    logs/logger.cpp
)

add_library(messengers
    STATIC
    messengers/text_messenger.cpp
)

add_library(protocol 
    STATIC
    protocol/packet_serializer.cpp
)

add_library(transport
    STATIC
    transport/tcp_transport.cpp
)



target_link_libraries(logger PRIVATE spdlog::spdlog)
target_link_libraries(transport PRIVATE logger)
target_link_libraries(protocol PRIVATE logger)
target_link_libraries(messengers PRIVATE logger protocol)

add_executable(app main.cpp)
target_link_libraries(app logger transport messengers)