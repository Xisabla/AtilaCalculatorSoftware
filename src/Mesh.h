/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Mesh.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_MESH_H
#define ATILACALCULATORSOFTWARE_MESH_H

#include "Utils.h"

#include <algorithm>
#include <cstring>
#include <gidpost.h>
#include <math.h>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <zlib.h>

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
    /*const*/ std::string mesh_name_;
    /*const*/ int ndim_;
    /*const*/ std::string element_name_;
    /*const*/ int nnode_;
    static size_t number_of_nodes_max_;

    int nb_of_elements_ = 0;
    std::vector<Str_node> tab_of_nodes_;
    std::unique_ptr<int[]> elements_;
    std::unique_ptr<int[]> element_numbers_;
    Str_Mesh(gzFile file_msh, dataFields fields);
    std::tuple<int&, int*> const get_an_element(const int& ind);
};

int getFields(gzFile file, char* buffer, dataFields fields);

// - Work In Progress ------------------------------------------------------------------------------

namespace Mesh {

/**
 * @class Node
 * @brief Point in 3D space representation, part of a Mesh
 * @see Mesh
 */
class Node {
  public:
    /**
     * @param id ID of the current Node
     * @param coord Space coordinates of the node
     */
    Node(unsigned int id, float* coord);

    /**
     * @param id ID of the current Node
     * @param x X component of the Space coordinates of the node
     * @param y Y component of the Space coordinates of the node
     * @param z Z component of the Space coordinates of the node
     */
    Node(unsigned int id, float x, float y, float z);

    /**
     * @return The ID of the node
     */
    const int getId();

    /**
     * @return The pointer of the space coordinates of the node
     */
    const float* getCoords();

    /**
     * @return The X space coordinates component of the node
     */
    const float getX();

    /**
     * @return The Y space coordinates component of the node
     */
    const float getY();

    /**
     * @return The Z space coordinates component of the node
     */
    const float getZ();

  private:
    /**
     * @brief ID of the Node
     */
    const unsigned int id;

    /**
     * @brief Space coordinates of the node
     */
    float coord[3];

    /**
     * @brief Memory size of the space coordinates of the node
     */
    static const size_t coordSize = 3 * sizeof(float);
};

/**
 * @class Mesh
 * @brief Representation of a mesh, all of its nodes and elements
 */
class Mesh {
  public:
    /**
     * @param file File that contains the mesh information
     * @param fields Base fields read from the file
     */
    Mesh(gzFile file, dataFields fields);

    const std::string getName();
    const std::string getElementName();
    const GiD_ElementType getElementType();

    const int getDimCount();
    const int getNodeCount();
    const int getElementCount();

    std::vector<Str_node> getNodes();

    // NOTE: Temporarily:
    void moveElementsPtr(std::unique_ptr<int[]>& ptr);
    void moveElementsConnectivityPtr(std::unique_ptr<int[]>& ptr);

    /**
     * @param id ID of the element
     * @return Return information about an element:
     */
    const std::tuple<int&, int*> getElement(const int& id);

    /**
     * @brief Biggest number of node encountered in the mesh
     */
    static size_t maxNodeCount;

  private:
    /**
     * @brief Name of the mesh
     */
    std::string name;

    /**
     * @brief String encoded element type of the mesh
     */
    std::string elementName;

    /**
     * @brief Element type of the mesh
     */
    GiD_ElementType elementType;

    /**
     * @brief Number of dimensions of the object (2 or 3)
     */
    int dimCount;

    /**
     * @brief Number of nodes which constitutes the mesh
     */
    int nodeCount;

    /**
     * @brief Number of elements in the mesh
     */
    int elementCount;

    /**
     * @brief Nodes that constitutes the mesh
     */
    std::vector<Str_node> nodes;

    /**
     * @brief Elements of the mesh
     */
    std::unique_ptr<int[]> elements;

    /**
     * @brief Connectivity of the elements
     */
    std::unique_ptr<int[]> elementsConnectivity;
};

} // namespace Mesh

#endif // ATILACALCULATORSOFTWARE_MESH_H
