/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      MeshGiD.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_MESHGID_H
#define ATILACALCULATORSOFTWARE_MESHGID_H

#include "Mesh.h"

#include <gidpost.h>
#include <math.h>

#define _CRT_SECURE_NO_WARNINGS

#if (defined(WIN32) || defined(WIN64))
#define _USE_MATH_DEFINES
#endif

void write_to_post_gid_file(Str_Mesh& pmesh);

/*
struct Str_Mesh_GiD : Str_Mesh {
    void write_to_post_gid_file(Str_Mesh&);
};
*/

#endif // ATILACALCULATORSOFTWARE_MESHGID_H
