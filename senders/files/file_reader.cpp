#include "file_reader.h"


namespace Senders {

FileReader::FileReader(std::shared_ptr<spdlog::logger> logger, Transport::ITransport& transport,
                       Security::ISecurity& security, const uint32_t& chunkSize)
                       : logger(logger), sender(logger, transport, security),
                         chunkSize(chunkSize) {};

void FileReader::sendMetaData(const std::filesystem::path& file) {
    logger->trace("Send metadata for file: {}", file.string());
    std::string name = file.filename().string();
    auto size = std::filesystem::file_size(file);
    if (size > std::numeric_limits<uint32_t>::max()) {
        logger->critical(
            "File: {} to large. Files larger than 4 GB are not supported",
            file.string()
        );
        throw std::runtime_error(
            fmt::format(
                "File: {} to large. Files larger than 4 GB are not supported",
                file.string()
            )
        );
    }
    sender.SendMetadata(name, size);
    logger->trace("Metadata file sent: {}", file.string());
}

void FileReader::sendEnd() {
    sender.SendEnd();
}

void FileReader::sendChunks(const std::filesystem::path& file) {
    logger->trace("Start sending chunks for file: {}", file.string());
    std::ifstream in(file, std::ios::binary);
    if (!in) {
        logger->error("Cannot open file: {}", file.string());
        throw std::runtime_error(fmt::format("Cannot open file: {}", file.string()));
    }

    std::vector<uint8_t> buffer(chunkSize);
    while(in) {
        in.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        std::streamsize readBytes = in.gcount();
        if (readBytes > 0) {
            sender.SendChunk(
                std::vector<uint8_t>(buffer.begin(), buffer.begin() + readBytes)
            );
        }
    }
    logger->trace("Ended sending chunks for file: {}", file.string());
}

void FileReader::readFile(const std::filesystem::path& file) {
    logger->info("Work with file: {}", file.string());
    if (!std::filesystem::exists(file)) {
        logger->error("File: {} not found!", file.string());
        throw std::runtime_error(fmt::format("File: {} not found!", file.string()));
    }
    sendMetaData(file);
    sendChunks(file);
    sendEnd();
    logger->info("File: {} was sent successfully", file.string());
}

} // namespace Messenger