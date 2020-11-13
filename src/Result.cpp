/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Result.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "Result.h"

//  --------------------------------------------------------------------------------------
//  RESULT
//  --------------------------------------------------------------------------------------

Result::Result(gzFile file, char (*fields)[40], const int componentCount) {
    // Import header data
    this->analysis = fields[1];
    this->results = fields[2];
    this->step = static_cast<float>(std::atof(fields[3]));
    this->componentCount = componentCount;

    // Shared buffer
    char buffer[GZ_BUFFER_SIZE];

    // Read data
    this->readComponents(file, buffer, fields);
    this->readResults(file, buffer);
}

//  --------------------------------------------------------------------------------------
//  RESULT > GETTERS
//  --------------------------------------------------------------------------------------

const std::string Result::getAnalysis() { return this->analysis; }
const std::string Result::getResults() { return this->results; }
const float Result::getStep() { return this->step; }
const unsigned int Result::getComponentCount() { return this->componentCount; }
const unsigned int Result::getResultCount() { return this->resultCount; }
const std::vector<std::string> Result::getComponents() { return this->components; }

//  --------------------------------------------------------------------------------------
//  RESULT > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

const std::tuple<int&, float*> Result::getResult(const int& id) {
    return std::make_tuple(std::ref(nodeIDs[id]), &values[id * componentCount]);
}

//  --------------------------------------------------------------------------------------
//  RESULT > PRIVATE METHODS
//  --------------------------------------------------------------------------------------

void Result::readComponents(gzFile file, char* buffer, char (*fields)[40]) {
    Mesh::getFields(file, buffer, fields);

    if (!strncmp(fields[0], "ComponentNames", 14)) {
        for (unsigned int i = 0; i < componentCount; i++) this->components.emplace_back(fields[i]);

        Mesh::getFields(file, buffer, fields);
    } else {
        if (componentCount == 1) {
            strcpy(fields[0], "X");
            this->components.emplace_back(fields[0]);
        } else if (componentCount == 4) {
            strcpy(fields[0], "X");
            strcpy(fields[1], "Y");
            strcpy(fields[2], "Z");
            strcpy(fields[3], "M");

            for (unsigned int i = 0; i < componentCount; i++) {
                this->components.emplace_back(fields[i]);
            }
        }
    }
}

void Result::readResults(gzFile file, char* buffer) {
    unsigned int gzReadSize;
    unsigned int node = 0;
    unsigned int shiftNodes = 0;
    unsigned int shiftResults = 0;

    std::unique_ptr<int[]> nodes = std::make_unique<int[]>(Mesh::Mesh::maxNodeCount);
    std::unique_ptr<float[]> results =
    std::make_unique<float[]>(componentCount * Mesh::Mesh::maxNodeCount);

    unsigned int readingSize = componentCount * sizeof(float);

    gzread(file, &node, sizeof(int));

    // Read nodes
    while (node != -1) {
        gzReadSize = gzread(file, &results[shiftResults], readingSize);
        nodes[shiftNodes++] = node;
        shiftResults += componentCount;

        if (gzReadSize == 0) __THROW__("Unexpected read size: 0");

        gzReadSize = gzread(file, &node, sizeof(int));

        if (gzReadSize == 0) __THROW__("Unexpected read size: 0");
    }

    // Read node and get buffer
    gzread(file, &node, sizeof(int));
    gzread(file, buffer, node);

    if (strcmp(buffer, "End Values")) __THROW__("Unexpected end of values");

    this->nodeIDs = std::make_unique<int[]>(shiftNodes);
    this->values = std::make_unique<float[]>(shiftResults);
    std::memcpy(this->nodeIDs.get(), nodes.get(), shiftNodes * sizeof(int));
    std::memcpy(this->values.get(), results.get(), shiftResults * sizeof(int));
    this->resultCount = shiftNodes;
}