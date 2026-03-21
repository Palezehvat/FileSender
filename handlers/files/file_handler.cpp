#include "file_handler.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

namespace Handlers {

constexpr uint32_t MAX_FILENAME = 255;

std::filesystem::path makeUniquePath(const std::filesystem::path& original) {
    if (!std::filesystem::exists(original)) {
        return original;
    }

    auto dir = original.parent_path();
    auto stem = original.stem().string();
    auto extension  = original.extension().string();

    int counter = 1;
    std::filesystem::path result;

    do {
        result = dir / (stem + " (" + std::to_string(counter) + ")" + extension);
        ++counter;
    } while (std::filesystem::exists(result));

    return result;
}

FileHandler::FileHandler(std::shared_ptr<spdlog::logger> logger,
                         ChooseFilePathFn choosePath)
                         : logger(logger), choosePath(choosePath), state(State::ReadyToWork){};

void FileHandler::handle(const Protocol::Packet& packet) {
    switch (packet.header.typePacket) {
        case Protocol::FileMeta:
            handleMetadata(packet);
            break;
        case Protocol::FileChunk:
            handleChunk(packet);
            break;
        case Protocol::FileEnd:
            handleEnd(packet);
            break;
        default:
            logger->error(
                "FileHandler received invalid packet type: {}",
                static_cast<uint8_t>(packet.header.typePacket)
            );
            throw std::runtime_error(
                fmt::format(
                    "FileHandler received invalid packet type: {}",
                    static_cast<uint8_t>(packet.header.typePacket)
                )
            );
            break;
    }
}

void FileHandler::handleMetadata(const Protocol::Packet& packet) {
    if (state != State::ReadyToWork) {
        throw std::runtime_error("File metadata received in invalid state");
    }
    logger->trace("Start get metadata");

    size_t offset = 0;

    if (packet.data.size() < sizeof(uint32_t)) {
        logger->error("Metadata packet too small (no name size)");
        throw std::runtime_error("Metadata packet too small (no name size)");
    }

    uint32_t sizeName;
    std::memcpy(&sizeName, packet.data.data() + offset, sizeof(uint32_t));
    sizeName = ntohl(sizeName);

    if (sizeName == 0 || sizeName > MAX_FILENAME) {
        logger->error("Invalid file name size");
        throw std::runtime_error("Invalid file name size");
    }

    offset += sizeof(uint32_t);
    std::vector<uint8_t> nameInBytes(
        packet.data.begin() + sizeof(uint32_t),
        packet.data.begin() + sizeof(uint32_t) + sizeName
    );

    if (packet.data.size() < offset + sizeName + sizeof(uint32_t)) {
        throw std::runtime_error("Metadata packet corrupted");
    }

    std::string fileName(nameInBytes.begin(), nameInBytes.end());
    offset += sizeName;

    uint32_t sizeFile;
    std::memcpy(&sizeFile, packet.data.data() + offset, sizeof(uint32_t));
    sizeFile = ntohl(sizeFile);

    fileName = std::filesystem::path(fileName).filename().string();
    
    currentPath = choosePath(fileName, sizeFile);
    if (currentPath.empty()) {
        logger->error("File receiving cancelled by user");
        throw std::runtime_error("File receiving cancelled by user");
    }
    currentPath = currentPath / fileName;
    currentPath = makeUniquePath(currentPath);
    std::filesystem::create_directories(currentPath.parent_path());

    out.open(currentPath, std::ios::binary);
    if (!out) {
        logger->error("Cannot open file for writing");
        throw std::runtime_error("Cannot open file for writing");
    }

    expectedSize = sizeFile;
    receivedSize = 0;
    state = State::ReceivingChunks;

    logger->info("Started receiving file: {}", currentPath.string());
}

void FileHandler::handleChunk(const Protocol::Packet& packet) {
    if (state != State::ReceivingChunks) {
        logger->error("FileChunk received in invalid state");
        throw std::runtime_error("FileChunk received in invalid state");
    }
    logger->trace("Start get chunk");

    if (packet.data.empty())
        logger->warn("Received empty file chunk");
    else
        logger->trace("Chunk file received");

    if (!packet.data.empty()) {
        out.write(
            reinterpret_cast<const char*>(packet.data.data()),
            packet.data.size()
        );
    }

    if (!out) {
        throw std::runtime_error("Failed writing to file");
    }

    receivedSize += static_cast<uint32_t>(packet.data.size());

    if (receivedSize > expectedSize) {
        throw std::runtime_error("Received more data than expected");
    }

    logger->trace("File progress: {}/{}", receivedSize, expectedSize);

    logger->trace("End get chunk");

}

void FileHandler::handleEnd(const Protocol::Packet& packet) {
    if (state != State::ReceivingChunks) {
        throw std::runtime_error("FileEnd received in invalid state");
    }

    if (receivedSize != expectedSize) {
        throw std::runtime_error("File transfer incomplete");
    }

    out.close();
    state = State::ReadyToWork;

    logger->info("File saved successfully: {}", currentPath.string());
}

void FileHandler::reset() {
    if (out.is_open())
        out.close();
    state = State::ReadyToWork;
}

}