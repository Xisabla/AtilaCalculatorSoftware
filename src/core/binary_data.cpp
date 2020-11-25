/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      binary_data.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "core/binary_data.h"

//  --------------------------------------------------------------------------------------
//  BINARY DATA
//  --------------------------------------------------------------------------------------

BinaryData::BinaryData(const std::string& file) {
    // Read compressed file in r1 mode
    Logger::info("Verifying BinaryData file: ", file.c_str());
    this->file = gzopen(file.c_str(), "r1");

    char buffer[GZ_BUFFER_SIZE];
    int size;

    if (!this->file) __THROW__("Unable to load or locate file: " + file);
    if (gzread(this->file, &size, sizeof(byteOrderCheck)) && byteOrderCheck != size)
        __THROW__("Cannot read binary file: invalid buffer size: " + std::to_string(size) +
                  " , expected " + std::to_string(byteOrderCheck));
    if (gzread(this->file, &size, sizeof(int)) == 0) __THROW__("Cannot read binary file");
    if (gzread(this->file, buffer, size * sizeof(char)) == 0) __THROW__("Cannot read binary file");
}

BinaryData::~BinaryData() {
    // Close file on destruction (when another file is opened)
    Logger::trace("Destructing BinaryData");
    if (this->file != nullptr) gzclose(this->file);
    this->meshes.clear();
    this->results.clear();
}

//  --------------------------------------------------------------------------------------
//  BINARY DATA > GETTERS
//  --------------------------------------------------------------------------------------

gzFile BinaryData::getFile() { return this->file; }

std::vector<Mesh>& BinaryData::getMeshes() { return this->meshes; }
std::vector<Result>& BinaryData::getResults() { return this->results; }

//  --------------------------------------------------------------------------------------
//  BINARY DATA > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

void BinaryData::readMeshes() {
    Logger::info("Reading meshes...");
    char buffer[GZ_BUFFER_SIZE];
    dataFields fields;

    z_off_t currentPos = gztell(file);
    unsigned int size = getFields(file, buffer, fields);

    // While we can a mesh
    while (size > 0 && !strcmp(fields[0], "MESH")) {
        // Read the mesh
        this->meshes.emplace_back(file, fields);

        currentPos = gztell(file);
        size = getFields(file, buffer, fields);

        Logger::debug("Mesh read, current pos: ", currentPos);
    }

    Logger::debug("No more mesh to read");

    // Check final position
    if (gzseek(file, currentPos, SEEK_SET) == -1)
        __THROW__("Cannot read binary file; Invalid final position");

    Logger::info("Reading meshes: Done");
}

std::optional<Result> BinaryData::readResult() {
    Logger::trace("Reading one result...");
    if (Mesh::maxNodeCount == 0) return std::nullopt;

    char buffer[GZ_BUFFER_SIZE];
    dataFields fields;

    // No results in the file
    if (!getFields(file, buffer, fields)) return std::nullopt;

    // Check for componentCount and read result
    if (!strcmp(fields[0], "Result")) {
        if (!strcmp(fields[4], "Vector")) {
            Logger::trace("Returning Vector result...");
            return Result(file, fields, 4);
        } else if (!strcmp(fields[4], "Scalar")) {
            Logger::trace("Returning Scalar result...");
            return Result(file, fields, 1);
        } else if (!strcmp(fields[4], "Matrix")) {
            Logger::trace("Returning Matrix result...");
            return Result(file, fields, 6);
        }
    }

    Logger::warn("No more result to read");
    return std::nullopt;
}

std::vector<Result> BinaryData::readResults(unsigned int n) {
    Logger::info("Reading all results...");
    if (n > 0 && this->results.size() >= n) return this->results;
    unsigned int current = static_cast<int>(this->results.size());

    while (std::optional<Result> result = readResult()) {
        this->results.emplace_back(std::move(*result));

        // Never break if n = 0, break if we have read enough values
        if (n > 0 && ++current >= n) break;
    }

    Logger::info("Reading all results: Done");
    return this->results;
}
