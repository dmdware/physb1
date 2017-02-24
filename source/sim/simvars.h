

#ifndef SIMVARS_H
#define SIMVARS_H

#include "../math/vec3f.h"
#include "../math/vec4f.h"
#include "../math/3dmath.h"
#include "../render/hypmesh.h"
#include "simdef.h"

#define MESHES	2

#define MESH_E	1
#define MESH_S	0

#define GRID	8
#define GRIDIS		(2<<(HYP_DIMS*3))
//#define GRIDIS		(1)

	//for(int gi=0; gi<pow(GRID,HYP_DIM); ++gi)
	//for(int gi=0; gi<GRIDIS; ++gi)

//extern HypMeshf g[MESHES];
extern HypMeshf axg[HYP_DIMS*2];
//extern HypMeshf gg[GRIDIS];
extern std::list<HypVecf> hh;
extern int selgg;
extern int selg;

extern HypMeshf gv;
//extern HypMeshf gve;
//extern HypMeshf gvs;

#define GVX	134
#define GVY	456
#define GVZ	343
#define GVW	100

#endif
