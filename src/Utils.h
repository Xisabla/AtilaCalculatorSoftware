/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Utils.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_UTILS_H
#define ATILACALCULATORSOFTWARE_UTILS_H

#include "zlib.h"
#include <ostream>

//  --------------------------------------------------------------------------------------
//  DEFINES
//  --------------------------------------------------------------------------------------

/**
 * @brief Buffer size for gzread uses
 */
#define GZ_BUFFER_SIZE 2000

//  --------------------------------------------------------------------------------------
//  MACROS
//  --------------------------------------------------------------------------------------

/**
 * @brief Macro to get the current LINE and FILE (format: FILE:LINE)
 */
#define __HERE__ std::string(__FILE__) + ":" + std::to_string(__LINE__)

/**
 * @brief Macro to throw a running error, showing the current line and file (see __HERE__ macro)
 */
#define __THROW__(x) throw std::runtime_error(std::string("[" + __HERE__ + "] " + x));

// #define __LOG__(x)
// #define __DEBUG__(x)

/**
 * @brief Macro to read a compressed file using gzread (zlib) that returns -1 if the read size is
 * not the expected size
 */
#define GZ_READ_CHECK(FILE, BUFFER, LEN)                   \
    if (static_cast<int>((LEN) * sizeof(*(BUFFER))) !=     \
        gzread((FILE), (BUFFER), LEN * sizeof(*(BUFFER)))) \
        return (-1);

//  --------------------------------------------------------------------------------------
//  STRUCTURES AND TYPEs
//  --------------------------------------------------------------------------------------

// using dataFields = char[10][40];
typedef char dataFields[10][40];

#endif // ATILACALCULATORSOFTWARE_UTILS_H