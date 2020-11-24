/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      logger.h

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#pragma once
#ifndef ATILACALCULATORSOFTWARE_LOGGER_H
#define ATILACALCULATORSOFTWARE_LOGGER_H

#include "core/utilities.h"

#include <ctime>
#include <map>
#include <string>
#include <regex>
#include <vector>

/**
 * @brief Logging time mode, TimeUTC means use abo
 */
enum LogTimeMode { TimeLocal, TimeUTC };

/**
 * @brief Logging level of a log entry, used to filter log entries, each filtering level will hide
 * lower level entries
 */
enum LogLevel { Trace = -1, Debug = 0, Info = 1, Warn = 2, Error = 3, Fatal = 4 };

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
    LogMetaData(LogLevel level, time_t timestamp = time(0));

    /**
     * @return The logging level of the log entry
     */
    LogLevel getLogLevel();

    /**
     * @return The timestamp of the log entry
     */
    time_t getTimestamp();

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
     * @brief Add a log entry in the current logger, automatically create log meta data
     * @param message Message to log
     * @param level Logging level (default = Debug)
     * @param timestamp Logging timestamp
     * @return The id of the current entry
     */
    size_t log(std::string message, LogLevel level = Debug, time_t timestamp = time(0));

    /**
     * @brief Retrieve the current instance of logger or create a new one
     * @return The Logger object instance reference
     */
    static Logger* getInstance();

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
     * @brief Log a TRACE entry into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t trace(std::string message);

    /**
     * @brief Log a DEBUG entry into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t debug(std::string message);

    /**
     * @brief Log an INFO entry into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t info(std::string message);

    /**
     * @brief Log a WARN entry into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t warn(std::string message);

    /**
     * @brief Log an ERROR entry into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t error(std::string message);

    /**
     * @brief Log a FATAL entry into the logger
     * @param message Message to log
     * @return The id of the current entry
     */
    static size_t fatal(std::string message);

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
    std::string formatLogLevel(LogLevel level);

    /**
     * @brief Get the formatted string of a log entry
     * @param metaData Meta data of the log entry
     * @param message Message of the log entry
     * @param format The format to apply to the log entry
     * @return The string formatted log entry
     */
    std::string format(LogMetaData metaData,
                       std::string message,
                       std::string format = Logger::getLoggingFormat());

    /**
     * @brief Nonformatted raw log entries
     */
    std::vector<std::pair<LogMetaData, std::string>> entries;

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
