/**
 * @file file_handler.h
 * @brief Implements the packet handler logic
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H


#include <memory>
#include <filesystem>
#include "ihandler.h"
#include "logger.h"

/**
 * @namespace Handlers
 * 
 * @brief Contains class FileHandler
 */
namespace Handlers {

/**
 * @brief Function for getting the path where the file will be saved
 */
using ChooseFilePathFn = std::function<
    std::filesystem::path(const std::string& fileName, uint32_t fileSize)
>;

/**
 * @class FileHandler
 * 
 * @brief Implements processing of file packages
 */
class FileHandler : public IPacketHandler {
public:
    /**
     * @brief Construct a new File Handler object
     * 
     * @param logger logger for tracking program execution
     * @param choosePath specific function for getting the path to save a file
     */
    explicit FileHandler(std::shared_ptr<spdlog::logger> logger,
                         ChooseFilePathFn choosePath);
    /**
     * @brief The main function for processing file packages
     * 
     * @param packet processed package
     */
    void handle(const Protocol::Packet& packet) override;

private:
    enum class State {
        ReadyToWork,
        ReceivingChunks
    };
    std::shared_ptr<spdlog::logger> logger;
    State state;

    std::ofstream out;
    ChooseFilePathFn choosePath;
    std::filesystem::path currentPath;

    uint32_t expectedSize = 0;
    uint32_t receivedSize = 0;

    void handleMetadata(const Protocol::Packet& packet);
    void handleChunk(const Protocol::Packet& packet);
    void handleEnd(const Protocol::Packet& packet);
    void reset();

};

} // namespace Handlers

#endif // FILE_HANDLER_H