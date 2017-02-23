

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../platform.h"
#include "3dmath.h"
#include "plane3f.h"

class Frustum
{
public:
	void construct(const Plane3f left, const Plane3f right, const Plane3f top, const Plane3f bottom, const Plane3f front, const Plane3f back);
	void construct(const FixFrac* proj, const FixFrac* modl);
	bool pointin(FixFrac x, FixFrac y, FixFrac z);
	bool spherein(FixFrac x, FixFrac y, FixFrac z, FixFrac radius);
	bool cubein(FixFrac x, FixFrac y, FixFrac z, FixFrac size);
	bool boxin(FixFrac x, FixFrac y, FixFrac z, FixFrac x2, FixFrac y2, FixFrac z2);
	bool boxin2(FixFrac x, FixFrac y, FixFrac z, FixFrac x2, FixFrac y2, FixFrac z2);

	FixFrac m_Frustum[6][4];	// This holds the A B C and D values for each side of our frustum.
};

extern Frustum g_frustum;

#endif
