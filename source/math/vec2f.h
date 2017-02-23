


#ifndef VEC2F_H
#define VEC2F_H

#include "fixfrac.h"

class Vec2f
{
public:
	FixFrac x, y;

	Vec2f()
	{
		x = y = FixFrac(INTBASIS(0));
	}

	Vec2f(FixFrac X, FixFrac Y)
	{
		x = X;
		y = Y;
	}


	bool operator==(const Vec2f vVector) const
	{
		if(x == vVector.x && y == vVector.y)
			return true;

		return false;
	}

	bool operator!=(const Vec2f vVector) const
	{
		if(x == vVector.x && y == vVector.y)
			return false;

		return true;
	}

	Vec2f operator+(const Vec2f vVector) const
	{
		return Vec2f(vVector.x + x, vVector.y + y);
	}

	Vec2f operator-(const Vec2f vVector) const
	{
		return Vec2f(x - vVector.x, y - vVector.y);
	}

	Vec2f operator*(const FixFrac num) const
	{
		return Vec2f(x * num, y * num);
	}

	Vec2f operator*(const Vec2f v) const
	{
		return Vec2f(x * v.x, y * v.y);
	}

	Vec2f operator/(const FixFrac num) const
	{
		return Vec2f(x / num, y / num);
	}

	FixFrac& operator[](const int32_t i)
	{
		return *(((FixFrac*)this)+i);
	}
};

#endif
