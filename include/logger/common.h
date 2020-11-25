/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      common.h

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#pragma once
#ifndef ATILACALCULATORSOFTWARE_COMMON_H
#define ATILACALCULATORSOFTWARE_COMMON_H

#include <string>
#include <utility>

//  --------------------------------------------------------------------------------------
//  DEFINED LATER
//  --------------------------------------------------------------------------------------

class LogEntries;
class LogMetaData;
class Logger;

//  --------------------------------------------------------------------------------------
//  ENUMS
//  --------------------------------------------------------------------------------------

/**
 * @brief Logging time mode, TimeUTC means use abo
 */
enum LogTimeMode { TimeLocal, TimeUTC };

/**
 * @brief Logging level of a log entry, used to filter log entries, each filtering level will hide
 * lower level entries
 */
enum LogLevel { Trace = -1, Debug = 0, Info = 1, Warn = 2, Error = 3, Fatal = 4 };

//  --------------------------------------------------------------------------------------
//  STRUCTURES AND TYPES
//  --------------------------------------------------------------------------------------

/**
 * @brief Entry listener for Logger, will be called as callback for each new entry in the logger
 * with the given parameters
 * @param index The index of the new entry
 * @param entry A reference to the entry
 * @param formatted A reference to the formatted string message of the entry
 * @param entries The LogEntries object that contains all the entries
 */
typedef void (*entry_listener)(size_t index,
                               const std::pair<LogMetaData, std::string>& entry,
                               const std::string& formatted,
                               const LogEntries& entries);

#endif // ATILACALCULATORSOFTWARE_COMMON_H
