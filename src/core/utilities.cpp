/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      utils.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "core/utilities.h"

unsigned int getFields(gzFile file, char* buffer, dataFields fields) {
    unsigned int size = 0;

    GZ_READ_CHECK(file, &size, 1);
    GZ_READ_CHECK(file, buffer, size);

    char* ptr = strtok(buffer, " ");

    while ((ptr[0] == '#' || !strcmp(ptr, "Unit") || size == 1) && size != 0) {
        GZ_READ_CHECK(file, &size, 1);
        GZ_READ_CHECK(file, buffer, size);
        ptr = strtok(buffer, " ");
    }

    if (!size) __THROW__("No more data to read");

    size = 0;
    while (ptr != nullptr) {
        if (ptr[0] == '"') {
            // Set current field
            strcpy(fields[size], ptr);

            while (ptr[strlen(ptr) - 1] != '"') {
                ptr = strtok(nullptr, " ");
                strcat(fields[size], ptr);
            }

            size_t len = strlen(fields[size]);

            // Shift fields
            for (size_t i = 0; i < len - 2; ++i) {
                fields[size][i] = fields[size][i + 1];
            }

            // Close field
            fields[size][len - 2] = '\0';
            ++size;
        } else {
            // Set next field
            strcpy(fields[size++], ptr);
        }

        ptr = strtok(nullptr, " ");
    }

    return size;
}
