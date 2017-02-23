



#include "../platform.h"
#include "quaternion.h"
#include "fixtrig.h"

inline void Quaternion::inverse()
{
	m_quat[0] = FixFrac(0)-m_quat[0];
	m_quat[1] = FixFrac(0)-m_quat[1];
	m_quat[2] = FixFrac(0)-m_quat[2];
	m_quat[3] = FixFrac(0)-m_quat[3];
}

void Quaternion::fromAngles( const FixFrac *angles )
{
	FixFrac angle;
	FixFrac sr, sp, sy, cr, cp, cy;

	angle = angles[2]*0.5f;
	sy = sin( angle );
	cy = cos( angle );
	angle = angles[1]*0.5f;
	sp = sin( angle );
	cp = cos( angle );
	angle = angles[0]*0.5f;
	sr = sin( angle );
	cr = cos( angle );

	FixFrac crcp = cr*cp;
	FixFrac srsp = sr*sp;

	m_quat[0] = ( FixFrac )( sr*cp*cy-cr*sp*sy );
	m_quat[1] = ( FixFrac )( cr*sp*cy+sr*cp*sy );
	m_quat[2] = ( FixFrac )( crcp*sy-srsp*cy );
	m_quat[3] = ( FixFrac )( crcp*cy+srsp*sy );
}

void Quaternion::slerp( const Quaternion& q1, Quaternion& q2, FixFrac interp )
{
	// Decide if one of the quaternions is backwards
	int32_t i;
	FixFrac a = 0, b = 0;
	for ( i = 0; i < 4; i++ )
	{
		a += ( q1[i]-q2[i] )*( q1[i]-q2[i] );
		b += ( q1[i]+q2[i] )*( q1[i]+q2[i] );
	}
	if ( a > b )
		q2.inverse();

	FixFrac cosom = q1[0]*q2[0]+q1[1]*q2[1]+q1[2]*q2[2]+q1[3]*q2[3];
	FixFrac sclq1, sclq2;

	if (( FixFrac(1.0)+cosom ) > ToFrac(0.00000001) )
	{
		if (( FixFrac(1)-cosom ) > ToFrac(0.00000001) )
		{
			FixFrac omega = acos( cosom );
			FixFrac sinom = sin( omega );
			sclq1 = sin(( FixFrac(1)-interp )*omega )/sinom;
			sclq2 = sin( interp*omega )/sinom;
		}
		else
		{
			sclq1 = FixFrac(1)-interp;
			sclq2 = interp;
		}
		for ( i = 0; i < 4; i++ )
			m_quat[i] = ( FixFrac )( sclq1*q1[i]+sclq2*q2[i] );
	}
	else
	{
		m_quat[0] = FixFrac(0)-q1[1];
		m_quat[1] = q1[0];
		m_quat[2] = FixFrac(0)-q1[3];
		m_quat[3] = q1[2];

		sclq1 = sin(( FixFrac(1)-interp )*0.5*M_PI );
		sclq2 = sin( interp*0.5*M_PI );
		for ( i = 0; i < 3; i++ )
			m_quat[i] = ( FixFrac )( sclq1*q1[i]+sclq2*m_quat[i] );
	}
}
