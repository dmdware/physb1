



#ifndef BARYCENTRIC_H
#define BARYCENTRIC_H

#include "fixfrac.h"

// compute the area of a triangle using Heron's formula
FixFrac triarea(FixFrac a, FixFrac b, FixFrac c);

// compute the distance between two 2d points
FixFrac dist(FixFrac x0, FixFrac y0, FixFrac z0, FixFrac x1, FixFrac y1, FixFrac z1);

// calculate barycentric coordinates
// triangle 1st vertex: x0,y0,z0
// triangle 2nd vertex: x1,y1,z1
//  triangle 3rd vertex: x2,y2,z2
// point inside triangle: vx, vy,vz
// *u,*v,*w are the coordinates returned
void barycent(FixFrac x0, FixFrac y0, FixFrac z0, FixFrac x1, FixFrac y1, FixFrac z1, FixFrac x2, FixFrac y2, FixFrac z2,
			  FixFrac vx, FixFrac vy, FixFrac vz,
			  FixFrac *u, FixFrac *v, FixFrac *w);

#endif
