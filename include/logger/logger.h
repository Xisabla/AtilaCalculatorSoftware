/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      logger.h

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#pragma once
#ifndef ATILACALCULATORSOFTWARE_LOGGER_H
#define ATILACALCULATORSOFTWARE_LOGGER_H

#include "common.h"
#include "core/utilities.h"
#include "log_entries.h"

#include <ctime>
#include <map>
#include <regex>
#include <string>
#include <vector>

/**
 * @brief Imported class
 */
class LogEntries;

/**
 * @class LogMetaData
 * @brief Store information about the log entry (LogLevel, Timestamp, ...)
 */
class LogMetaData {
  public:
    /**
     * @param level Log entry logging level
     * @param timestamp Log entry timestamp
     */
    explicit LogMetaData(LogLevel level, time_t timestamp = time(nullptr));

    /**
     * @return The logging level of the log entry
     */
    LogLevel getLogLevel();

    /**
     * @return The timestamp of the log entry
     */
    time_t getTimestamp() const;

  private:
    /**
     * @brief Logging level of the log entry
     * @see LogLevel
     */
    LogLevel level;

    /**
     * @brief Timestamp of the log entry
     */
    time_t timestamp;
};

/**
 * @class Logger
 * @brief Logging interface that stores logs, allow logfile writing, also contains other output
 * methods
 */
class Logger {
  public:
    /**
     * @brief Copy constructor of Logger
     * @param logger Logger to copy
     * @note If the source logger is destroyed, the entries vector of the copy will reset as it is a referenced pointer
     */
    Logger(const Logger &logger);


    /**
     * @brief Logger destructor
     */
    ~Logger();

    /**
     * @brief Copy operator of Logger
     * @param logger Logger to copy
     * @note If the source logger is destroyed, the entries vector of the copy will reset as it is a referenced pointer
     */
    Logger operator=(const Logger &logger);

    /**
     * @brief Add a log entry in the current logger, automatically create log meta data
     * @param message Message to log
     * @param level Logging level (default = Debug)
     * @param timestamp Logging timestamp
     * @return The id of the current entry
     */
    size_t
    log(const std::string& message, LogLevel level = Debug, time_t timestamp = time(nullptr));

    /**
     * @brief Get the formatted string of a log entry
     * @param metaData Meta data of the log entry
     * @param message Message of the log entry
     * @param format The format to apply to the log entry
     * @return The string formatted log entry
     */
    static std::string format(LogMetaData metaData,
                              const std::string& message,
                              const std::string& format = Logger::getLoggingFormat());

    /**
     * @brief Retrieve the current instance of logger or create a new one
     * @return The Logger object instance reference
     */
    static Logger* getInstance();

    /**
     * @brief Get the log entries
     * @return A pointer to the log entries container
     */
    static LogEntries* getLogs();

    /**
     * @return The current logging formatting string
     * @see logFormat
     */
    static std::string getLoggingFormat();

    /**
     * @return The default logging formatting string
     * @see defaultLogFormat
     */
    static std::string getDefaultLoggingFormat();

    /**
     * @brief Set the logging formatting string of the logger
     * @param format Format of the log entries
     * @see logFormat
     */
    static void setLoggingFormat(std::string format);

    /**
     * @brief Log a TRACE entry from the given string into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t trace_s(std::string message);

    /**
     * @brief Concatenate and log the given elements as an TRACE entry
     * @tparam TArgs Types of the given elements
     * @param args Elements to log (string, int, float, ...), will be concatenate
     * @return The id of the create entry
     */
    template<typename... TArgs> static size_t trace(TArgs const&... args) {
        return Logger::trace_s(concat(args...));
    }

    /**
     * @brief Log a DEBUG entry from the given string into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t debug_s(std::string message);

    /**
     * @brief Concatenate and log the given elements as an DEBUG entry
     * @tparam TArgs Types of the given elements
     * @param args Elements to log (string, int, float, ...), will be concatenate
     * @return The id of the create entry
     */
    template<typename... TArgs> static size_t debug(TArgs const&... args) {
        return Logger::debug_s(concat(args...));
    }

    /**
     * @brief Log an INFO entry from the given string into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t info_s(std::string message);

    /**
     * @brief Concatenate and log the given elements as an INFO entry
     * @tparam TArgs Types of the given elements
     * @param args Elements to log (string, int, float, ...), will be concatenate
     * @return The id of the create entry
     */
    template<typename... TArgs> static size_t info(TArgs const&... args) {
        return Logger::info_s(concat(args...));
    }

    /**
     * @brief Log a WARN entry from the given string into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t warn_s(std::string message);

    /**
     * @brief Concatenate and log the given elements as an WARN entry
     * @tparam TArgs Types of the given elements
     * @param args Elements to log (string, int, float, ...), will be concatenate
     * @return The id of the create entry
     */
    template<typename... TArgs> static size_t warn(TArgs const&... args) {
        return Logger::warn_s(concat(args...));
    }

    /**
     * @brief Log an ERROR entry from the given string into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t error_s(std::string message);

    /**
     * @brief Concatenate and log the given elements as an ERROR entry
     * @tparam TArgs Types of the given elements
     * @param args Elements to log (string, int, float, ...), will be concatenate
     * @return The id of the create entry
     */
    template<typename... TArgs> static size_t error(TArgs const&... args) {
        return Logger::error_s(concat(args...));
    }

    /**
     * @brief Log a FATAL entry from the given string into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t fatal_s(std::string message);

    /**
     * @brief Concatenate and log the given elements as an FATAL entry
     * @tparam TArgs Types of the given elements
     * @param args Elements to log (string, int, float, ...), will be concatenate
     * @return The id of the create entry
     */
    template<typename... TArgs> static size_t fatal(TArgs const&... args) {
        return Logger::fatal_s(concat(args...));
    }

  private:
    /**
     * @brief Logger Singleton private constructor
     */
    Logger();

    /**
     * @brief Get the string format of a given logging level
     * @param level The logging level
     * @return The string formatted logging level
     */
    static std::string formatLogLevel(LogLevel level);

    /**
     * @brief Nonformatted raw log entries
     * @note As LogEntries and Logger as self including classes, we have to use a reference instead
     * of a classic use (as LogEntries is "incomplete")
     */
    LogEntries* entries;

    /**
     * @brief Time mode of the log entries (Local or UTC - absolute)
     * Default value is TimeLocal
     */
    static LogTimeMode timeMode;

    /**
     * @brief Current log formatting string, if empty will use defaultLogFormat
     *
     * Variables:
     *  - %dt%: Hardcoded timestamp (eg: "1548405000")
     *  - %dt:sec%: Timestamp second ([0;59])
     *  - %dt:min%: Timestamp minute ([0;59])
     *  - %dt:hour%: Timestamp hour ([0:23])
     *  - %dt:mday%: Timestamp day of the month ([1:31])
     *  - %dt:mon%: Timestamp month of the year ([1:12])
     *  - %dt:year%: Timestamp year (> 1900)
     *  - %dt%:wday%: Timestamp day of the week ([0;6])
     *  - %dt:yday%: Timestamp day of the year ([0;365])
     *  - %level%: Logging level (eg: "WARN")
     *  - %message%: Logging message (eg: "Loading /share/file.res resource file")
     *
     * All dt variables are related to the ctime library tm structure
     * @see https://www.cplusplus.com/reference/ctime/tm/
     *
     * Default value is an empty string
     *
     * @see defaultLogFormat
     */
    static std::string logFormat;

    /**
     * @brief Default log formatting string
     * @see logFormat
     */
    static std::string defaultLogFormat;

    /**
     * @brief Logger instance
     */
    static Logger* instance;
};

#endif // ATILACALCULATORSOFTWARE_LOGGER_H
