/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryDataGiD.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_BINARYDATAGID_H
#define ATILACALCULATORSOFTWARE_BINARYDATAGID_H

#include "BinaryData.h"

#include <gidpost.h>
#include <math.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class Str_binary_data_GiD : public BinaryData {
  public:
    //	std::vector<Str_Mesh_GiD> meshes_;
    Str_binary_data_GiD(std::string file);
    void write_meshes();
    void write_one_step_to_post_gid_file(const float& step, const int& number_frame);
};

#endif // ATILACALCULATORSOFTWARE_BINARYDATAGID_H
