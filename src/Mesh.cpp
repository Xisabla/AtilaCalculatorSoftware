/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      Mesh.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "Mesh.h"

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

const unsigned int Mesh::Node::getId() { return this->id; }

const float* Mesh::Node::getCoords() { return this->coord; }
const float Mesh::Node::getX() { return this->coord[0]; }
const float Mesh::Node::getY() { return this->coord[1]; }
const float Mesh::Node::getZ() { return this->coord[2]; }

//  --------------------------------------------------------------------------------------
//  MESH > MESH
//  --------------------------------------------------------------------------------------

Mesh::Mesh::Mesh(gzFile file, dataFields(fields)) {
    // Check if the header is fine
    if (strcmp("dimension", fields[2]))
        __THROW__("Cannot read binary file: fields[2], expected \"dimensions\", got \"" +
                  fields[2] + "\"");
    if (strcmp("ElemType", fields[4]))
        __THROW__("Cannot read binary file: fields[4], expected \"ElemType\", got \"" + fields[4] +
                  "\"");
    if (strcmp("Nnode", fields[6]))
        __THROW__("Cannot read binary file: fields[6], expected \"Nnode\", got \"" + fields[6] +
                  "\"");

    // Import header data
    this->name = fields[1];
    this->dimCount = std::atoi(fields[3]);
    this->elementName = fields[5];
    this->nodeCount = std::atoi(fields[7]);

    char buffer[GZ_BUFFER_SIZE];

    // Continue reading
    if (getFields(file, buffer, fields) == 0)
        __THROW__("Cannot read binary file: read size is not the expected size");

    // Read coordinates
    if (!strcmp(fields[0], "Coordinates")) this->readCoordinates(file, buffer);

    if (getFields(file, buffer, fields) == 0)
        __THROW__("Cannot read binary file: an error has occurred while reading the fields");

    // Read Elements
    if (!strcmp(fields[0], "Elements")) this->readElements(file, buffer);
}

size_t Mesh::Mesh::maxNodeCount = 0;

const std::map<std::string, GiD_ElementType> Mesh::Mesh::GiD_ElementTypeEncoding = {
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
    { "Circle", GiD_NoElement }
};

//  --------------------------------------------------------------------------------------
//  MESH > MESH > GETTERS
//  --------------------------------------------------------------------------------------

const std::string Mesh::Mesh::getName() { return this->name; }
const std::string Mesh::Mesh::getElementName() { return this->elementName; }
const GiD_ElementType Mesh::Mesh::getElementType() { return this->elementType; }

const unsigned int Mesh::Mesh::getDimCount() { return this->dimCount; }
const unsigned int Mesh::Mesh::getNodeCount() { return this->nodeCount; }
const unsigned int Mesh::Mesh::getElementCount() { return this->elementCount; }

std::vector<Mesh::Node> Mesh::Mesh::getNodes() { return this->nodes; }

//  --------------------------------------------------------------------------------------
//  MESH > MESH > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

const std::tuple<int&, int*> Mesh::Mesh::getElement(const int& id) {
    return std::make_tuple(std::ref(elementsConnectivity[id]), &elements[id * (nodeCount + 1)]);
}

const int Mesh::Mesh::toPostGid() {
    GiD_BeginMesh(name.c_str(), DIM(dimCount), elementType, nodeCount);

    // Write Coordinates
    GiD_BeginCoordinates();
    for(Node &node: nodes) GiD_WriteCoordinates(node.getId(), node.getX(), node.getY(), node.getZ());
    GiD_EndCoordinates();

    // Write Elements
    GiD_BeginElements();
    for(unsigned int i = 0; i < elementCount; i++) {
        auto [id, nid] = getElement(i);
        GiD_WriteElement(id, nid);
    }
    GiD_EndElements();

    return GiD_EndMesh();
}

const GiD_ElementType Mesh::Mesh::getGiDElementType(const char* element) {
    std::string elementString(element);
    auto it = GiD_ElementTypeEncoding.find(elementString);

    if(it != GiD_ElementTypeEncoding.end()) return GiD_ElementTypeEncoding.at(elementString);

    __THROW__("Cannot find the type of the given element: " + element);
}

//  --------------------------------------------------------------------------------------
//  MESH > MESH > PRIVATE METHODS
//  --------------------------------------------------------------------------------------

void Mesh::Mesh::readCoordinates(gzFile& file, char* buffer) {
    unsigned int nodeId = 1;
    unsigned int count = 1;

    float coord[3];

    while (count) {
        // Read nodeId
        if (gzread(file, &nodeId, sizeof(int)) == 0)
            __THROW__(
            "Cannot read binary file: an error has occurred while reading the node coordinates");

        if (nodeId != count) {
            if (gzread(file, buffer, nodeId) == 0)
                __THROW__("Cannot read binary file: an error has occurred while reading the node "
                          "coordinates");

            // Reached end of coordinates reading
            if (!strcmp(buffer, "End Coordinates")) {
                nodeId = count;
                break;
            }
        }

        ++count;

        // Read node coordinates
        if (gzread(file, &coord, sizeof(coord)) == 0)
            __THROW__("Cannot read binary file: an error has occurred while reading the node "
                      "coordinates");

        // Append node to Mesh nodes
        nodes.emplace_back(nodeId, coord);
    }

    maxNodeCount = std::max(maxNodeCount, nodes.size());
}

void Mesh::Mesh::readElements(gzFile& file, char* buffer) {
    const unsigned int n = nodeCount + 1;
    const unsigned int elemSize = n * sizeof(int);
    unsigned int elementCount = 1;
    unsigned int count = 1;

    const std::unique_ptr<int[]> connectivity { new int[maxNodeCount] };
    const std::unique_ptr<int[]> elements { new int[n * maxNodeCount] };

    unsigned int nodeShift = 0;
    unsigned int elementsShift = 0;

    while (count) {
        // Read elementCount
        if (gzread(file, &elementCount, sizeof(int)) == 0)
            __THROW__("Cannot read binary file: an error has occurred while reading the elements");

        if (elementCount != count) {
            if (gzread(file, buffer, elementCount) == 0)
                __THROW__(
                "Cannot read binary file: an error has occurred while reading the elements");

            // Reached end of elements
            if (!strcmp(buffer, "End Elements")) {
                elementCount = count;
                break;
            }
        }

        ++count;
        connectivity[nodeShift++] = elementCount;

        // Read element
        if (gzread(file, &elements[elementsShift], elemSize) == 0)
            __THROW__("Cannot read binary file: an error has occurred while reading the elements");

        elementsShift += n;
    }

    this->elementsConnectivity = std::make_unique<int[]>(nodeShift);
    this->elements = std::make_unique<int[]>(elementsShift);
    memcpy(this->elementsConnectivity.get(), connectivity.get(), nodeShift * sizeof(int));
    memcpy(this->elements.get(), elements.get(), elementsShift * sizeof(int));
    this->elementCount = nodeShift;
}

//  --------------------------------------------------------------------------------------
//  MESH > METHODS
//  --------------------------------------------------------------------------------------

const unsigned int Mesh::getFields(gzFile file, char* buffer, dataFields fields) {
    unsigned int size = 0;

    GZ_READ_CHECK(file, &size, 1);
    GZ_READ_CHECK(file, buffer, size);

    char* ptr = strtok(buffer, " ");

    while ((ptr[0] == '#' || !strcmp(ptr, "Unit") || size == 1) && size != 0) {
        GZ_READ_CHECK(file, &size, 1);
        GZ_READ_CHECK(file, buffer, size);
        ptr = strtok(buffer, " ");
    }

    if (!size) __THROW__("No more data to read");

    size = 0;
    while (ptr != NULL) {
        if (ptr[0] == '"') {
            // Set current field
            strcpy(fields[size], ptr);

            while (ptr[strlen(ptr) - 1] != '"') {
                ptr = strtok(NULL, " ");
                strcat(fields[size], ptr);
            }

            size_t len = strlen(fields[size]);

            // Shift fields
            for (size_t i = 0; i < len - 2; ++i) {
                fields[size][i] = fields[size][i + 1];
            }

            // Close field
            fields[size][len - 2] = '\0';
            ++size;
        } else {
            // Set next field
            strcpy(fields[size++], ptr);
        }

        ptr = strtok(NULL, " ");
    }

    return size;
}
