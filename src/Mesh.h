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
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <zlib.h>

#define DIM(x) x == 3 ? GiD_3D : GiD_2D

namespace Mesh {

//  --------------------------------------------------------------------------------------
//  NODE
//  --------------------------------------------------------------------------------------

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
    const unsigned int getId();

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
};

//  --------------------------------------------------------------------------------------
//  MESH
//  --------------------------------------------------------------------------------------

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

    /**
     * @return The name of the name
     */
    const std::string getName();

    /**
     * @return The string encoded element type of the mesh
     */
    const std::string getElementName();

    /**
     * @return The Element type of the mesh
     */
    const GiD_ElementType getElementType();

    /**
     * @return The number of dimension of the mesh (2 or 3)
     */
    const unsigned int getDimCount();

    /**
     * @return The number of nodes that constitutes the mesh
     */
    const unsigned int getNodeCount();

    /**
     * @return The number of elements in the mesh
     */
    const unsigned int getElementCount();

    /**
     * @return The nodes that constitutes the mesh
     */
    std::vector<Node> getNodes();

    /**
     * @param id ID of the element
     * @return Return information about an element:
     */
    const std::tuple<int&, int*> getElement(const int& id);

    /**
     * @brief Write the mesh into a currently open PostResultFile
     * @return The state of the GiD Post Mesh closure
     */
    const int toPostGid();

    /**
     * @brief Get the GiD_ElementType of a string encoded GiDElementType
     * @param element string encoded GiD_ElementType
     * @return The according GiD_ElementType, GiD_NoElement by default
     * @throw runtime_error if the encoded GiD_ElementType is not recognized
     */
    const static GiD_ElementType getGiDElementType(const char* element);

    /**
     * @brief Biggest number of node encountered in the mesh
     */
    static size_t maxNodeCount;

    /**
     * @brief Map of all recognized GiD_ElementType according to their string encoded value
     */
    const static std::map<std::string, GiD_ElementType> GiD_ElementTypeEncoding;

  private:
    /**
     * Read the nodes coordinates from the mesh file
     * @param file File that contains the mesh information
     * @param buffer The zlib reading buffer from previous gzread
     */
    void readCoordinates(gzFile& file, char buffer[GZ_BUFFER_SIZE]);

    /**
     * Read the elements from the mesh file
     * @param file File that contains the mesh information
     * @param buffer The zlib reading buffer from previous gzread
     */
    void readElements(gzFile& file, char buffer[GZ_BUFFER_SIZE]);

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
    unsigned int dimCount;

    /**
     * @brief Number of nodes which constitutes the mesh
     */
    unsigned int nodeCount;

    /**
     * @brief Number of elements in the mesh
     */
    unsigned int elementCount;

    /**
     * @brief Nodes that constitutes the mesh
     */
    std::vector<Node> nodes;

    /**
     * @brief Elements of the mesh
     */
    int* elements;

    /**
     * @brief Connectivity of the elements
     */
    int* elementsConnectivity;
};

//  --------------------------------------------------------------------------------------
//  METHODS
//  --------------------------------------------------------------------------------------

const unsigned int getFields(gzFile file, char* buffer, dataFields fields);

} // namespace Mesh

#endif // ATILACALCULATORSOFTWARE_MESH_H
