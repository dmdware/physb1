


#ifndef VEC3F_H
#define VEC3F_H

#include "../platform.h"
#include "fixfrac.h"
#include "vec4f.h"

#define CLOSE_EPSILON ToFrac(INTBASIS(0.1))

class Matrix;

class Vec3f
{
public:
	FixFrac x, y, z;

	Vec3f()
	{
		x = y = z = FixFrac(INTBASIS(0));
	}

	Vec3f(Vec4f v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vec3f(FixFrac X, FixFrac Y, FixFrac Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vec3f(const FixFrac* values)
	{
		set(values);
	}

	bool operator==(const Vec3f vVector) const
	{
		if(x == vVector.x && y == vVector.y && z == vVector.z)
			return true;

		return false;
	}

	bool operator!=(const Vec3f vVector) const
	{
		if(x == vVector.x && y == vVector.y && z == vVector.z)
			return false;

		return true;
	}

	Vec3f operator+(const Vec3f vVector) const
	{
		return Vec3f(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	Vec3f operator-(const Vec3f vVector) const
	{
		return Vec3f(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	Vec3f operator*(const FixFrac num) const
	{
		return Vec3f(x * num, y * num, z * num);
	}

	Vec3f operator*(const Vec3f v) const
	{
		return Vec3f(x * v.x, y * v.y, z * v.z);
	}

	Vec3f operator/(const FixFrac num) const
	{
		return Vec3f(x / num, y / num, z / num);
	}

	FixFrac& operator[](const int32_t i)
	{
		return *(((FixFrac*)this)+i);
	}

	inline void set(const FixFrac* values)
	{
		x = values[0];
		y = values[1];
		z = values[2];
	}

	void transform(const Matrix& m);
	void transform3(const Matrix& m);
};

bool Close(Vec3f a, Vec3f b);
bool Close(Vec3f a, Vec3f b, FixFrac ep);

#endif
