/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryData.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "BinaryData.h"

//  --------------------------------------------------------------------------------------
//  BINARY_DATA
//  --------------------------------------------------------------------------------------

BinaryData::BinaryData(std::string file) {
    this->file = gzopen(file.c_str(), "r1");

    char buffer[GZ_BUFFER_SIZE];
    int size;

    if (!this->file) __THROW__("Unable to load or locate file: " + file);
    if (gzread(this->file, &size, sizeof(byteOrderCheck)) && byteOrderCheck != size)
        __THROW__("Cannot read binary file: invalid buffer size: " + std::to_string(size) +
                  " , expected " + std::to_string(byteOrderCheck));
    if (gzread(this->file, &size, sizeof(int)) == 0) __THROW__("Cannot read binary file");
    if (gzread(this->file, buffer, size * sizeof(char)) == 0) __THROW__("Cannot read binary file");

    // this->readResults();
}

BinaryData::BinaryData(QString filename) { BinaryData(filename.toStdString()); }

BinaryData::~BinaryData() {
    if(file != NULL) gzclose(file);
    meshes.clear();
    results.clear();
}

//  --------------------------------------------------------------------------------------
//  BINARY_DATA > GETTERS
//  --------------------------------------------------------------------------------------

const gzFile BinaryData::getFile() { return this->file; }

std::vector<Mesh::Mesh>& BinaryData::getMeshes() { return this->meshes; }
std::vector<Result> &BinaryData::getResults() { return this->results; }

//  --------------------------------------------------------------------------------------
//  BINARY_DATA > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

void BinaryData::readMeshes() {
    char buffer[GZ_BUFFER_SIZE];
    dataFields fields;

    z_off_t currentPos = gztell(file);
    int size = Mesh::getFields(file, buffer, fields);

    while (size > 0 && !strcmp(fields[0], "MESH")) {
        meshes.push_back(Mesh::Mesh(file, fields));

        currentPos = gztell(file);
        size = Mesh::getFields(file, buffer, fields);
    }

    if (gzseek(file, currentPos, SEEK_SET) == -1)
        __THROW__("Cannot read binary file; Invalid final position");
}

std::optional<Result> BinaryData::readResult() {
    if (Mesh::Mesh::maxNodeCount == 0) return std::nullopt;

    char buffer[GZ_BUFFER_SIZE];
    dataFields fields;

    if (!Mesh::getFields(file, buffer, fields)) return std::nullopt;
    if (!strcmp(fields[0], "Result")) {
        if (!strcmp(fields[4], "Vector")) {
            return Result(file, fields, 4);
        } else if (!strcmp(fields[4], "Scalar")) {
            return Result(file, fields, 1);
        } else if (!strcmp(fields[4], "Matrix")) {
            return Result(file, fields, 6);
        }
    }

    return std::nullopt;
}

std::vector<Result> BinaryData::readResults(unsigned int n) {
    if(n > 0 && results.size() >= n) return results;
    unsigned int current = results.size();

    while(std::optional<Result> result = readResult()) {
        results.emplace_back(std::move(*result));

        if(n > 0 && ++current >= n) break;
    }

    return results;
}

//  --------------------------------------------------------------------------------------
//  BINARY_DATA > PRIVATE METHODS
//  --------------------------------------------------------------------------------------