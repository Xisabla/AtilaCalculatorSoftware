/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Mesh.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "Mesh.h"

Mesh::Node::Node(int id, double* coord): id(id), x(coord[0]), y(coord[1]), z(coord[2]) { }

Mesh::Node::Node(int id, double x, double y, double z): id(id), x(x), y(y), z(z) { }

int Mesh::toPostGid(Mesh& mesh) {
    GiD_BeginMesh(mesh.name, DIM(mesh.dimCount), mesh.elemType, mesh.nodeCount);

    // Write Coordinates
    GiD_BeginCoordinates();
    for (const auto& node: mesh.nodes) {
        GiD_WriteCoordinates(node.id, node.x, node.y, node.z);
    }
    GiD_EndCoordinates();

    // Write Elements
    GiD_BeginElements();
    for (int i = 0; i < mesh.elemCount; i++) {
        int& id = std::ref(mesh.elements[id]);
        int* nid = &mesh.elemConnectivity[i * (mesh.nodeCount + 1)];

        GiD_WriteElementMat(id, nid);
    }
    GiD_EndElements();

    return GiD_EndMesh();
}