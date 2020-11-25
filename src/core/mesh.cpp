/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      mesh.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "core/mesh.h"

//  --------------------------------------------------------------------------------------
//  NODE
//  --------------------------------------------------------------------------------------

Node::Node(unsigned int id, const float* coord): id(id) {
    this->coord[0] = coord[0];
    this->coord[1] = coord[1];
    this->coord[2] = coord[2];
}

Node::Node(unsigned int id, float x, float y, float z): id(id) {
    this->coord[0] = x;
    this->coord[1] = y;
    this->coord[2] = z;
}

//  --------------------------------------------------------------------------------------
//  NODE > GETTERS
//  --------------------------------------------------------------------------------------

unsigned int Node::getId() const { return this->id; }

const float* Node::getCoords() { return this->coord; }
float Node::getX() { return this->coord[0]; }
float Node::getY() { return this->coord[1]; }
float Node::getZ() { return this->coord[2]; }

//  --------------------------------------------------------------------------------------
//  MESH
//  --------------------------------------------------------------------------------------

Mesh::Mesh(gzFile file, dataFields(fields)) {
    Logger::debug("Reading mesh...");
    Logger::trace("Asserting mesh header");
    // Check if the header is fine
    if (strcmp("dimension", fields[2]) != 0)
        __THROW__("Cannot read binary file: fields[2], expected \"dimensions\", got \"" +
                  fields[2] + "\"");
    if (strcmp("ElemType", fields[4]) != 0)
        __THROW__("Cannot read binary file: fields[4], expected \"ElemType\", got \"" + fields[4] +
                  "\"");
    if (strcmp("Nnode", fields[6]) != 0)
        __THROW__("Cannot read binary file: fields[6], expected \"Nnode\", got \"" + fields[6] +
                  "\"");

    // Import header data
    this->name = fields[1];
    this->dimCount = static_cast<unsigned int>(std::atoi(fields[3]));
    this->elementName = fields[5];
    this->nodeCount = static_cast<unsigned int>(std::atoi(fields[7]));

    Logger::trace("Mesh: ", this->name, " ", this->dimCount, " dimensions (", this->nodeCount, " nodes/elements)");

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

    Logger::debug("Reading mesh: Done");
}

size_t Mesh::maxNodeCount = 0;

const std::map<std::string, GiD_ElementType> Mesh::GiD_ElementTypeEncoding = {
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
//  MESH > GETTERS
//  --------------------------------------------------------------------------------------

std::string Mesh::getName() { return this->name; }
std::string Mesh::getElementName() { return this->elementName; }
GiD_ElementType Mesh::getElementType() { return this->elementType; }

unsigned int Mesh::getDimCount() const { return this->dimCount; }
unsigned int Mesh::getNodeCount() const { return this->nodeCount; }
unsigned int Mesh::getElementCount() const { return this->elementCount; }

std::vector<Node> Mesh::Mesh::getNodes() { return this->nodes; }

//  --------------------------------------------------------------------------------------
//  MESH > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

std::tuple<int&, int*> Mesh::getElement(const unsigned int& id) const {
    return std::make_tuple(std::ref(this->elementsConnectivity[id]),
                           &this->elements[id * (this->nodeCount + 1)]);
}

// const int Mesh::toPostGid() {
//    GiD_BeginMesh(name.c_str(), DIM(dimCount), elementType, nodeCount);
//
//    // Write Coordinates
//    GiD_BeginCoordinates();
//    for (Node& node: nodes)
//        GiD_WriteCoordinates(node.getId(), node.getX(), node.getY(), node.getZ());
//    GiD_EndCoordinates();
//
//    // Write Elements
//    GiD_BeginElements();
//    for (unsigned int i = 0; i < elementCount; i++) {
//        auto [id, nid] = getElement(i);
//        GiD_WriteElement(id, nid);
//    }
//    GiD_EndElements();
//
//    return GiD_EndMesh();
//}

GiD_ElementType Mesh::getGiDElementType(const char* element) {
    std::string elementString(element);
    auto it = Mesh::GiD_ElementTypeEncoding.find(elementString);

    if (it != Mesh::GiD_ElementTypeEncoding.end())
        return Mesh::GiD_ElementTypeEncoding.at(elementString);

    Logger::fatal("Unable to find element type of type: ", element);
    __THROW__("Cannot find the type of the given element: " + element);
}

//  --------------------------------------------------------------------------------------
//  MESH > PRIVATE METHODS
//  --------------------------------------------------------------------------------------

void Mesh::readCoordinates(gzFile& file, char* buffer) {
    Logger::trace("Mesh: ", this->name, ": reading coordinates...");
    unsigned int nodeId = 1;
    unsigned int count = 1;

    float coord[3];

    while (count) {
        // Read nodeId
        if (gzread(file, &nodeId, sizeof(int)) == 0)
            __THROW__("Cannot read binary file: an error has occurred while reading node id");

        if (nodeId != count) {
            if (gzread(file, buffer, nodeId) == 0)
                __THROW__("Cannot read binary file: an error has occurred while reading the nodes");

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
        this->nodes.emplace_back(nodeId, coord);
    }

    // Update maxNodeCount
    Mesh::maxNodeCount = std::max(Mesh::maxNodeCount, this->nodes.size());
    Logger::trace("Mesh: ", this->name, ": reading coordinates: Done - ", this->nodes.size(), " nodes read");
}

void Mesh::readElements(gzFile& file, char* buffer) {
    Logger::trace("Mesh: ", this->name, ": reading elements...");
    const unsigned int n = nodeCount + 1;
    const unsigned int elemSize = n * sizeof(int);
    unsigned int elementCount = 1;
    unsigned int count = 1;

    int* connectivity { new int[Mesh::maxNodeCount] };
    int* elements { new int[n * Mesh::maxNodeCount] };

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
        connectivity[nodeShift++] = static_cast<int>(elementCount);

        // Read element
        if (gzread(file, &elements[elementsShift], elemSize) == 0)
            __THROW__("Cannot read binary file: an error has occurred while reading the elements");

        elementsShift += n;
    }

    this->elementsConnectivity = connectivity;
    this->elements = elements;
    this->elementCount = nodeShift;

    Logger::trace("Mesh: ", this->name, ": reading elements: Done - ", elementCount, " elements");
}