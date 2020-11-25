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
#include <fstream>
#include <iomanip>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

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
     * @note If the source logger is destroyed, the entries vector of the copy will reset as it is a
     * referenced pointer
     */
    Logger(const Logger& logger);


    /**
     * @brief Logger destructor
     */
    ~Logger();

    /**
     * @brief Copy operator of Logger
     * @param logger Logger to copy
     * @note If the source logger is destroyed, the entries vector of the copy will reset as it is a
     * referenced pointer
     */
    Logger& operator=(const Logger& logger);

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
     * @brief Get the string format of a given logging level
     * @param level The logging level
     * @return The string formatted logging level
     */
    static std::string getEncodedLogLevel(LogLevel level);

    /**
     * @brief Format integer value to string fitting a given size
     * @param value Integer value to format
     * @param size Size of the string
     * @return The formatted string
     */
    static std::string formatInt(const int& value, int size);

    /**
     * @brief Replace the %level% variable by the given logging level
     * @param str The base string
     * @param level The logging level
     * @return The string with the variable replaced
     */
    static std::string formatEncodedLogLevel(const std::string& str, LogLevel level);

    /**
     * @briefReplace the %dt:x% variables by the given time values
     * @param str The base string
     * @param t The time values
     * @return The string with the variable replaced
     */
    static std::string formatTime(std::string str, time_t timestamp, tm* t);

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
     * @brief Enable or disable Logger verbosity
     */
    static void setVerbosity(bool verbosity);

    /**
     * @brief Set the verbosity level, will show all logs above the given level (including itself)
     * @param level Lower level of verbosity
     */
    static void setVerboseLevel(LogLevel level);

    /**
     * @brief Set the verbosity level range, won't show any log out of this range
     * @param lowest Lowest verbosity level of the range
     * @param highest Highest verbosity level of the range
     */
    static void setVerboseLevelRange(LogLevel lowest, LogLevel highest);

    /**
     * @brief Set the verbose levels manually, will only show elements that have the LogLevel in the
     * set
     * @param levels Set of allowed logging levels in verbose
     */
    static void setVerboseLevels(std::set<LogLevel> levels);

    /**
     * @brief Writes incoming logs to the given file
     * @param filename Path to the log file
     * @param legacy If set on true, will write all previous logs
     */
    static void logToFile(const std::string& filename, bool legacy = true);

    /**
     * @brief If set on true will enable file logging
     */
    static void setFileLogging(bool value);

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
    static size_t trace_s(const std::string& message);

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
    static size_t debug_s(const std::string& message);

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
    static size_t info_s(const std::string& message);

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
    static size_t warn_s(const std::string& message);

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
    static size_t error_s(const std::string& message);

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
    static size_t fatal_s(const std::string& message);

    /**
     * @brief Concatenate and log the given elements as an FATAL entry
     * @tparam TArgs Types of the given elements
     * @param args Elements to log (string, int, float, ...), will be concatenate
     * @return The id of the create entry
     */
    template<typename... TArgs> static size_t fatal(TArgs const&... args) {
        return Logger::fatal_s(concat(args...));
    }

    /**
     * @brief Add an entry listener in the entry listeners vector
     * @param listener Listener that will trigger on any new entry
     * @return The id of the listener
     */
    static size_t addEntryListener(entry_listener listener);

  private:
    /**
     * @brief Logger Singleton private constructor
     */
    Logger();

    /**
     * @brief Nonformatted raw log entries
     * @note As LogEntries and Logger as self including classes, we have to use a reference instead
     * of a classic use (as LogEntries is "incomplete")
     */
    LogEntries* entries;

    /**
     * @brief If set on true, will show log messages in console
     */
    bool verbose;

    /**
     * @brief Allowed logging levels on verbose (in console)
     */
    std::set<LogLevel> verbosityLevels;

    /**
     * @brief If set on true, will write log messages into file
     */
    bool fileLogging;

    /**
     * @brief Output file stream for file writing log messages
     */
    std::ofstream loggingFile;

    /**
     * @brief List of listeners that will trigger on new entry
     */
    std::vector<entry_listener> listeners;

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
