#pragma once
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <string>
#include <functional>
#include <iostream>
#include <optional>
#include "mesh.hpp"
#include "result.hpp"

using My_fields = char[10][40];

struct Str_binary_data{

	gzFile file_msh_ = nullptr;
	bool   meshes_read_ = false;
	z_off_t result_position_ = 0L;

	std::vector<Str_Mesh> meshes_;
	std::vector<Str_Result> results_;

//	Str_binary_data();
	Str_binary_data(string file);
	~Str_binary_data();

	bool read_meshes();
	bool read_all_results();
	std::optional<Str_Result> read_one_result();

};
