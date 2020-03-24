#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "result.hpp"
#include "gidpost.h"

struct Str_Result_GiD : Str_Result{
     GiD_ResultType     Type_;
     GiD_ResultLocation Where_ = GiD_OnNodes;
     string        		GaussPointsName_;
     string        		RangeTable_;
     int                compc_;
     string        		compv_;
     Str_Result_GiD(gzFile file_msh, char names[10][40], const int result_size) : Str_Result(file_msh, names,result_size){};
     void write_to_post_gid_file();
};
