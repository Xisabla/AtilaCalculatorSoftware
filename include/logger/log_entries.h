/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      log_entries.h

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#pragma once
#ifndef ATILACALCULATORSOFTWARE_LOG_ENTRIES_H
#define ATILACALCULATORSOFTWARE_LOG_ENTRIES_H

#include "common.h"
#include "logger.h"

#include <set>
#include <string>
#include <vector>

/**
 * @class LogEntries
 * @brief Contains the entries of the logger
 */
class LogEntries : private std::vector<std::pair<LogMetaData, std::string>> {
  public:
    using vector::emplace_back;
    using vector::push_back;
    using vector::operator[];
    using vector::begin;
    using vector::end;
    using vector::size;

    /**
     * @brief Filter logs to keep only entries above the given logging level (including itself)
     * @param lowest The lowest logging level to keep
     * @return A new LogEntries object will filtered entries
     * @note This object is not refreshed by any new entry in the main object
     */
    LogEntries filter(LogLevel lowest);

    /**
     * @brief Filter logs to keep only entries above the given lowest logging level and under the
     * given highest logging level (including themself)
     * @param lowest The lowest logging level to keep
     * @param highest The highest logging level to keep
     * @return A new LogEntries object will filtered entries
     * @note This object is not refreshed by any new entry in the main object
     */
    LogEntries filter(LogLevel lowest, LogLevel highest);

    /**
     * @brief Filter logs to keep only entries with the given logging levels
     * @param whitelist Set of logging level to keep
     * @return A new LogEntries object will filtered entries
     * @note This object is not refreshed by any new entry in the main object
     */
    LogEntries filter(std::set<LogLevel> whitelist);

    /**
     * @brief Filter logs to exclude the given logging levels from the entry
     * @param level Logging level to exclude
     * @return A new LogEntries object will filtered entries
     * @note This object is not refreshed by any new entry in the main object
     */
    LogEntries exclude(LogLevel level);

    /**
     * @brief Filter logs to exclude all logging levels between the given lowest and highest given
     * logging levels
     * @param lowest Lowest logging level to exclude
     * @param highest Highest logging level to exclude
     * @return A new LogEntries object will filtered entries
     * @note This object is not refreshed by any new entry in the main object
     */
    LogEntries exclude(LogLevel lowest, LogLevel highest);

    /**
     * @brief Filter logs to exclude all given logging levels
     * @param blacklist Logging levels to exclude
     * @return A new LogEntries object will filtered entries
     * @note This object is not refreshed by any new entry in the main object
     */
    LogEntries exclude(std::set<LogLevel> blacklist);

    /**
     * @brief Get log entries as formatted string
     * @return A vector of the formatted log entries as String
     */
    std::vector<std::string> formatted();
};

#endif // ATILACALCULATORSOFTWARE_LOG_ENTRIES_H
