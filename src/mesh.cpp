#include "mesh.hpp"

Str_Mesh::Str_Mesh(gzFile file_msh, dataFields fields)
: mesh_name_(fields[1]), ndim_(std::atoi(fields[3])), element_name_(fields[5]),
  nnode_(std::atoi(fields[7])) {
    // Fields assertions
    if (strcmp("dimension", fields[2]))
        __THROW__("Cannot read binary file: fields[2], expected \"dimensions\", got \"" +
                  fields[2] + "\"");
    if (strcmp("ElemType", fields[4]))
        __THROW__("Cannot read binary file: fields[4], expected \"ElemType\", got \"" + fields[4] +
                  "\"");
    if (strcmp("Nnode", fields[6]))
        __THROW__("Cannot read binary file: fields[6], expected \"Nnode\", got \"" + fields[6] +
                  "\"");

    float coord[3];
    char buffer[GZ_BUFFER_SIZE];
    int node_number = 1;
    int element_number = 1;
    int count = 0;
    int status;

    if (getFields(file_msh, buffer, fields) == 0)
        __THROW__("Cannot read binary file: read size is not the expected size");

    if (!strcmp(fields[0], "Coordinates")) {
        count = node_number;
        while (count) {
            status = gzread(file_msh, &node_number, sizeof(int));
            if (status == 0)
                __THROW__("Cannot read binary file: an error has occurred while reading the node "
                          "coordinates");

            if (node_number != count) {
                status = gzread(file_msh, buffer, node_number);
                if (status == 0)
                    __THROW__("Cannot read binary file: an error has occurred while reading the "
                              "node coordinates");
                if (!strcmp(buffer, "End Coordinates")) {
                    node_number = count;
                    break;
                }
            }
            ++count;
            status = gzread(file_msh, &coord, sizeof(coord));
            if (status == 0)
                __THROW__("Cannot read binary file: an error has occurred while reading the node "
                          "coordinates");
            tab_of_nodes_.emplace_back(node_number, coord);
        }
        number_of_nodes_max_ = std::max(number_of_nodes_max_, tab_of_nodes_.size());
    }

    if (getFields(file_msh, buffer, fields) == 0)
        __THROW__("Cannot read binary file: an error has occurred while reading the fields");
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
            if (status == 0)
                __THROW__(
                "Cannot read binary file: an error has occurred while reading the elements");
            if (element_number != count) {
                status = gzread(file_msh, buffer, element_number);
                if (!strcmp(buffer, "End Elements")) {
                    element_number = count;
                    break;
                }
            }
            ++count;
            element_numbers[shift_nodes++] = element_number;
            status = gzread(file_msh, &elements[shift_elements], size_of_an_element);
            shift_elements += nb_nodes;
            //			elements.emplace_back(element_number, node_numbers);
            if (status == 0)
                __THROW__(
                "Cannot read binary file: an error has occurred while reading the elements");
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
