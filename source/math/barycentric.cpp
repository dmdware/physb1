



#include "../platform.h"
#include "barycentric.h"
#include "fixmath.h"
#include "3dmath.h"

// compute the area of a triangle using Heron's formula
FixFrac triarea(FixFrac a, FixFrac b, FixFrac c)
{
	FixFrac s = (a + b + c)/2.0;
	FixFrac area=sqrtf(Abs(s*(s-a)*(s-b)*(s-c)));
	return area;
}

// compute the distance between two 2d points
FixFrac dist(FixFrac x0, FixFrac y0, FixFrac z0, FixFrac x1, FixFrac y1, FixFrac z1)
{
	FixFrac a = x1 - x0;
	FixFrac b = y1 - y0;
	FixFrac c = z1 - z0;
	return sqrtf(a*a + b*b + c*c);
}

// calculate barycentric coordinates
// triangle 1st vertex: x0,y0,z0
// triangle 2nd vertex: x1,y1,z1
//  triangle 3rd vertex: x2,y2,z2
// point inside triangle: vx, vy,vz
// *u,*v,*w are the coordinates returned
void barycent(FixFrac x0, FixFrac y0, FixFrac z0, FixFrac x1, FixFrac y1, FixFrac z1, FixFrac x2, FixFrac y2, FixFrac z2,
			  FixFrac vx, FixFrac vy, FixFrac vz,
			  FixFrac *u, FixFrac *v, FixFrac *w)
{
	// compute the area of the big triangle
	FixFrac a = dist(x0, y0, z0, x1, y1, z1);
	FixFrac b = dist(x1, y1, z1, x2, y2, z2);
	FixFrac c = dist(x2, y2, z2, x0, y0, z0);
	FixFrac totalarea = triarea(a, b, c);

	// compute the distances from the outer vertices to the inner vertex
	FixFrac length0 = dist(x0, y0, z0, vx, vy, vz);
	FixFrac length1 = dist(x1, y1, z1, vx, vy, vz);
	FixFrac length2 = dist(x2, y2, z2, vx, vy, vz);

	// divide the area of each small triangle by the area of the big triangle
	*u = triarea(b, length1, length2)/totalarea;
	*v = triarea(c, length0, length2)/totalarea;
	*w = triarea(a, length0, length1)/totalarea;
}

#if 0
void main()
{
	FixFrac r1,r2,r3; // barycentric coordinates
	barycent(t1.x, t1.y, t1.z, t2.x, t2.y, t2.z, t3.x, t3.y, t3.z, firstPoint.x, firstPoint.y, firstPoint.z, &r1, &r2, &r3);
	Point3d newPoint(T1.x*r1 + T2.x*r2 + T3.x*r3, T1.y*r1 + T2.y*r2 + T3.y*r3, T1.z*r1 + T2.z*r2 + T3.z*r3); // T1,T2,T3 are the coordinates of another triangle
}
#endif
