/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryData.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_BINARYDATA_H
#define ATILACALCULATORSOFTWARE_BINARYDATA_H

#include "Mesh.h"
#include "Result.h"

#include <QString>
#include <functional>
#include <gidpost.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <zlib.h>

// - Work In Progress ------------------------------------------------------------------------------

/**
 * @class BinaryData
 * @brief Load the resources file and allow to read meshes and results from it
 */
class BinaryData {
  public:
    /**
     * @param file Path to the res gzFile
     */
    BinaryData(std::string file);

    /**
     * @param filename Path to the res gzFile
     */
    BinaryData(QString filename);

    /**
     * @return The currently open res gzFile
     */
    const gzFile getFile();

    /**
     * @return The loaded meshes
     */
    std::vector<Mesh::Mesh>& getMeshes();

    /**
     * @return The loaded results
     */
    std::vector<Result>& getResults();

    /**
     * @brief Closes open files
     */
    ~BinaryData();

    /**
     * Read all meshes from the res gzFile
     */
    void readMeshes();

    /**
     * @brief Read one result from the res gzFile
     * @return The result read or a nullopt if no result was found
     */
    std::optional<Result> readResult();

    /**
     * @brief Read the n given results, stops if there is no more result
     * @param n The number of results to read (0 = all)
     * @return The results read
     */
    std::vector<Result> readResults(unsigned int n = 0);

  protected:
    /**
     * @brief Opened res gzFile
     */
    gzFile file;

    /**
     * @brief Read meshes
     */
    std::vector<Mesh::Mesh> meshes;

    /**
     * @brief Read meshes
     */
    std::vector<Result> results;

    const static int byteOrderCheck = 0x91d;
};

// - Work In Progress ------------------------------------------------------------------------------
struct Str_binary_data {
    BinaryData bd;
    gzFile file_msh_ = nullptr;
    bool meshes_read_ = false;
    z_off_t result_position_ = 0L;

    std::vector<Mesh::Mesh> meshes_;
    std::vector<Result> results_;

    //	Str_binary_data();
    Str_binary_data(std::string file);
    ~Str_binary_data();

    bool read_meshes();
    bool read_all_results();
    std::optional<Result> read_one_result();
};

#endif // ATILACALCULATORSOFTWARE_BINARYDATA_H
