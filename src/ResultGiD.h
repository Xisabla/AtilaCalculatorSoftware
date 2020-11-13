/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      ResultGiD.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_RESULTGID_H
#define ATILACALCULATORSOFTWARE_RESULTGID_H

#include "Result.h"

#include <gidpost.h>
/*
struct Str_Result_GiD : Str_Result {
    GiD_ResultType Type_;
    GiD_ResultLocation Where_ = GiD_OnNodes;
    std::string GaussPointsName_;
    std::string RangeTable_;
    int compc_;
    std::string compv_;
    Str_Result_GiD(gzFile file_msh, char names[10][40], const int result_size)
    : Str_Result(file_msh, names, result_size) {};
    void write_to_post_gid_file();
};*/

#endif // ATILACALCULATORSOFTWARE_RESULTGID_H
