/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      log_entries.cpp

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#include "include/logger/log_entries.h"

//  --------------------------------------------------------------------------------------
//  LOG ENTRIES
//  --------------------------------------------------------------------------------------

//  --------------------------------------------------------------------------------------
//  LOG ENTRIES > GETTERS
//  --------------------------------------------------------------------------------------

//  --------------------------------------------------------------------------------------
//  LOG ENTRIES > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

LogEntries LogEntries::filter(LogLevel lowest) {
    LogEntries filtered;

    for (auto& entry: *this) {
        if (entry.first.getLogLevel() >= lowest) filtered.push_back(entry);
    }

    return filtered;
}

LogEntries LogEntries::filter(LogLevel lowest, LogLevel highest) {
    LogEntries filtered;

    for (auto& entry: *this) {
        if (entry.first.getLogLevel() >= lowest && entry.first.getLogLevel() <= highest)
            filtered.push_back(entry);
    }

    return filtered;
}

LogEntries LogEntries::filter(std::set<LogLevel> whitelist) {
    LogEntries filtered;

    for (auto& entry: *this) {
        if (whitelist.contains(entry.first.getLogLevel())) filtered.push_back(entry);
    }

    return filtered;
}

LogEntries LogEntries::exclude(LogLevel level) {
    LogEntries filtered;

    for (auto& entry: *this) {
        if (entry.first.getLogLevel() != level) filtered.push_back(entry);
    }

    return filtered;
}

LogEntries LogEntries::exclude(LogLevel lowest, LogLevel highest) {
    LogEntries filtered;

    for (auto& entry: *this) {
        if (entry.first.getLogLevel() < lowest || entry.first.getLogLevel() > highest)
            filtered.push_back(entry);
    }

    return filtered;
}

LogEntries LogEntries::exclude(std::set<LogLevel> blacklist) {
    LogEntries filtered;

    for (auto& entry: *this) {
        if (!blacklist.contains(entry.first.getLogLevel())) filtered.push_back(entry);
    }

    return filtered;
}

std::vector<std::string> LogEntries::formatted() {
    std::vector<std::string> formatted;

    std::transform(this->begin(),
                   this->end(),
                   std::back_inserter(formatted),
                   [](const std::pair<LogMetaData, std::string>& entry) {
                       return Logger::format(entry.first, entry.second);
                   });

    return formatted;
}

//  --------------------------------------------------------------------------------------
//  LOG ENTRIES > PRIVATE METHODS
//  --------------------------------------------------------------------------------------