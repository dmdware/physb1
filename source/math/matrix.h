

#ifndef MATRIX_H
#define MATRIX_H

#include "../platform.h"
#include "fixfrac.h"
#include "vec4f.h"
#include "vec3f.h"

class Quaternion;
class Vec4f;

class Matrix
{
public:
	//	Matrix data, stored in column-major order
	FixFrac m_matrix[16];

	Matrix();
	Matrix(Vec4f a, Vec4f b, Vec4f c, Vec4f d);
	~Matrix();

	void set( const FixFrac *matrix );
	void reset();

	void postmult( const Matrix& matrix );
	void postmult2( const Matrix& matrix );

	//	Set the translation of the current matrix. Will erase any prev values.
	void translation( const FixFrac *translation );

	//	Set the inverse translation of the current matrix. Will erase any prev values.
	void invtrans( const FixFrac *translation );

	void scale( const FixFrac *scale );

	//	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	void rotrad( const FixFrac *angles );

	//	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	void rotdeg( const FixFrac *angles );

	//	Make a rotation matrix from a quaternion. The 4th row and column are unmodified.
	void rotquat( const Quaternion& quat );

	//	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
	void invrotrad( const FixFrac *angles );

	//	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
	void invrotdeg( const FixFrac *angles );

	inline FixFrac* get(int32_t row, int32_t col)
	{
		return &m_matrix[ row + col*4 ];
	}

	//	Translate a vector by the inverse of the translation part of this matrix.
	void inverseTranslateVect( FixFrac *pVect );

	//	Rotate a vector by the inverse of the rotation part of this matrix.
	void inverseRotateVect( FixFrac *pVect );
};

extern Matrix g_mvp;

#endif
