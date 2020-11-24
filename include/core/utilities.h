/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      utils.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_UTILITIES_H
#define ATILACALCULATORSOFTWARE_UTILITIES_H

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <zlib.h>

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

#ifdef DEBUG
#define __DEBUG__(x) std::cout << "[" << __HERE__ << "]" << x << std::endl;
#define __DEBUG_FIELDS__(x)                                                          \
    __DEBUG__(std::string("Fields: ") + std::string(fields[0]) + std::string(", ") + \
              std::string(fields[1]) + std::string(", ") + std::string(fields[2]) +  \
              std::string(", ") + std::string(fields[3]) + std::string(", ") +       \
              std::string(fields[4]) + std::string(", ") + std::string(fields[5]) +  \
              std::string(", ") + std::string(fields[6]) + std::string(", ") +       \
              std::string(fields[7]) + std::string(", ") + std::string(fields[8]) +  \
              std::string(", ") + std::string(fields[9]));
#define __PDEBUG__(x, y) std::cout << "[" << __HERE__ << "]" << x << ": " << y << std::endl;
#else
#define __DEBUG__(x) \
    { }
#define __DEBUG_FIELDS__(x) \
    { }
#define __PDEBUG__(x, y) \
    { }
#endif


/**
 * @brief Macro to read a compressed file using gzread (zlib) that returns -1 if the read size is
 * not the expected size
 */
#define GZ_READ_CHECK(FILE, BUFFER, LEN)                   \
    if (static_cast<int>((LEN) * sizeof(*(BUFFER))) !=     \
        gzread((FILE), (BUFFER), (LEN) * sizeof(*(BUFFER)))) \
        return (-1);

//  --------------------------------------------------------------------------------------
//  STRUCTURES AND TYPES
//  --------------------------------------------------------------------------------------

typedef char dataFields[10][40];

//  --------------------------------------------------------------------------------------
//  METHODS
//  --------------------------------------------------------------------------------------

/**
 * @brief Read fields from the given resource file
 * @param file resource file to read
 * @param buffer Zlib reading buffer
 * @param fields dataFields pointer in which fields will be written
 * @return The size of read data
 */
unsigned int getFields(gzFile file, char* buffer, dataFields fields);

#endif // ATILACALCULATORSOFTWARE_UTILITIES_H