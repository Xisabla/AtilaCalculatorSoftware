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

struct Str_Result {
    // std::string analysis;
    std::string analysis_;
    // std::string results;
    std::string results_;
    // float step;
    float step_;
    // int resultSize;
    int result_size_;

    // int resultCount
    int number_of_results_ = 0;

    // std::vector<std::string> components
    std::vector<std::string> component_names_;

    // std::uniq_ptr<int[]> nodeCount;
    std::unique_ptr<int[]> node_numbers_;
    // std::uniq_ptr<float[]> value;
    std::unique_ptr<float[]> data_;

    // Result(gzFile file, dataFields fields, const int result_size);
    Str_Result(gzFile file_msh, dataFields names, const int result_size);

    // std::tuple<int&, float*> const getResult(const int& ind);
    std::tuple<int&, float*> const get_one_result(const int& ind);
    /*
    void add_result(const int &ind, const std::array<float, one_result_max_size> &data) {
        this->data.emplace_back(ind, data);
    }
    */
    /*
    void add_component_names(const char names[10][40]) {
        for (int i = 0; i < size; ++i) {
            this->component_names[i] = names[i];
        }
    }
    */
};

// - Work In Progress ------------------------------------------------------------------------------

/**
 * @class Result
 * @brief Representation of all the results and their components read from a res file
 */
class Result {
  public:
    Result(gzFile file, dataFields fields, const int componentCount);

    const std::string getAnalysis();
    const std::string getResults();
    const float getStep();
    const unsigned int getComponentCount();
    const unsigned int getResultCount();
    const std::vector<std::string> getComponents();

    // TODO: Remove
    void moveNodeCounts(std::unique_ptr<int[]>* ptr);
    void moveValues(std::unique_ptr<float[]>* ptr);

    /**
     * @param id ID of the result
     * @return Return information about the result
     */
    const std::tuple<int&, float*> getResult(const int& id);

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
    std::string results;

    /**
     * @brief Time step inside the analysis
     */
    float step;

    /**
     * @brief Number of components of the result
     */
    unsigned int componentCount;

    /**
     * @brief Number of results read
     */
    unsigned int resultCount;

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
