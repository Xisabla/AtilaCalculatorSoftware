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

#include <functional>
#include <gidpost.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <zlib.h>

struct Str_binary_data {

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
