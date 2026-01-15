/**
 * @file file_reader.h
 * @brief Implements file reading
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef FILE_READER_H
#define FILE_READER_H


#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>
#include "logger.h"
#include "file_sender.h"
#include "itransport.h"
#include "isecurity.h"

/**
 * @namespace Senders
 * 
 * @brief Contains class FileReader
 */
namespace Senders {

/**
 * @class FileReader
 * 
 * @brief Required for reading a file and then sending the file to the server
 */
class FileReader {
public:
    /**
     * @brief Construct a new File Reader object
     * 
     * @param logger logger for tracking program execution
     * @param transport data transfer method (implemented protocol)
     * @param security encryption method
     * @param chunkSize size of transferred data
     */
    explicit FileReader(std::shared_ptr<spdlog::logger> logger, Transport::ITransport& transport,
                        Security::ISecurity& security, const uint32_t& chunkSize);
    /**
     * @brief Reads a file and sends data to the server
     * 
     * @param file path to file
     */
    void readFile(const std::filesystem::path& file);
private:
    std::shared_ptr<spdlog::logger> logger;
    Senders::FileSender sender;
    uint32_t chunkSize;

    void sendMetaData(const std::filesystem::path& file);
    void sendChunks(const std::filesystem::path& file);
    void sendEnd();
};

} // namespace Messenger

#endif // FILE_READER_H