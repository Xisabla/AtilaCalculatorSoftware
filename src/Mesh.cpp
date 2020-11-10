/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Mesh.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "Mesh.h"

//  --------------------------------------------------------------------------------------
//  STRUCT CONSTRUCTORS
//  --------------------------------------------------------------------------------------

size_t Mesh::Mesh::maxNodeCount = 0;

Mesh::Node::Node(int id, float* coord): id(id), x(coord[0]), y(coord[1]), z(coord[2]) { }

Mesh::Node::Node(int id, float x, float y, float z): id(id), x(x), y(y), z(z) { }

// TODO: Refactor
Mesh::Mesh::Mesh(gzFile file, char (*fields)[40])
: name(fields[1]), dimCount(std::atoi(fields[3])), elemType(getGiDElementType(fields[5])),
  nodeCount(std::atoi(fields[7])) {
    // Check for wrong formatted fields
    if (strcmp("dimension", fields[2])) __THROW__("Cannot read binary file");
    if (strcmp("ElemType", fields[4])) __THROW__("Cannot read binary file");
    if (strcmp("Nnode", fields[4])) __THROW__("Cannot read binary file");

    float coord[3];
    char readResult[FIELDS_READ_SIZE];
    int localNodeCount = 1;
    int localElemCount = 1;
    int count, status;

    if (getFields(file, readResult, fields) == 0) __THROW__("Cannot read binary file");

    if (!strcmp(fields[0], "Coordinates")) {
        count = localNodeCount;

        while (count) {
            status = gzread(file, &localNodeCount, sizeof(int));
            if (status == 0) __THROW__("Cannot read binary file");

            if (localNodeCount != count) {
                status = gzread(file, readResult, localNodeCount);
                if (status == 0) __THROW__("Cannot read binary file");

                if (!strcmp(readResult, "End Coordinates")) {
                    localNodeCount = count;
                    break;
                }
            }

            ++count;

            status = gzread(file, &coord, sizeof(coord));
            if (status == 0) __THROW__("Cannot read binary file");

            nodes.emplace_back(localNodeCount, coord);
        }
    }

    if (getFields(file, readResult, fields) == 0) __THROW__("Cannot read binary file");

    if (!strcmp(fields[0], "Elements")) {
        const int nbNodes = nodeCount + 1;
        const int elemSize = nbNodes * sizeof(int);

        std::unique_ptr<int[]> elemNumbers { new int[maxNodeCount] };
        std::unique_ptr<int[]> elems { new int[nbNodes * maxNodeCount] };

        int shiftNodes = 0;
        int shiftElems = 0;
        static int count = 1;

        while (count) {
            status = gzread(file, &localElemCount, sizeof(int));
            if (status == 0) __THROW__("Cannot read binary file");

            if (localElemCount != count) {
                status = gzread(file, &localElemCount, sizeof(int));
                if (!strcmp(readResult, "End Elements")) {
                    localElemCount = count;
                    break;
                }
            }

            ++count;
            elemNumbers[shiftNodes++] = localElemCount;
            status = gzread(file, &elems[shiftElems], elemSize);
            shiftElems += nbNodes;

            if (status == 0) __THROW__("Cannot read binary file")
        }

        this->elemConnectivity = std::make_unique<int[]>(shiftNodes);
        this->elements = std::make_unique<int[]>(shiftElems);
        memcpy(this->elemConnectivity.get(), elemNumbers.get(), shiftNodes * sizeof(int));
        memcpy(this->elements.get(), elems.get(), shiftElems * sizeof(int));
        this->elemCount = shiftNodes;
    }
}

//  --------------------------------------------------------------------------------------
//  METHODS
//  --------------------------------------------------------------------------------------

// TODO: Refactor
int Mesh::getFields(gzFile file, char* readResult, char (*fields)[40]) {
    unsigned int size = 0;

    // TODO: Init ptr and make a do while instead of while do
    GZ_READ(file, &size, 1);
    GZ_READ(file, readResult, size);

    char* ptr = strtok(readResult, " ");

    while ((ptr[0] == '#' || !strcmp(ptr, "Unit") || size == 1) && size != 0) {
        GZ_READ(file, &size, 1);
        GZ_READ(file, readResult, size);
        ptr = strtok(readResult, " ");
    }

    if (!size) __THROW__("No more data")

    size = 0;

    while (ptr != NULL) {
        if (ptr[0] == '"') {
            strcpy(fields[size], ptr);

            while (ptr[strlen(ptr) - 1] != '"') {
                ptr = strtok(NULL, " ");
                strcat(fields[size], ptr);
            }

            size_t len = strlen(fields[size]);
            for (size_t i = 0; i < len - 2; ++i) {
                fields[size][i] = fields[size][i + 1];
            }

            fields[size][len - 2] = '\0';
            ++size;
        } else {
            strcpy(fields[size++], ptr);
        }

        ptr = strtok(NULL, " ");
    }

    return size;
}

GiD_ElementType Mesh::getGiDElementType(const char* elem) {
    std::string elemStr(elem);
    auto it = GiD_StrElementTypeMap.find(elemStr);

    if (it != GiD_StrElementTypeMap.end()) {
        return GiD_StrElementTypeMap.at(elemStr);
    } else {
        __THROW__("Cannot find the type of the element: " + elem)
    }
}

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
//  --------------------------------------------------------------------------------------
//  DEBUG METHODS
//  --------------------------------------------------------------------------------------