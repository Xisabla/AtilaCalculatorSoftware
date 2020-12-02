/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      logger.cpp

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#include "logger/logger.h"

//  --------------------------------------------------------------------------------------
//  LOG META DATA
//  --------------------------------------------------------------------------------------

LogMetaData::LogMetaData(LogLevel level, time_t timestamp): level(level), timestamp(timestamp) { }

//  --------------------------------------------------------------------------------------
//  LOG META DATA > GETTERS
//  --------------------------------------------------------------------------------------

LogLevel LogMetaData::getLogLevel() { return this->level; }

time_t LogMetaData::getTimestamp() const { return this->timestamp; }

//  --------------------------------------------------------------------------------------
//  LOGGER
//  --------------------------------------------------------------------------------------

Logger::Logger()
: entries(new LogEntries()), verbose(false), verbosityLevels({ Info, Warn, Error, Fatal }),
  fileLogging(false), loggingFile() { }

Logger::Logger(const Logger& logger) {
    // Move entries pointer
    this->entries = logger.entries;
    this->verbose = logger.verbose;
    this->verbosityLevels = logger.verbosityLevels;
    this->loggingFile.copyfmt(logger.loggingFile);
    this->fileLogging = logger.fileLogging;
}

Logger::~Logger() {
    // On destruction, clear entries
    delete this->entries;

    // Close opened logging file
    if (this->loggingFile.is_open()) this->loggingFile.close();

    // Reset instance pointer
    Logger::instance = nullptr;
}

LogTimeMode Logger::timeMode = TimeLocal;

std::string Logger::logFormat;

std::string Logger::defaultLogFormat =
"[%dt:year%-%dt:mon%-%dt:mday%|%dt:hour%:%dt:min%:%dt:sec%][%level%]%message%";

Logger* Logger::instance = nullptr;

//  --------------------------------------------------------------------------------------
//  LOGGER > OPERATORS
//  --------------------------------------------------------------------------------------

Logger& Logger::operator=(const Logger& logger) {
    if (&logger != this) {
        // Move entries pointer
        this->entries = logger.entries;
        this->verbose = logger.verbose;
        this->verbosityLevels = logger.verbosityLevels;
        this->loggingFile.copyfmt(logger.loggingFile);
        this->fileLogging = logger.fileLogging;
    }

    return *this;
}

//  --------------------------------------------------------------------------------------
//  LOGGER > GETTERS
//  --------------------------------------------------------------------------------------

Logger* Logger::getInstance() {
    if (Logger::instance == nullptr) Logger::instance = new Logger();

    return Logger::instance;
}

LogEntries* Logger::getLogs() { return Logger::getInstance()->entries; }

std::string Logger::getLoggingFormat() {
    return Logger::logFormat.empty() ? Logger::defaultLogFormat : Logger::logFormat;
}

std::string Logger::getDefaultLoggingFormat() { return Logger::defaultLogFormat; }

//  --------------------------------------------------------------------------------------
//  LOGGER > SETTERS
//  --------------------------------------------------------------------------------------

void Logger::setVerbosity(bool verbosity) { Logger::getInstance()->verbose = verbosity; }

void Logger::setVerboseLevel(LogLevel level) { Logger::setVerboseLevelRange(level, Fatal); }

void Logger::setVerboseLevelRange(LogLevel lowest, LogLevel highest) {
    Logger::getInstance()->verbosityLevels.clear();

    if (Trace >= lowest && Trace <= highest) Logger::getInstance()->verbosityLevels.insert(Trace);
    if (Debug >= lowest && Debug <= highest) Logger::getInstance()->verbosityLevels.insert(Debug);
    if (Info >= lowest && Info <= highest) Logger::getInstance()->verbosityLevels.insert(Info);
    if (Warn >= lowest && Warn <= highest) Logger::getInstance()->verbosityLevels.insert(Warn);
    if (Error >= lowest && Error <= highest) Logger::getInstance()->verbosityLevels.insert(Error);
    if (Fatal >= lowest && Fatal <= highest) Logger::getInstance()->verbosityLevels.insert(Fatal);
}

void Logger::setVerboseLevels(std::set<LogLevel> levels) {
    Logger::getInstance()->verbosityLevels = std::move(levels);
}

void Logger::logToFile(const std::string& filename, bool legacy) {
    try {
        Logger::getInstance()->loggingFile.open(filename);
        Logger::getInstance()->fileLogging = true;

        if (legacy) {
            for (auto& entry: *Logger::getInstance()->entries) {
                Logger::getInstance()->loggingFile << format(entry.first, entry.second)
                                                   << std::endl;
            }
        }
    } catch (std::exception& e) {
        Logger::error("Unable to write logs to file \"", filename, "\"");
    }
}

void Logger::setFileLogging(bool value) { Logger::getInstance()->fileLogging = value; }

void Logger::setLoggingFormat(std::string format) { Logger::logFormat = std::move(format); }

//  --------------------------------------------------------------------------------------
//  LOGGER > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

size_t Logger::log(const std::string& message, LogLevel level, time_t timestamp) {
    LogMetaData metaData(level, timestamp);
    std::pair<LogMetaData, std::string> entry = std::make_pair(metaData, message);
    std::string formatted = Logger::format(metaData, message);

    this->entries->push_back(entry);

    size_t index = this->entries->size() - 1;

    if (this->verbose && this->verbosityLevels.count(level) > 0) std::cout << formatted << std::endl;

    if (this->fileLogging && this->loggingFile.is_open())
        this->loggingFile << formatted << std::endl;

    for (auto& listener: this->listeners) {
        listener(index, entry, formatted, *(this->entries));
    }

    return index;
}

std::string Logger::getEncodedLogLevel(LogLevel level) {
    // TODO: Make this method a formatting method for the message, used by the global "format"
    // method
    if (level == Trace) return "TRACE";
    if (level == Debug) return "DEBUG";
    if (level == Info) return "INFO";
    if (level == Warn) return "WARN";
    if (level == Error) return "ERROR";
    if (level == Fatal) return "FATAL";

    return "DEBUG";
}

std::string Logger::formatInt(const int& value, int size) {
    std::stringstream ss;

    ss << std::setw(size) << std::setfill('0') << value;

    return ss.str();
}

std::string Logger::formatEncodedLogLevel(const std::string& str, LogLevel level) {
    return std::regex_replace(str, std::regex("%level%"), getEncodedLogLevel(level));
}

std::string Logger::formatTime(std::string str, time_t timestamp, tm* t) {
    str = std::regex_replace(str, std::regex("%dt%"), std::to_string(timestamp));
    str = std::regex_replace(str, std::regex("%dt:yday%"), formatInt(t->tm_yday, 3));
    str = std::regex_replace(str, std::regex("%dt:wday%"), formatInt(t->tm_wday, 3));
    str = std::regex_replace(str, std::regex("%dt:year%"), formatInt(t->tm_year + 1900, 4));
    str = std::regex_replace(str, std::regex("%dt:mon%"), formatInt((t->tm_mon + 1), 2));
    str = std::regex_replace(str, std::regex("%dt:mday%"), formatInt(t->tm_mday, 2));
    str = std::regex_replace(str, std::regex("%dt:hour%"), formatInt(t->tm_hour, 2));
    str = std::regex_replace(str, std::regex("%dt:min%"), formatInt(t->tm_min, 2));
    str = std::regex_replace(str, std::regex("%dt:sec%"), formatInt(std::min(t->tm_sec, 59), 2));

    return str;
}

std::string
Logger::format(LogMetaData metaData, const std::string& message, const std::string& format) {
    time_t timestamp = metaData.getTimestamp();
    LogLevel level = metaData.getLogLevel();
    tm* t = Logger::timeMode == TimeUTC ? gmtime(&timestamp) : localtime(&timestamp);

    // TODO: Add colorizing if a flag is set
    std::string formatted = std::regex_replace(format, std::regex("%message%"), message);
    formatted = Logger::formatEncodedLogLevel(formatted, level);
    formatted = Logger::formatTime(formatted, timestamp, t);

    return formatted;
}

size_t Logger::trace_s(const std::string& message) {
    return Logger::getInstance()->log(message, Trace);
}

size_t Logger::debug_s(const std::string& message) {
    return Logger::getInstance()->log(message, Debug);
}

size_t Logger::info_s(const std::string& message) {
    return Logger::getInstance()->log(message, Info);
}

size_t Logger::warn_s(const std::string& message) {
    return Logger::getInstance()->log(message, Warn);
}

size_t Logger::error_s(const std::string& message) {
    return Logger::getInstance()->log(message, Error);
}

size_t Logger::fatal_s(const std::string& message) {
    return Logger::getInstance()->log(message, Fatal);
}

size_t Logger::addEntryListener(entry_listener listener) {
    Logger::getInstance()->listeners.push_back(listener);

    return Logger::getInstance()->listeners.size();
}
