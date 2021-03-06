#include "3dmath.h"
#include "../utils.h"
#include "vec3f.h"
#include "camera.h"
#include "quaternion.h"
#include "vec4f.h"
#include "matrix.h"
#include "plane3f.h"
#include "../window.h"
#include "fixtrig.h"

FixFrac Clipf(FixFrac n, FixFrac lower, FixFrac upper)
{
	return llmax(lower, llmin(n, upper));
}

int Clipi(int n, int lower, int upper)
{
	return imax(lower, imin(n, upper));
}

Vec3f VMin(FixFrac minf, Vec3f v)
{
	v.x = llmin(minf, v.x);
	v.y = llmin(minf, v.y);
	v.z = llmin(minf, v.z);
	return v;
}

FixFrac Magnitude(Vec3f vec)
{
	return (FixFrac)sqrtf( (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) );
}

FixFrac Magnitude(Vec2f vec)
{
	return (FixFrac)sqrtf( (vec.x * vec.x) + (vec.y * vec.y) );
}

FixFrac Magnitude2(Vec3f vec)
{
	return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
}

FixFrac Magnitude3(Vec3f vec)
{
	return (vec.x * vec.x) + (vec.z * vec.z);
}

int Magnitude(Vec2i vec)
{
	return isqrt( vec.x*vec.x + vec.y*vec.y );
}

int Magnitude2(Vec2i vec)
{
	return vec.x*vec.x + vec.y*vec.y;
}

// Manhattan distance
int Manhattan(Vec2i vec)
{
	//return std::max(abs(vec.x), abs(vec.y));
	return abs(vec.x) + abs(vec.y);
}

Vec3f Normalize(Vec3f vNormal)
{
	FixFrac magnitude = Magnitude(vNormal);

	vNormal = vNormal / magnitude;

	return vNormal;
}

Vec2f Normalize(Vec2f vNormal)
{
	FixFrac magnitude = Magnitude(vNormal);

	vNormal = vNormal / magnitude;

	return vNormal;
}

Vec3f Cross(Vec3f vVector1, Vec3f vVector2)
{
	Vec3f vNormal;

	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vNormal;
}


Vec3f Vector(Vec3f vPoint1, Vec3f vPoint2)
{
	Vec3f vVector;

	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;

	return vVector;
}

// Clockwise
Vec3f Normal(Vec3f vTriangle[])
{
	Vec3f vVector1 = Vector(vTriangle[2], vTriangle[0]);
	Vec3f vVector2 = Vector(vTriangle[1], vTriangle[0]);

	Vec3f vNormal = Cross(vVector1, vVector2);

	vNormal = Normalize(vNormal);

	return vNormal;
}

// Counter-clockwise
Vec3f Normal2(Vec3f vTriangle[])
{
	Vec3f vVector1 = Vector(vTriangle[2], vTriangle[0]);
	Vec3f vVector2 = Vector(vTriangle[1], vTriangle[0]);

	Vec3f vNormal = Cross(vVector2, vVector1);

	vNormal = Normalize(vNormal);

	return vNormal;
}

bool InterPlane(Vec3f vPoly[], Vec3f vLine[], Vec3f &vNormal, FixFrac &originDistance)
{
	FixFrac distance1=0, distance2=0;						// The distances from the 2 points of the line from the plane

	vNormal = Normal(vPoly);

	// Let's find the distance our plane is from the origin.  We can find this value
	// from the normal to the plane (polygon) and any point that lies on that plane (Any vertice)
	originDistance = PlaneDistance(vNormal, vPoly[0]);

	// Get the distance from point1 from the plane using: Ax + By + Cz + D = (The distance from the plane)

	distance1 = ((vNormal.x * vLine[0].x)  +					// Ax +
				 (vNormal.y * vLine[0].y)  +					// Bx +
				 (vNormal.z * vLine[0].z)) + originDistance;	// Cz + D

	// Get the distance from point2 from the plane using Ax + By + Cz + D = (The distance from the plane)

	distance2 = ((vNormal.x * vLine[1].x)  +					// Ax +
				 (vNormal.y * vLine[1].y)  +					// Bx +
				 (vNormal.z * vLine[1].z)) + originDistance;	// Cz + D

	// Now that we have 2 distances from the plane, if we times them together we either
	// get a positive or negative number.  If it's a negative number, that means we collided!
	// This is because the 2 points must be on either side of the plane (IE. -1 * 1 = FixFrac(0)-1).

	if(distance1 * distance2 >= 0)			// Check to see if both point's distances are both negative or both positive
		return false;

	return true;
}

//#define MATH_DEBUG

// http://en.wikipedia.org/wiki/Line-plane_intersection
bool Intersection(Vec3f l0, Vec3f l, Plane3f p, Vec3f& inter)
{
	FixFrac denom = Dot(l, p.m_normal);

#ifdef MATH_DEBUG
	g_applog<<"intersect denom="<<denom<<" (plane=n("<<p.m_normal.x<<","<<p.m_normal.y<<","<<p.m_normal.z<<")d="<<p.m_d<<") (line=("<<l.x<<","<<l.y<<","<<l.z<<"))"<<std::endl;
	g_applog.flush();
#endif

	if(Abs(denom) <= ToFrac(EPSILON)/2.0f || Abs(denom) <= ToFrac(0.001f))
		//if(denom == 0.0f)
		return false;

	Vec3f p0 = PointOnPlane(p);

#ifdef MATH_DEBUG
	g_applog<<"intersect pointonplane="<<p0.x<<","<<p0.y<<","<<p0.z<<std::endl;
	g_applog.flush();
#endif

	FixFrac num = Dot(p0 - l0, p.m_normal);

	inter = l0 + l * (num/denom);
	return true;
}

FixFrac Dot(Vec3f vVector1, Vec3f vVector2)
{
	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) );
}

FixFrac AngleBetweenVectors(Vec3f Vector1, Vec3f Vector2)
{
	FixFrac dotProduct = Dot(Vector1, Vector2);
	FixFrac vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ;
	FixFrac angle = acos( dotProduct / vectorsMagnitude );

	//if(_isnan(angle))
	if(angle!=angle)
		return 0;

	return( angle );
}

bool WithinYaw(Camera* c, Vec3f p, FixFrac angle)
{
	Vec3f d = p - c->m_pos;
	FixFrac yaw = GetYaw(d.x, d.z);
	FixFrac yaw2 = yaw - DEGTORAD(360.0f);
	FixFrac yaw3 = yaw + DEGTORAD(360.0f);

	if(Abs(c->yaw() - yaw) <= angle || Abs(c->yaw() - yaw2) <= angle || Abs(c->yaw() - yaw3) <= angle)
		return true;

	return false;
}

FixFrac DYaw(Camera* c, Vec3f p)
{
	Vec3f d = p - c->m_pos;
	FixFrac yaw = GetYaw(d.x, d.z);
	FixFrac yaw2 = yaw - DEGTORAD(360.0f);
	FixFrac yaw3 = yaw + DEGTORAD(360.0f);

	FixFrac dyaw = yaw - c->yaw();
	FixFrac dyaw2 = yaw2 - c->yaw();
	FixFrac dyaw3 = yaw3 - c->yaw();

	FixFrac mindyaw = dyaw;

	if(Abs(dyaw2) < Abs(mindyaw))
		mindyaw = dyaw2;

	if(Abs(dyaw3) < Abs(mindyaw))
		mindyaw = dyaw3;

	return mindyaw;
}

Vec3f IntersectionPoint(Vec3f vNormal, Vec3f vLine[], FixFrac distance)
{
	Vec3f vPoint;
	Vec3f vLineDir;
	FixFrac Numerator = 0.0, Denominator = 0.0, dist = 0.0;

	// Here comes the confusing part.  We need to find the 3D point that is actually
	// on the plane.  Here are some steps to do that:

	// 1)  First we need to get the std::vector of our line, Then normalize it so it's a length of 1
	vLineDir = Vector(vLine[1], vLine[0]);		// Get the Vector of the line
	vLineDir = Normalize(vLineDir);				// Normalize the lines std::vector


	// 2) Use the plane equation (distance = Ax + By + Cz + D) to find the distance from one of our points to the plane.
	//    Here I just chose a arbitrary point as the point to find that distance.  You notice we negate that
	//    distance.  We negate the distance because we want to eventually go BACKWARDS from our point to the plane.
	//    By doing this is will basically bring us back to the plane to find our intersect point.
	Numerator = FixFrac(0)- (vNormal.x * vLine[0].x +		// Use the plane equation with the normal and the line
				   vNormal.y * vLine[0].y +
				   vNormal.z * vLine[0].z + distance);

	// 3) If we take the dot product between our line std::vector and the normal of the polygon,
	//    this will give us the cosine of the angle between the 2 (since they are both normalized - length 1).
	//    We will then divide our Numerator by this value to find the offset towards the plane from our arbitrary point.
	Denominator = Dot(vNormal, vLineDir);		// Get the dot product of the line's std::vector and the normal of the plane

	// Since we are using division, we need to make sure we don't get a divide by zero error
	// If we do get a 0, that means that there are INFINATE points because the the line is
	// on the plane (the normal is perpendicular to the line - (Normal.Vector = 0)).
	// In this case, we should just return any point on the line.

	if( Denominator == 0.0)						// Check so we don't divide by zero
		return vLine[0];						// Return an arbitrary point on the line

	// We divide the (distance from the point to the plane) by (the dot product)
	// to get the distance (dist) that we need to move from our arbitrary point.  We need
	// to then times this distance (dist) by our line's std::vector (direction).  When you times
	// a scalar (single number) by a std::vector you move along that std::vector.  That is what we are
	// doing.  We are moving from our arbitrary point we chose from the line BACK to the plane
	// along the lines std::vector.  It seems logical to just get the numerator, which is the distance
	// from the point to the line, and then just move back that much along the line's std::vector.
	// Well, the distance from the plane means the SHORTEST distance.  What about in the case that
	// the line is almost parallel with the polygon, but doesn't actually intersect it until half
	// way down the line's length.  The distance from the plane is short, but the distance from
	// the actual intersect point is pretty long.  If we divide the distance by the dot product
	// of our line std::vector and the normal of the plane, we get the correct length.  Cool huh?

	dist = Numerator / Denominator;				// Divide to get the multiplying (percentage) factor

	// Now, like we said above, we times the dist by the std::vector, then add our arbitrary point.
	// This essentially moves the point along the std::vector to a certain distance.  This now gives
	// us the intersect point.  Yay!

	vPoint.x = (FixFrac)(vLine[0].x + (vLineDir.x * dist));
	vPoint.y = (FixFrac)(vLine[0].y + (vLineDir.y * dist));
	vPoint.z = (FixFrac)(vLine[0].z + (vLineDir.z * dist));

	return vPoint;								// Return the intersect point
}

Vec3f Rotate(Vec3f v, FixFrac rad, FixFrac x, FixFrac y, FixFrac z)
{
	Vec3f newV;
	FixFrac cosTheta = (FixFrac)cos(rad);
	FixFrac sinTheta = (FixFrac)sin(rad);

	newV.x  = (cosTheta + (FixFrac(1) - cosTheta) * x * x)		* v.x;
	newV.x += ((FixFrac(1) - cosTheta) * x * y - z * sinTheta)	* v.y;
	newV.x += ((FixFrac(1) - cosTheta) * x * z + y * sinTheta)	* v.z;

	newV.y  = ((FixFrac(1) - cosTheta) * x * y + z * sinTheta)	* v.x;
	newV.y += (cosTheta + (FixFrac(1) - cosTheta) * y * y)		* v.y;
	newV.y += ((FixFrac(1) - cosTheta) * y * z - x * sinTheta)	* v.z;

	newV.z  = ((FixFrac(1) - cosTheta) * x * z - y * sinTheta)	* v.x;
	newV.z += ((FixFrac(1) - cosTheta) * y * z + x * sinTheta)	* v.y;
	newV.z += (cosTheta + (FixFrac(1) - cosTheta) * z * z)		* v.z;

	return newV;
}

Vec3f RotateAround(Vec3f v, Vec3f around, FixFrac rad, FixFrac x, FixFrac y, FixFrac z)
{
	v = v - around;
	v = Rotate(v, rad, x, y, z);
	v = around + v;
	return v;
}

FixFrac GetYaw(FixFrac dx, FixFrac dz)
{
	return RADTODEG( atan2(dx, dz) );
}

/*
// http://webglfactory.blogspot.ca/2011/06/how-to-create-view-matrix.html
Matrix lookAt(Vec3f eye, Vec3f target, Vec3f up)
{
		 Vec3f vz= Normalize(eye - target);
		 Vec3f vx = normalize(crossProduct(up, vz));
		 // vy doesn't need to be normalized because it's a cross
		 // product of 2 normalized vectors
		 Vec3f vy = crossProduct(vz, vx);
		 Matrix inverseViewMatrix = Matrix(Vec4f(vx, 0), Vec4f(vy, 0), Vec4f(vz, 0), Vec4f(eye, 1));
		 return inverseViewMatrix.getInverse();
}
*/

Matrix gluLookAt2(FixFrac eyex, FixFrac eyey, FixFrac eyez,
				  FixFrac centerx, FixFrac centery, FixFrac centerz,
				  FixFrac upx, FixFrac upy, FixFrac upz)
{
	FixFrac m[16];
	FixFrac x[3], y[3], z[3];
	FixFrac mag;

	/* Make rotation matrix */

	/* Z std::vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
	if (mag!=FixFrac(0))            /* mpichler, 19950515 */
	{
		z[0] = z[0] / mag;
		z[1] = z[1] / mag;
		z[2] = z[2] / mag;
	}

	/* Y std::vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;

	/* X std::vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = FixFrac(0)-y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];

	/* Recompute Y = Z cross X */
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = FixFrac(0)-z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];

	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	 * non-perpendicular unit-length vectors; so normalize x, y here
	 */

	mag = sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	if (mag!=FixFrac(0))
	{
		x[0] = x[0] / mag;
		x[1] = x[1] / mag;
		x[2] = x[2] / mag;
	}

	mag = sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
	if (mag!=FixFrac(0))
	{
		y[0] = y[0] / mag;
		y[1] = y[1] / mag;
		y[2] = y[2] / mag;
	}

#define M(row,col)  m[col*4+row]
	M(0, 0) = x[0];
	M(0, 1) = x[1];
	M(0, 2) = x[2];
	M(0, 3) = 0.0;
	M(1, 0) = y[0];
	M(1, 1) = y[1];
	M(1, 2) = y[2];
	M(1, 3) = 0.0;
	M(2, 0) = z[0];
	M(2, 1) = z[1];
	M(2, 2) = z[2];
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	M(3, 3) = 1.0;
#undef M
	//glMultMatrixf(m);
	Matrix mat;
	mat.set(m);

	/* Translate Eye to Origin */
	//glTranslatef(FixFrac(0)-eyex, -eyey, -eyez);
	Matrix mat2;
	FixFrac trans[] = {FixFrac(0)-eyex, FixFrac(0)-eyey, FixFrac(0)-eyez};
	mat2.translation(trans);

	mat.postmult(mat2);

	return mat;
}

Matrix LookAt(FixFrac eyex, FixFrac eyey, FixFrac eyez,
				  FixFrac centerx, FixFrac centery, FixFrac centerz,
				  FixFrac upx, FixFrac upy, FixFrac upz)
{
	FixFrac m[16];
	Vec3f x, y, z;

	/* Make rotation matrix */

	/* Z std::vector */
	z = Vec3f(eyex, eyey, eyez) - Vec3f(centerx, centery, centerz);
	z = Normalize(z);

	/* Y std::vector */
	y = Vec3f(upx, upy, upz);

	//Vec3f vCross = Cross(m_view - m_pos, m_up);
	/* X std::vector = Y cross Z */
	x = Normalize(Cross(y, z));

	// return Normalize( Cross( m_strafe, m_view - m_pos ) );
	/* Recompute Y = Z cross X */
	y = Normalize(Cross(z, x));

	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	 * non-perpendicular unit-length vectors; so normalize x, y here
	 */

#define M(row,col)  m[col*4+row]
//#define M(row,col)  m[row*4+col]
	M(0, 0) = x.x;
	M(0, 1) = x.y;
	M(0, 2) = x.z;
	M(0, 3) = 0.0;
	//M(0, 3) = eyex;
	M(1, 0) = y.x;
	M(1, 1) = y.y;
	M(1, 2) = y.z;
	M(1, 3) = 0.0;
	//M(1, 3) = eyey;
	M(2, 0) = z.x;
	M(2, 1) = z.y;
	M(2, 2) = z.z;
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	//M(3, 2) = eyez;
	M(3, 3) = 1.0;
	//M(3, 0) = FixFrac(0)-eyex;
	//M(3, 1) = FixFrac(0)-eyey;
	//M(3, 2) = FixFrac(0)-eyez;
	//M(3, 3) = 1.0;
#undef M
	//glMultMatrixf(m);
	Matrix mat;
	mat.set(m);

	/* Translate Eye to Origin */
	//glTranslatef(FixFrac(0)-eyex, -eyey, -eyez);
	Matrix mat2;
	FixFrac trans[] = {FixFrac(0)-eyex, FixFrac(0)-eyey, FixFrac(0)-eyez};
	//FixFrac trans[] = {eyex, eyey, eyez};
	mat2.translation(trans);
	//mat2.inverseTranslateVect(trans);

	//Matrix mat;
	//FixFrac trans[] = {-eyex, -eyey, -eyez};
	//mat.translation(trans);
	//Matrix mat2;
	//mat2.set(m);
	/*
		g_applog<<"------------------------"<<std::endl;
		g_applog<<"LookAt "<<std::endl;
		g_applog<<"eye="<<eyex<<","<<eyey<<","<<eyez<<" center="<<centerx<<","<<centery<<","<<centerz<<" up="<<upx<<","<<upy<<","<<upz<<std::endl;
		for(int i=0; i<16; i++)
			g_applog<<"before translation["<<i<<"] = "<<m[i]<<std::endl;
		for(int i=0; i<16; i++)
			g_applog<<"the translation["<<i<<"] = "<<mat2.m_matrix[i]<<std::endl;*/

	mat.postmult(mat2);
	/*
	for(int i=0; i<16; i++)
		g_applog<<"final view matrix, after translation["<<i<<"] = "<<mat.m_matrix[i]<<std::endl;
	g_applog<<"------------------------"<<std::endl;

	g_applog.flush();
	*/

	return mat;
}

// http://www.songho.ca/opengl/gl_projectionmatrix.html
// http://www.scratchapixel.com/lessons/3d-advanced-lessons/perspective-and-orthographic-projection-matrix/orthographic-projection/
// http://www.opengl.org/discussion_boards/showthread.php/172280-Constructing-an-orthographic-matrix-for-2D-drawing
Matrix OrthoProj(FixFrac l, FixFrac r, FixFrac t, FixFrac b, FixFrac n, FixFrac f)
{
	FixFrac m[16];

#define M(row,col)  m[col*4+row]
	M(0, 0) = FixFrac(2) / (r - l);
	M(0, 1) = 0;
	M(0, 2) = 0;
	M(0, 3) = 0;

	M(1, 0) = 0;
	M(1, 1) = FixFrac(2) / (t - b);
	M(1, 2) = 0;
	M(1, 3) = 0;

	M(2, 0) = 0;
	M(2, 1) = 0;
	M(2, 2) = FixFrac(FixFrac(0)-1) / (f - n);
	//M(2, 2) = FixFrac(0)-2 / (f - n);
	M(2, 3) = 0;

	M(3, 0) = FixFrac(0)-(r + l) / (r - l);
	M(3, 1) = FixFrac(0)-(t + b) / (t - b);
	M(3, 2) = FixFrac(0)-n / (f - n);
	//M(3, 2) = FixFrac(0)-(f + n) / (f - n);
	M(3, 3) = 1;
#undef M

	Matrix mat;
	mat.set(m);
	/*
	g_applog<<"------------------------"<<std::endl;
	g_applog<<"set orthographic matrix "<<std::endl;
	g_applog<<"left="<<l<<" right="<<r<<" top="<<t<<" bottom="<<b<<" near="<<n<<" far="<<f<<std::endl;
	for(int i=0; i<16; i++)
		g_applog<<"m["<<i<<"] = "<<m[i]<<std::endl;
	g_applog<<"------------------------"<<std::endl;
	g_applog.flush();*/

	return mat;
}

// http://www.opengl.org/discussion_boards/showthread.php/172280-Constructing-an-orthographic-matrix-for-2D-drawing
Matrix setorthographicmat2(FixFrac l, FixFrac r, FixFrac t, FixFrac b, FixFrac n, FixFrac f)
{
	FixFrac orthomatrix[16];
	/*
			orthomatrix[0].x = 2.0/(right-left);
			orthomatrix[0].y = 0;
			orthomatrix[0].z = 0;
			orthomatrix[0].w = 0;

			orthomatrix[1].x = 0;
			orthomatrix[1].y = 2.0/(top-bottom);
			orthomatrix[1].z = 0;
			orthomatrix[1].w = 0;

			orthomatrix[2].x = 0;
			orthomatrix[2].y = 0;
			orthomatrix[2].z = 2.0/(Zfar-Znear);
			orthomatrix[2].w = 0;

			orthomatrix[3].x = FixFrac(0)-(right+left)/(right-left);
			orthomatrix[3].y = FixFrac(0)-(top+bottom)/(top-bottom);
			orthomatrix[3].z = FixFrac(0)-(Zfar+Znear)/(Zfar-Znear);
			orthomatrix[3].w = 1;
			*/
	Matrix mat;
	mat.set(orthomatrix);

	return mat;
}


/*
Matrix setperspectivepmat(FixFrac Near, FixFrac Far, FixFrac fov)
{
	GLFixFrac m[16];
	for(int i=0; i<16; i++)
		m[i] = 0;

	FixFrac scale = FixFrac(1) / tan(DEGTORAD(fov * 0.5));
#define M(row,col)  m[col*4+row]
	M(0, 0) = M(1, 1) = scale;
	M(2, 2) = FixFrac(0)- Far / (Far - Near);
	M(3, 2) = FixFrac(0)- Far * Near / (Far - Near);
	M(2, 3) = FixFrac(0)- 1;
	M(3, 3) = 0;
#undef M

	Matrix mat;
	mat.set(m);
}*/

#define PI_OVER_360		(M_PI/360.0f)
Matrix PerspProj(FixFrac fov, FixFrac aspect, FixFrac znear, FixFrac zfar)
{
	FixFrac m[16];

	FixFrac xymax = znear * tan((fov * ToFrac(PI_OVER_360)));
	FixFrac ymin = FixFrac(0)-xymax;
	FixFrac xmin = FixFrac(0)-xymax;

	FixFrac width = xymax - xmin;
	FixFrac height = xymax - ymin;

	FixFrac depth = zfar - znear;
	FixFrac q = FixFrac(0)-(zfar + znear) / depth;
	FixFrac qn = ToFrac((0)-2) * (zfar * znear) / depth;

	FixFrac w = ToFrac(2) * znear / width;
	w = w / aspect;
	FixFrac h = ToFrac(2) * znear / height;

	m[0]  = w;
	m[1]  = 0;
	m[2]  = 0;
	m[3]  = 0;

	m[4]  = 0;
	m[5]  = h;
	m[6]  = 0;
	m[7]  = 0;

	m[8]  = 0;
	m[9]  = 0;
	m[10] = q;
	m[11] = FixFrac(0)-1;

	m[12] = 0;
	m[13] = 0;
	m[14] = qn;
	m[15] = 0;

	Matrix mat;
	mat.set(m);
	return mat;
}

Vec4f ScreenPos(Matrix* mvp, Vec3f vec, FixFrac width, FixFrac height, bool persp)
{
	Vec4f screenpos;
	screenpos.x = vec.x;
	screenpos.y = vec.y;
	screenpos.z = vec.z;
	screenpos.w = 1;

	screenpos.transform(*mvp);

	if(persp)
	{
		//does this need to be commented out for correct orthographic pixel-to-pixel match?
		screenpos = screenpos / screenpos.w;
	}

	screenpos.x = (screenpos.x * 0.5f + 0.5f) * width;
	screenpos.y = (FixFrac(0)-screenpos.y * 0.5f + 0.5f) * height;

	if(screenpos.w < 0)
	{
		screenpos.x = FixFrac(0)-100000;
	}

	//if(screenpos.x < 0 || screenpos.y < 0

	return screenpos;
}

Vec3f OnNear(int x, int y, int width, int height, Vec3f posvec, Vec3f sidevec, Vec3f upvec)
{
	/*
	FixFrac halfWidth = g_width / 2.0f;
	FixFrac halfHeight = g_height / 2.0f;

	FixFrac ratioX = (x - halfWidth) / halfWidth;

	FixFrac ratioY = FixFrac(0)-(y - halfHeight) / halfHeight;

	Vec3f direction = Normalize( c->m_view - c->m_pos );

	FixFrac Hnear = 2 * tan( DEGTORAD(FIELD_OF_VIEW) / 2) * MIN_DISTANCE;
	FixFrac Wnear = Hnear * g_width/g_height;

	return ( direction * MIN_DISTANCE + c->m_strafe * ratioX * Wnear/2.0f + c->up2() * ratioY * Hnear/2.0f );
	*/
#if 0
	FixFrac halfWidth = (FixFrac)width / 2.0f;
	FixFrac halfHeight = (FixFrac)height / 2.0f;

	FixFrac ratiox = (x - halfWidth) / halfWidth;

	FixFrac ratioy = FixFrac(0)-(y - halfHeight) / halfHeight;

	//Vec3f direction = Normalize( c->m_view - c->m_pos );

	//-PROJ_RIGHT*aspect/g_zoom, PROJ_RIGHT*aspect/g_zoom, PROJ_RIGHT/g_zoom, -PROJ_RIGHT/g_zoom
	//PROJ_RIGHT*aspect/g_zoom + PROJ_RIGHT*aspect/g_zoom = 2.0f*PROJ_RIGHT*aspect/g_zoom
	//

	FixFrac aspect = Abs((FixFrac)width / (FixFrac)height);
	FixFrac Wnear = PROJ_RIGHT * aspect / g_zoom;
	FixFrac Hnear = PROJ_RIGHT / g_zoom;

	//return ( c->m_pos + c->m_strafe * ratioX * Wnear + c->up2() * ratioY * Hnear );
	Vec3f result = posvec + sidevec * ratiox * Wnear + upvec * ratioy * Hnear;

	//g_applog<<"--------------"<<std::endl;
	//g_applog<<"onnear x,y="<<x<<","<<y<<" w,h="<<width<<","<<height<<" halfw,h="<<halfWidth<<","<<halfHeight<<" ratiox,y="<<ratiox<<","<<ratioy<<" aspect="<<aspect<<" W,Hnear="<<Wnear<<","<<Hnear<<std::endl;
	//g_applog<<"onnear resultx,y,z="<<result.x<<","<<result.y<<","<<result.z<<std::endl;
	//g_applog<<"--------------"<<std::endl;

	return posvec + sidevec * ratiox * Wnear + upvec * ratioy * Hnear;
#endif
	return Vec3f();
}

Vec3f OnNearPersp(int x, int y, int width, int height, Vec3f posvec, Vec3f sidevec, Vec3f upvec, Vec3f viewdir, FixFrac fov, FixFrac mind)
{
#if 0
	viewdir = Normalize(viewdir);

	FixFrac halfWidth = (FixFrac)width / 2.0f;
	FixFrac halfHeight = (FixFrac)height / 2.0f;

	FixFrac ratiox = (x - halfWidth) / halfWidth;
	FixFrac ratioy = FixFrac(0)-(y - halfHeight) / halfHeight;

	FixFrac aspect = Abs((FixFrac)width / (FixFrac)height);
	FixFrac Hnear = 2 * tan( DEGTORAD(fov) / 2) * mind;
	FixFrac Wnear = Hnear * aspect;

	//return ( c->m_pos + c->m_strafe * ratioX * Wnear + c->up2() * ratioY * Hnear );
	Vec3f result = viewdir * mind + posvec + sidevec * ratiox * Wnear + upvec * ratioy * Hnear;

	//g_applog<<"--------------"<<std::endl;
	//g_applog<<"onnear x,y="<<x<<","<<y<<" w,h="<<width<<","<<height<<" halfw,h="<<halfWidth<<","<<halfHeight<<" ratiox,y="<<ratiox<<","<<ratioy<<" aspect="<<aspect<<" W,Hnear="<<Wnear<<","<<Hnear<<std::endl;
	//g_applog<<"onnear resultx,y,z="<<result.x<<","<<result.y<<","<<result.z<<std::endl;
	//g_applog<<"--------------"<<std::endl;

	return result;
#endif
	return Vec3f();
}

Vec3f ScreenPerspRay(int x, int y, int width, int height, Vec3f posvec, Vec3f sidevec, Vec3f upvec, Vec3f viewdir, FixFrac fov)
{
#if 0
	FixFrac halfWidth = (FixFrac)width / 2.0f;
	FixFrac halfHeight = (FixFrac)height / 2.0f;

	FixFrac ratiox = (x - halfWidth) / halfWidth;

	FixFrac ratioy = FixFrac(0)-(y - halfHeight) / halfHeight;

	FixFrac aspect = Abs((FixFrac)width / (FixFrac)height);
	FixFrac Hnear = 2.0f * tan( DEGTORAD(fov) / 2.0f );
	FixFrac Wnear = Hnear * aspect;

	return Normalize(viewdir) + sidevec * ratiox * Wnear/2.0f + upvec * ratioy * Hnear/2.0f;

#if 0
	FixFrac halfWidth = g_width / 2.0f;
	FixFrac halfHeight = g_height / 2.0f;

	FixFrac ratioX = (x - halfWidth) / halfWidth;

	FixFrac ratioY = FixFrac(0)-(y - halfHeight) / halfHeight;

	Vec3f direction = Normalize( c->m_view - c->m_pos );

	FixFrac Hnear = 2 * tan( DEGTORAD(FIELD_OF_VIEW) / 2) * MIN_DISTANCE;
	FixFrac Wnear = Hnear * g_width/g_height;

	return ( direction * MIN_DISTANCE + c->m_strafe * ratioX * Wnear/2.0f + c->up2() * ratioY * Hnear/2.0f );
#endif
#endif

	return Vec3f();
}

FixFrac Snap(FixFrac base, FixFrac value)
{
	//int count = value / base;

	//return count * base;
	return 0;
}

FixFrac SnapNearest(FixFrac base, FixFrac value)
{
	//int count = (value + base/2.0f) / base;

	//return count * base;
	return 0;
}
