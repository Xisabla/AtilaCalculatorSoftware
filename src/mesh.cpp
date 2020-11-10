#include "mesh.hpp"
#include <algorithm> // std::max

Str_Mesh::Str_Mesh(gzFile file_msh, char fields[10][40])
: mesh_name_(fields[1]), ndim_(std::atoi(fields[3])), element_name_(fields[5]),
  nnode_(std::atoi(fields[7])) {

    if (strcmp("dimension", fields[2])) {
        throw string("Cannot read binary file (Mesh)");
    }

    if (strcmp("ElemType", fields[4])) {
        throw string("Cannot read binary file (Mesh)");
    }

    if (strcmp("Nnode", fields[6])) {
        throw string("Cannot read binary file (Mesh)");
    }

    float coord[3];
    char dummy_char[SIZE];
    int node_number = 1;
    int element_number = 1;
    int count = 0;
    int status;

    if (get_fields(file_msh, dummy_char, fields) == 0) {
        throw string("Cannot read binary file (Mesh)");
    }
    if (!strcmp(fields[0], "Coordinates")) {
        count = node_number;
        while (count) {
            status = gzread(file_msh, &node_number, sizeof(int));
            if (status == 0) {
                throw string("Cannot read binary file (Mesh)");
            }

            if (node_number != count) {
                status = gzread(file_msh, dummy_char, node_number);
                if (status == 0) {
                    throw string("Cannot read binary file (Mesh)");
                }
                if (!strcmp(dummy_char, "End Coordinates")) {
                    node_number = count;
                    break;
                }
            }
            ++count;
            status = gzread(file_msh, &coord, sizeof(coord));
            if (status == 0) {
                throw string("Cannot read binary file (Mesh)");
            }
            tab_of_nodes_.emplace_back(node_number, coord);
        }
        number_of_nodes_max_ = std::max(number_of_nodes_max_, tab_of_nodes_.size());
    }

    if (get_fields(file_msh, dummy_char, fields) == 0) {
        throw string("Cannot read binary file");
    }
    if (!strcmp(fields[0], "Elements")) {
        const int nb_nodes = nnode_ + 1;
        //		const int nb_nodes = nnode_;
        const int size_of_an_element = (nb_nodes) * sizeof(int);
        std::unique_ptr<int[]> element_numbers { new int[number_of_nodes_max_] };
        std::unique_ptr<int[]> elements { new int[nb_nodes * number_of_nodes_max_] };
        int shift_nodes = 0;
        int shift_elements = 0;
        static int count = 1;
        while (count) {
            status = gzread(file_msh, &element_number, sizeof(int));
            if (status == 0) {
                throw string("Cannot read binary file (Mesh)");
            }
            if (element_number != count) {
                status = gzread(file_msh, dummy_char, element_number);
                if (!strcmp(dummy_char, "End Elements")) {
                    element_number = count;
                    break;
                }
            }
            ++count;
            element_numbers[shift_nodes++] = element_number;
            status = gzread(file_msh, &elements[shift_elements], size_of_an_element);
            shift_elements += nb_nodes;
            //			elements.emplace_back(element_number, node_numbers);
            if (status == 0) {
                throw string("Cannot read binary file (Mesh)");
            }
        }

        this->element_numbers_ = std::make_unique<int[]>(shift_nodes);
        this->elements_ = std::make_unique<int[]>(shift_elements);
        memcpy(this->element_numbers_.get(), element_numbers.get(), shift_nodes * sizeof(int));
        memcpy(this->elements_.get(), elements.get(), shift_elements * sizeof(int));
        this->nb_of_elements_ = shift_nodes;
    }
}

std::tuple<int&, int*> const Str_Mesh::get_an_element(const int& ind) {
    return std::make_tuple(std::ref(element_numbers_[ind]), &elements_[ind * (nnode_ + 1)]);
}

size_t Str_Mesh::number_of_nodes_max_ = 0;
