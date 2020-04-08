#include "binary_data_class.hpp"

using namespace std ;

Binary_data_class::Binary_data_class(string str):Str_binary_data_GiD(str),pathToFile(str){
    read_meshes();
    while (auto one_result = read_one_result()) {
	    results_.emplace_back(move(*one_result));
	}
}
string Binary_data_class::getPath() const {
    return pathToFile;
}
Binary_data_class::~Binary_data_class(){
    
}