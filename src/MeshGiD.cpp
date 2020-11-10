/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      MeshGiD.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "MeshGiD.h"

/*
    elementType = giveTheTypeOfElementForGiD(elementName);
    GiD_BeginMesh(meshName, ndim == 3 ? GiD_3D : GiD_2D, elementType, nnode);
*/


static const char* strElementType[] = { "",          "Point",         "Linear",
                                        "Triangle",  "Quadrilateral", "Tetrahedra",
                                        "Hexahedra", "Prism",         "Pyramid",
                                        "Sphere",    "Circle" };

GiD_ElementType giveTheTypeOfElementForGiD(const char* elementName) {
    unsigned i;
    for (i = 0; i < sizeof(strElementType) / sizeof(char*); ++i) {
        if (!strcmp(strElementType[i], elementName)) break;
    }
    switch (i) {
        case 3:
            return GiD_Triangle;
        case 4:
            return GiD_Quadrilateral;
        case 5:
            return GiD_Tetrahedra;
        case 6:
            return GiD_Hexahedra;
        case 7:
            return GiD_Prism;
        default:
            throw("Cannot find the type of element (Mesh)");
    }
    return GiD_NoElement;
}


void write_to_post_gid_file(Str_Mesh& pmesh) {
    GiD_BeginMesh(pmesh.mesh_name_.c_str(),
                  pmesh.ndim_ == 3 ? GiD_3D : GiD_2D,
                  giveTheTypeOfElementForGiD(pmesh.element_name_.c_str()),
                  pmesh.nnode_);
    GiD_BeginCoordinates();

    for (const auto& coord: pmesh.tab_of_nodes_) {
        GiD_WriteCoordinates(coord.number, coord.coord[0], coord.coord[1], coord.coord[2]);
        //        std::cout<<coord.number<<std::endl;
    }
    GiD_EndCoordinates();
    GiD_BeginElements();
    for (auto i = 0; i < pmesh.nb_of_elements_; ++i) {
        auto [element_number, element] = pmesh.get_an_element(i);
        GiD_WriteElementMat(element_number, element);
        /*
        std::cout<<element_number;
        for (auto j=0;j<pmesh.nnode_+ 1;++j){
            std::cout<<" "<<element[j];
        }
        std::cout<<"\n";
        */
    }
    GiD_EndElements();
    GiD_EndMesh();
}