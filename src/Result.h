/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Result.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_RESULT_H
#define ATILACALCULATORSOFTWARE_RESULT_H

#include "Mesh.h"

#include <cstring>
#include <exception>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <zlib.h>

/**
 * @class Result
 * @brief Representation of all the results and their components read from a res file
 */
class Result {
  public:
    Result(gzFile file, dataFields fields, const int componentCount);

    /**
     * @return The type of analysis of the result
     */
    const std::string getAnalysis();

    /**
     * @return The type of result
     */
    const std::string getResultType();

    /**
     * @return Step of the result
     */
    const float getStep();

    /**
     * @return The number component of the result (1 or 4)
     */
    const unsigned int getComponentCount();

    /**
     * @return The number of values of the result
     */
    const unsigned int getValuesCount();

    /**
     * @return The components of the result (X, Y, Z, M)
     */
    const std::vector<std::string> getComponents();

    /**
     * @param id ID of the result
     * @return Return information about the result
     */
    const std::tuple<int&, float*> getResult(const int& id);

    //    /**
    //     * @brief Write the result into a currently open PostResultFile
    //     * @return The state of the GiD Post Result closure
    //     */
    //    const int toPostGid();

  private:
    /**
     * Read the result components from the mesh file
     * @param file File that contains the result and meshes information
     * @param buffer The zlib reading buffer from previous gzread
     * @param fields Base fields read from the file
     */
    void readComponents(gzFile file, char buffer[GZ_BUFFER_SIZE], dataFields fields);

    /**
     * Read the results values from the res file
     * @param file File that contains the result information
     * @param buffer Thz zlib reading buffer from previous gzread
     */
    void readResults(gzFile file, char buffer[GZ_BUFFER_SIZE]);

    /**
     * @brief Type of analysis
     */
    std::string analysis;

    /**
     * @brief Type of results
     */
    std::string result;

    /**
     * @brief Time step inside the analysis
     */
    float step;

    /**
     * @brief Number of components of the result
     */
    unsigned int componentCount;

    /**
     * @brief Number of values read
     */
    unsigned int valuesCount;

    /**
     * @brief Components of the result (size = componentCount)
     */
    std::vector<std::string> components;

    /**
     * @brief List of IDs of the nodes involved in the result
     */
    std::unique_ptr<int[]> nodeIDs;

    /**
     * @brief Values of the result
     */
    std::unique_ptr<float[]> values;
};

#endif // ATILACALCULATORSOFTWARE_RESULT_H
