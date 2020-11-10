/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Mesh.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_MESH_H
#define ATILACALCULATORSOFTWARE_MESH_H

#include "gidpost.h"
#include "zlib.h"
#include <algorithm>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <vector>
// TODO: Debug data with some print methods
#include <iostream>

#define DIM(x) x == 3 ? GiD_3D : GiD_2D
// TODO: Move to something like utils.h ---
#include <string> // Note: this is just to remember that we need the string lib
#define __HERE__ std::string(__FILE__) + ":" + std::to_string(__LINE__)
#define __THROW__(x) throw std::runtime_error(std::string("[" + __HERE__ + "] " + x));
// ---

#if (defined(WIN32) || defined(WIN64))
#define _USE_MATH_DEFINES
#endif

#define GZ_READ(FILE, DATA, SIZE)                       \
    if (static_cast<int>((SIZE) * sizeof(*(DATA))) !=   \
        gzread((FILE), (DATA), SIZE * sizeof(*(DATA)))) \
        return (-1);

#define FIELDS_READ_SIZE 2000

/**
 * @brief Generic Mesh objects used with GiD
 */
namespace Mesh {

//  --------------------------------------------------------------------------------------
//  STRUCT DEFINITIONS
//  --------------------------------------------------------------------------------------
struct Node {
    int id;
    float x;
    float y;
    float z;

    Node(int id, float coord[3]);
    Node(int id, float x, float y, float z);
};

struct Mesh {
    const char* name;
    const GiD_ElementType elemType;
    const int dimCount;
    const int nodeCount;
    int elemCount;
    static size_t maxNodeCount;

    std::vector<Node> nodes;
    std::unique_ptr<int[]> elements;
    std::unique_ptr<int[]> elemConnectivity;

    Mesh(gzFile file, char fields[10][40]);
};

//  --------------------------------------------------------------------------------------
//  METHODS
//  --------------------------------------------------------------------------------------

/**
 * @brief Read the fields from a compressed file
 * @param file Compressed field to read
 * @param readResult Buffer that will contain the uncompressed data from the file
 * @param fields Pointer of fields
 * @return The number of read elements
 */
int getFields(gzFile file, char readResult[FIELDS_READ_SIZE], char fields[10][40]);

/**
 * @brief Map of all recognized GiDElementType according to their String encoding
 */
const static std::map<std::string, GiD_ElementType> GiD_StrElementTypeMap = {
    { "", GiD_NoElement },
    { "Point", GiD_NoElement },
    { "Linear", GiD_NoElement },
    { "Triangle", GiD_Triangle },
    { "Quadrilateral", GiD_Quadrilateral },
    { "Tetrahedra", GiD_Tetrahedra },
    { "Hexahedra", GiD_Hexahedra },
    { "Prism", GiD_Prism },
    { "Pyramid", GiD_NoElement },
    { "Sphere", GiD_NoElement },
    { "Circle", GiD_NoElement },
};

/**
 * @brief Get the GiDElementType of a String encoded GiDElementType
 * @param elem String encoded GiDElementType
 * @return The according GiDElementType, GiD_NoElement by default
 * @throw runtime_error if the encoded GiDElementType is not recognized
 */
GiD_ElementType getGiDElementType(const char* elem);

/**
 * Write a string mesh into a previously opened PostResultFile
 * @param mesh Mesh to write
 * @return The state of the GiD Post Mesh closure
 */
int toPostGid(Mesh& mesh);

//  --------------------------------------------------------------------------------------
//  DEBUG METHODS
//  --------------------------------------------------------------------------------------

} // namespace Mesh

#endif // ATILACALCULATORSOFTWARE_MESH_H