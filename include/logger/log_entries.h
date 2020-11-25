/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      log_entries.h

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#pragma once
#ifndef ATILACALCULATORSOFTWARE_LOG_ENTRIES_H
#define ATILACALCULATORSOFTWARE_LOG_ENTRIES_H

#include "logger.h"
#include <string>
#include <vector>
#include <set>

/**
 * @brief Imported class
 */
class Logger;

/**
 * @brief Imported class
 */
class LogMetaData;

/**
 * @class LogEntries
 * @brief Contains the entries of the logger
 */
class LogEntries : private std::vector<std::pair<LogMetaData, std::string>> {
  public:
    using vector::push_back;
    using vector::emplace_back;
    using vector::size;
    using vector::operator[];
    using vector::begin;
    using vector::end;

    /*
    LogEntries* filter(LogLevel lowest);
    LogEntries* filter(LogLevel lowest, LogLevel highest);
    LogEntries* filter(std::set<LogLevel> whitelist);

    LogEntries* exclude(LogLevel levels...);
    // ? LogEntries* exclude(LogLevel level);
    LogEntries* exclude(LogLevel lowest, LogLevel highest);
    LogEntries* exclude(std::set<LogLevel> blacklist);

    std::vector<std::string> formatted();
     */

};

#endif // ATILACALCULATORSOFTWARE_LOG_ENTRIES_H
