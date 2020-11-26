/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      result.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "core/result.h"

//  --------------------------------------------------------------------------------------
//  RESULT
//  --------------------------------------------------------------------------------------

Result::Result(gzFile file, char (*fields)[40], const int componentCount) {
    Logger::trace("Reading result...");
    // Import header data
    this->analysis = fields[1];
    this->result = fields[2];
    this->step = static_cast<float>(std::atof(fields[3]));
    this->componentCount = componentCount;

    Logger::trace("Result: ",
                  this->analysis,
                  " ",
                  this->result,
                  this->componentCount,
                  " components (",
                  this->step,
                  " steps)");

    // Shared buffer
    char buffer[GZ_BUFFER_SIZE];

    // Read data
    this->readComponents(file, buffer, fields);
    this->readResults(file, buffer);

    Logger::trace("Reading result: Done");
}

//  --------------------------------------------------------------------------------------
//  RESULT > GETTERS
//  --------------------------------------------------------------------------------------

std::string Result::getAnalysis() { return this->analysis; }
std::string Result::getResultType() { return this->result; }

float Result::getStep() const { return this->step; }

int Result::getComponentCount() const { return this->componentCount; }
unsigned int Result::getValuesCount() const { return this->valuesCount; }

std::vector<std::string> Result::getComponents() { return this->components; }

//  --------------------------------------------------------------------------------------
//  RESULT > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

std::tuple<int&, float*> Result::getResult(const unsigned int& id) {
    return std::make_tuple(std::ref(this->nodeIDs[id]), &this->values[id * this->componentCount]);
}

// const int Result::toPostGid() {
//
// }

//  --------------------------------------------------------------------------------------
//  RESULT > PRIVATE METHODS
//  --------------------------------------------------------------------------------------

void Result::readComponents(gzFile file, char* buffer, char (*fields)[40]) {
    Logger::trace("Result: ", this->result, ": reading components...");
    getFields(file, buffer, fields);

    if (!strncmp(fields[0], "ComponentNames", 14)) {
        Logger::trace("Read component: ", fields[1]);
        for (unsigned int i = 0; i < componentCount; i++) this->components.emplace_back(fields[i]);

        getFields(file, buffer, fields);
    } else if (componentCount == 1) {
        strcpy(fields[0], "X");
        Logger::trace("X Component");
        this->components.emplace_back(fields[0]);
    } else if (componentCount == 4) {
        Logger::trace("X,Y,Z,M Components");
        strcpy(fields[0], "X");
        strcpy(fields[1], "Y");
        strcpy(fields[2], "Z");
        strcpy(fields[3], "M");

        for (unsigned int i = 0; i < componentCount; i++) {
            this->components.emplace_back(fields[i]);
        }
    }

    Logger::trace("Result: ", this->result, ": reading components: Done");
}

void Result::readResults(gzFile file, char* buffer) {
    Logger::trace("Result: ", this->result, ": reading results entries...");
    unsigned int node = 0;
    unsigned int shiftNodes = 0;
    unsigned int shiftResults = 0;

    int* nodes = static_cast<int*>(malloc(sizeof(int) * Mesh::Mesh::maxNodeCount));
    auto* results =
    static_cast<float*>(malloc(sizeof(float) * componentCount * Mesh::Mesh::maxNodeCount));


    unsigned int readingSize = this->componentCount * sizeof(float);

    gzread(file, &node, sizeof(int));

    // Read nodes
    while (node != -1) {
        int gzReadSize = gzread(file, &results[shiftResults], readingSize);
        nodes[shiftNodes++] = static_cast<int>(node);
        shiftResults += this->componentCount;

        if (gzReadSize == 0) __THROW__("Unexpected read size: 0");

        gzReadSize = gzread(file, &node, sizeof(int));

        if (gzReadSize == 0) __THROW__("Unexpected read size: 0");
    }

    // Read node and get buffer
    gzread(file, &node, sizeof(int));
    gzread(file, buffer, node);

    if (strcmp(buffer, "End Values") != 0) __THROW__("Unexpected end of values");

    this->nodeIDs = nodes;
    this->values = results;
    this->valuesCount = shiftNodes;

    Logger::trace("Result: ", this->result, ": reading results entries: Done");
}
