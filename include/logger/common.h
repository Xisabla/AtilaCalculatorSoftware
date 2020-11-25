/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      common.h

  Copyright (c) 2020 - All rights reserved
  Distributed under the MIT License (https://opensource.org/licenses/MIT)

=========================================================================*/
#pragma once
#ifndef ATILACALCULATORSOFTWARE_COMMON_H
#define ATILACALCULATORSOFTWARE_COMMON_H

/**
 * @brief Logging time mode, TimeUTC means use abo
 */
enum LogTimeMode { TimeLocal, TimeUTC };

/**
 * @brief Logging level of a log entry, used to filter log entries, each filtering level will hide
 * lower level entries
 */
enum LogLevel { Trace = -1, Debug = 0, Info = 1, Warn = 2, Error = 3, Fatal = 4 };

#endif // ATILACALCULATORSOFTWARE_COMMON_H
