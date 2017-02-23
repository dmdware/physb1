




#ifndef PLANE3F_H
#define PLANE3F_H

#include "vec3f.h"
#include "fixfrac.h"

#define CLOSE_EPSILON ToFrac(0.1)

#define FRICTION (30)
#define INVFRICTION (1.0f/FRICTION)

class Plane3f
{
public:
	Vec3f m_normal;
	FixFrac m_d;

	bool operator==(const Plane3f p) const
	{
		//if(fabs(m_normal.x - p.m_normal.x) <= EPSILON && fabs(m_normal.y - p.m_normal.y) <= EPSILON && fabs(m_normal.z - p.m_normal.z) <= EPSILON && fabs(m_d - p.m_d) <= EPSILON)
		//	return true;

		if(m_normal.x == p.m_normal.x && m_normal.y == p.m_normal.y && m_normal.z == p.m_normal.z && m_d == p.m_d)
			return true;

		return false;
	}

	Plane3f();
	Plane3f(FixFrac x, FixFrac y, FixFrac z, FixFrac d);
	~Plane3f();
};

bool Close(Plane3f a, Plane3f b);
Vec3f PointOnPlane(Plane3f p);
FixFrac PlaneDistance(Vec3f normal, Vec3f point);
bool PointBehindPlane(Vec3f point, Plane3f plane);
bool PointOnOrBehindPlane(Vec3f point, Plane3f plane, FixFrac epsilon=CLOSE_EPSILON);
bool PointOnOrBehindPlane(Vec3f point, Vec3f normal, FixFrac dist, FixFrac epsilon=CLOSE_EPSILON);
void RotatePlane(Plane3f& p, Vec3f about, FixFrac radians, Vec3f axis);
void MakePlane(Vec3f* norm, FixFrac* d, Vec3f point, Vec3f setnorm);
void ParamLine(Vec3f* line, Vec3f* change);
bool LineInterPlane(const Vec3f* line, const Vec3f norm, const FixFrac d, Vec3f* inter);

#endif
