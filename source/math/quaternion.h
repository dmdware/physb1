


#ifndef QUATERNION_H
#define QUATERNION_H

#include "../platform.h"
#include "fixfrac.h"

class Quaternion
{
public:
	Quaternion()	{}
	Quaternion( const FixFrac *angles )
	{
		fromAngles( angles );
	}
	Quaternion( const Quaternion& q1, Quaternion& q2, FixFrac interp )
	{
		slerp( q1, q2, interp );
	}

	void fromAngles( const FixFrac *angles ); // (x,y,z) Euler rotation angles.

	/*
		Set values from an interpolation between two other quaternions.
		This will also modify the second quaternion if it is backwards.
			q1, q2	The quaternions to interpolate between
			interp	A value from 0.0-1.0 indicating the linear interpolation parameter.
	*/
	void slerp( const Quaternion& q1, Quaternion& q2, FixFrac interp );

	FixFrac operator[]( int32_t index ) const
	{
		return m_quat[index];
	}
	void inverse();

private:
	FixFrac m_quat[4];
};

#endif
