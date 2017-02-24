




#ifndef MATH3D_H
#define MATH3D_H

#include "../platform.h"
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "plane3f.h"
#include "../utils.h"
#include "camera.h"

#define EPSILON	0.1f

#define DEGTORAD(a)		(a * M_PI / 180)
#define RADTODEG(a)		(a * 180 / M_PI)

#ifndef PI_ON_180
#	define PI_ON_180	(M_PI/180.0)
#endif

#define CAMERA_SPEED	121

Vec3f VMin(FixFrac minf, Vec3f v);

/*
7-rotation

3-rotation
calcpithc aw

calcpitch atan2(x,z)
whole: involved ratio : uninvolved
sqrt( x^2 + z^2 + y^2 ) = 1
(x^2 + z^2 ) / (y^2) = ((x2*xs)^2 + (z2*zs)^2) / ((y*ys)^2)
sqrt((x2*xs)^2 + (z2*zs)^2 + (y*ys)^2)) = 1
sub pitch   2d back-rot  to origin
->next yaw

void Camera::calcyaw()
{
	Vec3f d = m_view - m_pos;
	m_orientv.y = GetYaw(d.x, d.z);
}

void Camera::calcpitch()
{
	Vec3f d = m_view - m_pos;
	FixFrac lateral = MAG_VEC3F(Vec3f(d.x, 0, d.z));
	m_orientv.x = RADTODEG( atan2(d.y, lateral) );
}

void Camera::calcroll()
{
	Vec3f d = m_view - m_pos;
	FixFrac lateral = MAG_VEC3F(Vec3f(d.x, 0, d.z));
	m_orientv.x = RADTODEG( atan2(d.y, lateral) );
}
*/

#define MAG_VEC3F(vec)		((FixFrac)sqrtf( ((vec).x * (vec).x) + ((vec).y * (vec).y) + ((vec).z * (vec).z) ))
#define MAG_VEC2F(vec)		((FixFrac)sqrtf( ((vec).x * (vec).x) + ((vec).y * (vec).y) ))
#define DOT_VEC3F(vec)		(((vec).x * (vec).x) + ((vec).y * (vec).y) + ((vec).z * (vec).z))
#define DOT_VEC2F(vec)		(((vec).x * (vec).x) + ((vec).y * (vec).y))
#define MAG_VEC2I(vec)		(isqrt( (vec).x*(vec).x + (vec).y*(vec).y ))	////MUST BE DETERMINISTIC!!!!
#define DOT_VEC2I(vec)		((vec).x*(vec).x + (vec).y*(vec).y)
#define MAN_VEC2I(vec)		(iabs((vec).x) + iabs((vec).y))	// Manhattan distance

inline int32_t Diag(int32_t x, int32_t y)
{
	int32_t ax = iabs(x);
	int32_t ay = iabs(y);
	return ((((ax+ay)<<1) + (3 - 4) * imin(ax, ay))>>1);
}

//Diagonal distance
//#define DIA_VEC2I(vec)		((((iabs((vec).x)+iabs((vec).y))<<1) + (3 - 4) * imin(iabs((vec).x), iabs((vec).y)))>>1)
#define DIA_VEC2I(vec)			Diag((vec).x,(vec).y)
//#define DIA_VEC2I(vec)		((((abs((vec).x)+abs((vec).y))<<1) + (3 - 4) * std::min(abs((vec).x), abs((vec).y)))>>1)

/*
    dx = abs(node.x - goal.x)
    dy = abs(node.y - goal.y)
    return D * (dx + dy) + (D2 - 2 * D) * enmin(dx, dy)

	D2 = 3
	D = 2
*/

Vec3f Normalize(Vec3f vNormal);
Vec2f Normalize(Vec2f vNormal);
Vec3f Cross(Vec3f vVector1, Vec3f vVector2);
FixFrac Dot(Vec3f vVector1, Vec3f vVector2);
Vec3f Vector(Vec3f vPoint1, Vec3f vPoint2);
Vec3f Normal(Vec3f vTriangle[]);
Vec3f Normal2(Vec3f vTriangle[]);
bool InterPlane(Vec3f vPoly[], Vec3f vLine[], Vec3f &vNormal, FixFrac &originDistance);
bool WithinYaw(Camera* c, Vec3f p, FixFrac angle);
FixFrac DYaw(Camera* c, Vec3f p);
FixFrac AngleBetweenVectors(Vec3f Vector1, Vec3f Vector2);
Vec3f IntersectionPoint(Vec3f vNormal, Vec3f vLine[], FixFrac distance);
bool Intersection(Vec3f l0, Vec3f l, Plane3f p, Vec3f& inter);
Vec3f OnNear(int32_t x, int32_t y, int32_t width, int32_t height, Vec3f posvec, Vec3f sidevec, Vec3f upvec);
Vec3f OnNearPersp(int32_t x, int32_t y, int32_t width, int32_t height, Vec3f posvec, Vec3f sidevec, Vec3f upvec, Vec3f viewdir, FixFrac fov, FixFrac mind);
Vec3f ScreenPerspRay(int32_t x, int32_t y, int32_t width, int32_t height, Vec3f posvec, Vec3f sidevec, Vec3f upvec, Vec3f viewdir, FixFrac fov);
Vec3f Rotate(Vec3f v, FixFrac rad, FixFrac x, FixFrac y, FixFrac z);
Vec3f RotateAround(Vec3f v, Vec3f around, FixFrac rad, FixFrac x, FixFrac y, FixFrac z);
FixFrac Clipf(FixFrac n, FixFrac lower, FixFrac upper);
int32_t Clipi(int32_t n, int32_t lower, int32_t upper);
FixFrac GetYaw(FixFrac dx, FixFrac dz);
Matrix gluLookAt2(FixFrac eyex, FixFrac eyey, FixFrac eyez,
				  FixFrac centerx, FixFrac centery, FixFrac centerz,
				  FixFrac upx, FixFrac upy, FixFrac upz);
Matrix LookAt(FixFrac eyex, FixFrac eyey, FixFrac eyez,
				  FixFrac centerx, FixFrac centery, FixFrac centerz,
				  FixFrac upx, FixFrac upy, FixFrac upz);
Matrix PerspProj(FixFrac fov, FixFrac aspect, FixFrac znear, FixFrac zfar);
Matrix OrthoProj(FixFrac l, FixFrac r, FixFrac t, FixFrac b, FixFrac n, FixFrac f);
Vec4f ScreenPos(Matrix* mvp, Vec3f vec, FixFrac width, FixFrac height, bool persp);
FixFrac Snap(FixFrac base, FixFrac value);
FixFrac SnapNearest(FixFrac base, FixFrac value);

#endif
