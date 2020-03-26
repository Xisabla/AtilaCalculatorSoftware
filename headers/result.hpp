#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <memory>
#include <exception> // for std::exception
#include <tuple>

#include "zlib.h"

using std::string;



class result_exception: public std::exception
{
private:
	std::string m_error;

public:
	result_exception(std::string error)
		: m_error(error)
	{
	}

	const char* what() const noexcept { return m_error.c_str(); } // C++11 version
};


struct Str_Result {
	string 	analysis_;
	string 	results_;
	float 	step_;
	int 	result_size_;

	int number_of_results_ = 0;

	std::vector<string> component_names_;

	std::unique_ptr<int[]> node_numbers_;
	std::unique_ptr<float[]> data_;

	Str_Result(gzFile file_msh, char names[10][40], const int result_size);

	std::tuple<int&, float*> const get_one_result(const int &ind);
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

