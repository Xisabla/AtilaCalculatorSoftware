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

class result_exception : public std::exception {
  private:
    std::string m_error;

  public:
    result_exception(std::string error): m_error(error) { }

    const char* what() const noexcept { return m_error.c_str(); } // C++11 version
};


struct Str_Result {
    std::string analysis_;
    std::string results_;
    float step_;
    int result_size_;

    int number_of_results_ = 0;

    std::vector<std::string> component_names_;

    std::unique_ptr<int[]> node_numbers_;
    std::unique_ptr<float[]> data_;

    Str_Result(gzFile file_msh, dataFields names, const int result_size);

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

#endif // ATILACALCULATORSOFTWARE_RESULT_H
