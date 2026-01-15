/**
 * @file logger.h
 * @brief Contains the implementation of project logging
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>

/**
 * @namespace Logs
 * 
 * @brief Contains Logger
 */
namespace Logs {

/**
 * @class Logger
 * @brief Need for logging
 * 
 * Includes:
 * - logging for both server and client
 * - logging of sending, response, and errors for the client
 * - logging of all key actions for the server
 * - logging of various levels:
 * - trace
 * - debug
 * - info
 * - warn
 * - critical
 * - error
 */
class Logger {

public:
    /**
     * @brief Initializing the logger at program startup
     * @param nameLogger - internal name of the logger
     * @param logFilePath - path to the file where the logger will be located
     * @param isNeedCleanLogFile - Specifies whether to clear the logs of the
     * previous session (default, disabled)
     */
    static void init(const std::string& nameLogger, const std::string& logFilePath,
                     const bool& isNeedCleanLogFile = false);
    /**
     * @brief Get the Logger object
     * @return std::shared_ptr<spdlog::logger> 
     */
    static std::shared_ptr<spdlog::logger> getLogger();

private:
    static std::shared_ptr<spdlog::logger> logger;

};

} // namespace nLogger 

#endif // LOGGER_H