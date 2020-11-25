/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      logger.cpp

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#include "logger/logger.h"

#include <utility>

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

Logger::Logger(): entries(new LogEntries()), verbose(false), verbosityLevels({ Info, Warn, Error, Fatal }), fileLogging(false), loggingFile() {};

Logger::Logger(const Logger& logger) {
    // Move entries pointer
    this->entries = std::move(logger.entries);
    this->verbose = logger.verbose;
    this->verbosityLevels = logger.verbosityLevels;
    this->loggingFile.copyfmt(logger.loggingFile);
    this->fileLogging = logger.fileLogging;

    // Overwrite static parameters
    Logger::timeMode = logger.timeMode;
    Logger::logFormat = logger.logFormat;
}

Logger::~Logger() {
    // On destruction, clear entries
    delete this->entries;

    // Close opened logging file
    if(this->loggingFile.is_open())
        this->loggingFile.close();

    // Reset instance pointer
    this->instance = nullptr;
}

LogTimeMode Logger::timeMode = TimeLocal;

std::string Logger::logFormat;

std::string Logger::defaultLogFormat =
"[%dt:year%-%dt:mon%-%dt:mday%|%dt:hour%:%dt:min%:%dt:sec%][%level%] %message%";

Logger* Logger::instance = nullptr;

//  --------------------------------------------------------------------------------------
//  LOGGER > OPERATORS
//  --------------------------------------------------------------------------------------

Logger Logger::operator=(const Logger& logger) {
    // Move entries pointer
    this->entries = std::move(logger.entries);
    this->verbose = logger.verbose;
    this->verbosityLevels = logger.verbosityLevels;
    this->loggingFile.copyfmt(logger.loggingFile);
    this->fileLogging = logger.fileLogging;

    // Overwrite static parameters
    Logger::timeMode = logger.timeMode;
    Logger::logFormat = logger.logFormat;

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

void Logger::setVerboseLevel(LogLevel level) {
    Logger::getInstance()->verbosityLevels = { level };
}

void Logger::setVerboseLevelRange(LogLevel lowest, LogLevel highest) {
    Logger::getInstance()->verbosityLevels.clear();

    if(Trace >= lowest && Trace <= highest) Logger::getInstance()->verbosityLevels.insert(Trace);
    if(Debug >= lowest && Debug <= highest) Logger::getInstance()->verbosityLevels.insert(Debug);
    if(Info >= lowest && Info <= highest) Logger::getInstance()->verbosityLevels.insert(Info);
    if(Warn >= lowest && Warn <= highest) Logger::getInstance()->verbosityLevels.insert(Warn);
    if(Error >= lowest && Error <= highest) Logger::getInstance()->verbosityLevels.insert(Error);
    if(Fatal >= lowest && Fatal <= highest) Logger::getInstance()->verbosityLevels.insert(Fatal);
}

void Logger::setVerboseLevels(std::set<LogLevel> levels) { Logger::getInstance()->verbosityLevels = levels; }

void Logger::logToFile(std::string filename, bool legacy) {
    try {
        Logger::getInstance()->loggingFile.open(filename);
        Logger::getInstance()->fileLogging = true;

        if(legacy) {
            for(auto &entry : *Logger::getInstance()->entries) {
                Logger::getInstance()->loggingFile << Logger::getInstance()->format(entry.first, entry.second) << std::endl;
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

    this->entries->emplace_back(metaData, message);

    if(this->verbose && this->verbosityLevels.contains(level))
        std::cout << this->format(metaData, message) << std::endl;

    if(this->fileLogging && this->loggingFile.is_open())
        this->loggingFile << this->format(metaData, message) << std::endl;

    return this->entries->size() - 1;
}

std::string
Logger::format(LogMetaData metaData, const std::string& message, const std::string& format) {
    time_t timestamp = metaData.getTimestamp();
    LogLevel level = metaData.getLogLevel();

    // TODO: Split into "formatTime", "formatLogLevel", "formatMessage"
    // TODO: Add colorizing if a flag is set
    tm* t = Logger::timeMode == TimeUTC ? gmtime(&timestamp) : localtime(&timestamp);

    std::string formatted = std::regex_replace(format, std::regex("%message%"), message);
    formatted = std::regex_replace(formatted, std::regex("%level%"), formatLogLevel(level));
    formatted = std::regex_replace(formatted, std::regex("%dt%"), std::to_string(timestamp));
    formatted = std::regex_replace(formatted, std::regex("%dt:yday%"), std::to_string(t->tm_yday));
    formatted = std::regex_replace(formatted, std::regex("%dt:wday%"), std::to_string(t->tm_wday));
    formatted = std::regex_replace(formatted, std::regex("%dt:year%"), std::to_string(t->tm_year));
    formatted =
    std::regex_replace(formatted, std::regex("%dt:mon%"), std::to_string(t->tm_mon + 1));
    formatted = std::regex_replace(formatted, std::regex("%dt:mday%"), std::to_string(t->tm_mday));
    formatted = std::regex_replace(formatted, std::regex("%dt:hour%"), std::to_string(t->tm_hour));
    formatted = std::regex_replace(formatted, std::regex("%dt:min%"), std::to_string(t->tm_min));
    formatted =
    std::regex_replace(formatted, std::regex("%dt:sec%"), std::to_string(std::min(t->tm_sec, 59)));

    return formatted;
}

size_t Logger::trace_s(std::string message) {
    return Logger::getInstance()->log(std::move(message), Trace);
}

size_t Logger::debug_s(std::string message) {
    return Logger::getInstance()->log(std::move(message), Debug);
}

size_t Logger::info_s(std::string message) {
    return Logger::getInstance()->log(std::move(message), Info);
}

size_t Logger::warn_s(std::string message) {
    return Logger::getInstance()->log(std::move(message), Warn);
}

size_t Logger::error_s(std::string message) {
    return Logger::getInstance()->log(std::move(message), Error);
}

size_t Logger::fatal_s(std::string message) {
    return Logger::getInstance()->log(std::move(message), Fatal);
}

//  --------------------------------------------------------------------------------------
//  LOGGER > PRIVATE METHODS
//  --------------------------------------------------------------------------------------

std::string Logger::formatLogLevel(LogLevel level) {
    // TODO: Change this method name like "getEncodedLogLevel" (part of LogMetaData ?)
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
