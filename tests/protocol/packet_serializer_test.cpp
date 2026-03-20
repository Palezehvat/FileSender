#include <gtest/gtest.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/null_sink.h>
#include "packet_serializer.h"
#include "packet.h"
#include "logger.h"

TEST(packet_serializer, isCommonMessageSerializedAndDeserializedCorrectlyWithTextMessage) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_1");
    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::TextMessage,
            Protocol::TypeEncryption::NoEncryption,
            5
        ), 
        {'h', 'e', 'l', 'l', 'o'},
        logger
    );
    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);
    auto deserialized = Protocol::PacketSerializer::deserialize(serialized, logger);

    EXPECT_EQ(deserialized.header.typePacket, deserialized.header.typePacket);
    EXPECT_EQ(deserialized.header.typeEncryption, deserialized.header.typeEncryption);
    EXPECT_EQ(deserialized.header.sizePacket, deserialized.header.sizePacket);
    EXPECT_EQ(deserialized.data, deserialized.data);
}

TEST(packet_serializer, isCommonMessageSerializedAndDeserializedCorrectlyWithFileMeta) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_2");
    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::FileMeta,
            Protocol::TypeEncryption::NoEncryption,
            5
        ), 
        {'h', 'e', 'l', 'l', 'o'},
        logger
    );
    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);
    auto deserialized = Protocol::PacketSerializer::deserialize(serialized, logger);

    EXPECT_EQ(deserialized.header.typePacket, deserialized.header.typePacket);
    EXPECT_EQ(deserialized.header.typeEncryption, deserialized.header.typeEncryption);
    EXPECT_EQ(deserialized.header.sizePacket, deserialized.header.sizePacket);
    EXPECT_EQ(deserialized.data, deserialized.data);
}

TEST(packet_serializer, isCommonMessageSerializedAndDeserializedCorrectlyWithFileChunk) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_3");
    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::FileChunk,
            Protocol::TypeEncryption::NoEncryption,
            5
        ), 
        {'h', 'e', 'l', 'l', 'o'},
        logger
    );
    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);
    auto deserialized = Protocol::PacketSerializer::deserialize(serialized, logger);

    EXPECT_EQ(deserialized.header.typePacket, deserialized.header.typePacket);
    EXPECT_EQ(deserialized.header.typeEncryption, deserialized.header.typeEncryption);
    EXPECT_EQ(deserialized.header.sizePacket, deserialized.header.sizePacket);
    EXPECT_EQ(deserialized.data, deserialized.data);
}

TEST(packet_serializer, isCommonMessageSerializedAndDeserializedCorrectlyWithFileEnd) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_4");
    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::FileEnd,
            Protocol::TypeEncryption::NoEncryption,
            5
        ), 
        {'h', 'e', 'l', 'l', 'o'},
        logger
    );
    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);
    auto deserialized = Protocol::PacketSerializer::deserialize(serialized, logger);

    EXPECT_EQ(deserialized.header.typePacket, deserialized.header.typePacket);
    EXPECT_EQ(deserialized.header.typeEncryption, deserialized.header.typeEncryption);
    EXPECT_EQ(deserialized.header.sizePacket, deserialized.header.sizePacket);
    EXPECT_EQ(deserialized.data, deserialized.data);
}