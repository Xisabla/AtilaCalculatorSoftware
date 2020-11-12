/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Mesh.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "Mesh.h"

Str_Mesh::Str_Mesh(gzFile file_msh, dataFields fields)
/*: mesh_name_(fields[1]), ndim_(std::atoi(fields[3])), element_name_(fields[5]),
  nnode_(std::atoi(fields[7]))*/
{
    Mesh::Mesh m(file_msh, fields);

    this->mesh_name_ = m.getName();
    this->ndim_ = m.getDimCount();
    this->element_name_ = m.getElementName();
    this->nnode_ = m.getNodeCount();
    number_of_nodes_max_ = Mesh::Mesh::maxNodeCount;

    this->nb_of_elements_ = m.getElementCount();
    this->tab_of_nodes_ = m.getNodes();
    m.moveElementsPtr(this->elements_);
    m.moveElementsConnectivityPtr(this->element_numbers_);

    // this->elements_ = m.getElements();
    // this->element_numbers_ = m.getElementNumbers();

    /**
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
     **/
}

std::tuple<int&, int*> const Str_Mesh::get_an_element(const int& ind) {
    return std::make_tuple(std::ref(element_numbers_[ind]), &elements_[ind * (nnode_ + 1)]);
}

size_t Str_Mesh::number_of_nodes_max_ = 0;

// - Work In Progress ------------------------------------------------------------------------------

//  --------------------------------------------------------------------------------------
//  MESH > NODE
//  --------------------------------------------------------------------------------------

Mesh::Node::Node(unsigned int id, float* coord): id(id) {
    this->coord[0] = coord[0];
    this->coord[1] = coord[1];
    this->coord[2] = coord[2];
};

Mesh::Node::Node(unsigned int id, float x, float y, float z): id(id) {
    this->coord[0] = x;
    this->coord[1] = y;
    this->coord[2] = z;
}

//  --------------------------------------------------------------------------------------
//  MESH > NODE > GETTERS
//  --------------------------------------------------------------------------------------

const int Mesh::Node::getId() { return this->id; }
const float* Mesh::Node::getCoords() { return this->coord; }
const float Mesh::Node::getX() { return this->coord[0]; }
const float Mesh::Node::getY() { return this->coord[1]; }
const float Mesh::Node::getZ() { return this->coord[2]; }

//  --------------------------------------------------------------------------------------
//  MESH > MESH
//  --------------------------------------------------------------------------------------

size_t Mesh::Mesh::maxNodeCount = 0;

Mesh::Mesh::Mesh(gzFile file, dataFields(fields))
: name(fields[1]), dimCount(std::atoi(fields[3])), elementName(fields[5]),
  nodeCount(std::atoi(fields[7])) {
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

    if (getFields(file, buffer, fields) == 0)
        __THROW__("Cannot read binary file: read size is not the expected size");

    if (!strcmp(fields[0], "Coordinates")) {
        count = node_number;
        while (count) {
            status = gzread(file, &node_number, sizeof(int));
            if (status == 0)
                __THROW__("Cannot read binary file: an error has occurred while reading the node "
                          "coordinates");

            if (node_number != count) {
                status = gzread(file, buffer, node_number);
                if (status == 0)
                    __THROW__("Cannot read binary file: an error has occurred while reading the "
                              "node coordinates");
                if (!strcmp(buffer, "End Coordinates")) {
                    node_number = count;
                    break;
                }
            }
            ++count;
            status = gzread(file, &coord, sizeof(coord));
            if (status == 0)
                __THROW__("Cannot read binary file: an error has occurred while reading the node "
                          "coordinates");
            nodes.emplace_back(node_number, coord);
        }
        maxNodeCount = std::max(maxNodeCount, nodes.size());
    }

    if (getFields(file, buffer, fields) == 0)
        __THROW__("Cannot read binary file: an error has occurred while reading the fields");
    if (!strcmp(fields[0], "Elements")) {
        const int nb_nodes = nodeCount + 1;
        //		const int nb_nodes = nnode_;
        const int size_of_an_element = (nb_nodes) * sizeof(int);
        std::unique_ptr<int[]> element_numbers { new int[maxNodeCount] };
        std::unique_ptr<int[]> elements { new int[nb_nodes * maxNodeCount] };
        int shift_nodes = 0;
        int shift_elements = 0;
        static int count = 1;
        while (count) {
            status = gzread(file, &element_number, sizeof(int));
            if (status == 0)
                __THROW__(
                "Cannot read binary file: an error has occurred while reading the elements");
            if (element_number != count) {
                status = gzread(file, buffer, element_number);
                if (!strcmp(buffer, "End Elements")) {
                    element_number = count;
                    break;
                }
            }
            ++count;
            element_numbers[shift_nodes++] = element_number;
            status = gzread(file, &elements[shift_elements], size_of_an_element);
            shift_elements += nb_nodes;
            //			elements.emplace_back(element_number, node_numbers);
            if (status == 0)
                __THROW__(
                "Cannot read binary file: an error has occurred while reading the elements");
        }

        this->elementsConnectivity = std::make_unique<int[]>(shift_nodes);
        this->elements = std::make_unique<int[]>(shift_elements);
        memcpy(this->elementsConnectivity.get(), element_numbers.get(), shift_nodes * sizeof(int));
        memcpy(this->elements.get(), elements.get(), shift_elements * sizeof(int));
        this->elementCount = shift_nodes;
    }
}

//  --------------------------------------------------------------------------------------
//  MESH > MESH > GETTERS
//  --------------------------------------------------------------------------------------

const std::string Mesh::Mesh::getName() { return this->name; }
const std::string Mesh::Mesh::getElementName() { return this->elementName; }
const GiD_ElementType Mesh::Mesh::getElementType() { return this->elementType; }

const int Mesh::Mesh::getDimCount() { return this->dimCount; }
const int Mesh::Mesh::getNodeCount() { return this->nodeCount; }
const int Mesh::Mesh::getElementCount() { return this->elementCount; }

std::vector<Str_node> Mesh::Mesh::getNodes() { return this->nodes; }

void Mesh::Mesh::moveElementsPtr(std::unique_ptr<int[]>& ptr) { ptr = std::move(this->elements); }
void Mesh::Mesh::moveElementsConnectivityPtr(std::unique_ptr<int[]>& ptr) {
    ptr = std::move(this->elementsConnectivity);
}

//  --------------------------------------------------------------------------------------
//  MESH > MESH > METHODS
//  --------------------------------------------------------------------------------------

const std::tuple<int&, int*> Mesh::Mesh::getElement(const int& id) {
    return std::make_tuple(std::ref(elementsConnectivity[id]), &elements[id * (nodeCount + 1)]);
}
/*const std::tuple<int &, int *> Mesh::Mesh::getElement(const int& id) {

}*/