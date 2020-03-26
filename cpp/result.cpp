#include <cstring>
#include "mesh.hpp"
#include "result.hpp"



Str_Result::Str_Result(gzFile file_msh, char fields[10][40], const int result_size) :
	analysis_(fields[1]), results_(fields[2]),
	step_(static_cast<float>(std::atof(fields[3]))), result_size_(result_size) {
	char dummy_char[SIZE];
	int node_number = 0;

	int size_to_read = result_size * sizeof(float);

	auto node_numbers = std::make_unique<int[]>(Str_Mesh::number_of_nodes_max_);
	auto results = std::make_unique<float[]>(result_size * Str_Mesh::number_of_nodes_max_);

	auto local_size = get_fields(file_msh, dummy_char, fields);
	if (!strncmp(fields[0], "ComponentNames", 14)) {
		for (auto i = 0; i < result_size; ++i) {
			this->component_names_.emplace_back(fields[i]);
		}
		local_size = get_fields(file_msh, dummy_char, fields);
	}
	/*else {
		if (result_size == 1){
			strcpy(fields[0],"X");
			this->component_names_.emplace_back(fields[0]);
		}
		else if (result_size == 4){
			strcpy(fields[0],"X");
			strcpy(fields[1],"Y");
			strcpy(fields[2],"Z");
			strcpy(fields[3],"M");
			for (auto i = 0; i < result_size; ++i) {
				this->component_names_.emplace_back(fields[i]);
			}
		}
	}
	*/

	local_size = gzread(file_msh, &node_number, sizeof(int));
	int shift_nodes = 0;
	int shift_results = 0;
	while (node_number != -1) {
		local_size = gzread(file_msh, &results[shift_results], size_to_read);
		node_numbers[shift_nodes++] = node_number;
		shift_results += result_size;
		if (local_size == 0){
			throw string("Cannot read binary file (Result)");
		}

		//		result.add_result(node_number - 1, float_fields);
		local_size = gzread(file_msh, &node_number, sizeof(int));
		if (local_size == 0){
			throw string("Cannot read binary file (Result)");
		}

	}
	local_size = gzread(file_msh, &node_number, sizeof(int));
	local_size = gzread(file_msh, dummy_char, node_number);
	if (strcmp(dummy_char, "End Values")){
		throw result_exception{"No more result"};
	}

	this->node_numbers_ = std::make_unique<int[]>(shift_nodes);
	this->data_ = std::make_unique<float[]>(shift_results);
	std::memcpy(this->node_numbers_.get(), node_numbers.get(), shift_nodes * sizeof(int));
	std::memcpy(this->data_.get(), results.get(), shift_results * sizeof(int));
	this->number_of_results_ = shift_nodes;
}

std::tuple<int&, float*> const Str_Result::get_one_result(const int &ind) {
	return std::make_tuple(std::ref(node_numbers_[ind]), &data_[ind*result_size_]);
}

