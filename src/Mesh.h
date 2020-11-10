/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Mesh.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_MESH_H
#define ATILACALCULATORSOFTWARE_MESH_H

#include "Utils.h"

#include <algorithm>
#include <cstring>
#include <math.h>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <zlib.h>

struct Str_node {
    int number;
    float coord[3];
    static const size_t size_coord = 3 * sizeof(float);
    Str_node(int number, float coord[3]) {
        this->number = number;
        std::memcpy(this->coord, coord, size_coord);
    }
};


struct Str_Mesh {
    const std::string mesh_name_;
    const int ndim_;
    const std::string element_name_;
    const int nnode_;
    static size_t number_of_nodes_max_;

    int nb_of_elements_ = 0;
    std::vector<Str_node> tab_of_nodes_;
    std::unique_ptr<int[]> elements_;
    std::unique_ptr<int[]> element_numbers_;
    Str_Mesh(gzFile file_msh, dataFields fields);
    std::tuple<int&, int*> const get_an_element(const int& ind);
};

int getFields(gzFile file, char* buffer, dataFields fields);

#endif // ATILACALCULATORSOFTWARE_MESH_H
