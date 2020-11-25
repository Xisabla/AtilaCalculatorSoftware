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

Logger::Logger(): entries(new LogEntries()) {};

Logger::~Logger() { delete this->entries; }

LogTimeMode Logger::timeMode = TimeLocal;

std::string Logger::logFormat;

std::string Logger::defaultLogFormat =
"[%dt:year%-%dt:mon%-%dt:mday%|%dt:hour%:%dt:min%:%dt:sec%][%level%] %message%";

Logger* Logger::instance = nullptr;

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

void Logger::setLoggingFormat(std::string format) { Logger::logFormat = std::move(format); }

//  --------------------------------------------------------------------------------------
//  LOGGER > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

size_t Logger::log(const std::string& message, LogLevel level, time_t timestamp) {
    LogMetaData metaData(level, timestamp);

    this->entries->emplace_back(metaData, message);

    // TODO: Remove this or use a flag to turn on/off logging in console (depending on the loglevel)
    std::cout << this->format(metaData, message) << std::endl;

    return this->entries->size() - 1;
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