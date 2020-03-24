#pragma once
#include "mesh.hpp"
#define _CRT_SECURE_NO_WARNINGS



#if (defined(WIN32)||defined(WIN64))
#define _USE_MATH_DEFINES
//#define  sscanf    sscanf_s
#endif
#include <math.h>
void write_to_post_gid_file(Str_Mesh &pmesh);

/*
struct Str_Mesh_GiD : Str_Mesh {
	void write_to_post_gid_file(Str_Mesh&);
};
*/
