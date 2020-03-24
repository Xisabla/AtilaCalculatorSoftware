#pragma once
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include "gidpost.h"
#include "mesh_GiD.hpp"
#include "result_GiD.hpp"
#include "binary_data.hpp"

struct Str_binary_data_GiD : Str_binary_data{
//	std::vector<Str_Mesh_GiD> meshes_;
	Str_binary_data_GiD(std::string file);
    void write_meshes();
    void write_one_step_to_post_gid_file(const float &step, const int &number_frame);
};
