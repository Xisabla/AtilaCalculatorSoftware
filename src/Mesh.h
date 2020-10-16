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
#include <memory>
#include <string>
#include <vector>

#define DIM(a) a == 3 ? GiD_3D : GiD_2D

// #if (defined(WIN32)||defined(WIN64))
// #define _USE_MATH_DEFINES
// #endif

namespace Mesh {

struct Node {
    int id;
    double x;
    double y;
    double z;

    Node(int id, double coord[3]);
    Node(int id, double x, double y, double z);
};

struct Mesh {
    const char* name;
    const GiD_ElementType elemType;
    const int dimCount;
    const int nodeCount;
    const int elemCount;

    const std::vector<Node> nodes;
    const std::unique_ptr<int[]> elements;
    const std::unique_ptr<int[]> elemConnectivity;
};

// Node node(int id, double coord[3]);
// Node node(int id, double x, double y, double z);
// Mesh mesh(gzFile file, char fields[10][40]);

/**
 * Write a string mesh into a previously opened PostResultFile
 * @param mesh Mesh to write
 * @return The state of the GiD Post Mesh closure
 */
int toPostGid(Mesh& mesh);

} // namespace Mesh

#endif // ATILACALCULATORSOFTWARE_MESH_H