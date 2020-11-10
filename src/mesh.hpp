#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <tuple>


#include "zlib.h"

using std::string;



#if (defined(WIN32)||defined(WIN64))
#define _USE_MATH_DEFINES
//#define  sscanf    sscanf_s
#endif
#include <math.h>

#define SIZE 2000


#define MYGZREAD(_FILE_, _DATA_, _Size_) \
	if (static_cast<int>((_Size_) * sizeof(*(_DATA_))) != gzread((_FILE_), (_DATA_), _Size_ * sizeof(*(_DATA_)))) return(-1);


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
	const string mesh_name_;
	const int ndim_;
	const string element_name_;
	const int nnode_;
	static size_t number_of_nodes_max_;

	int nb_of_elements_ = 0;
	std::vector<Str_node> tab_of_nodes_;
	std::unique_ptr<int[]> elements_;
	std::unique_ptr<int[]> element_numbers_;
	Str_Mesh(gzFile file_msh, char fields[10][40]);
	std::tuple<int&, int*> const get_an_element(const int &ind);
};

int get_fields(gzFile  file_msh, char dummy_char[SIZE], char fields[10][40]);
