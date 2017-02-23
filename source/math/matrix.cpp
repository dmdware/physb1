




#include "matrix.h"
#include "../platform.h"
#include "quaternion.h"
#include "vec4f.h"
#include "fixtrig.h"

Matrix g_mvp;

Matrix::Matrix()
{
	reset();
}

Matrix::Matrix(Vec4f a, Vec4f b, Vec4f c, Vec4f d)
{
#define M(row,col)  m_matrix[col*4+row]

#undef M
}

Matrix::~Matrix()
{
}

void Matrix::set( const FixFrac *matrix )
{
	//memcpy( m_matrix, matrix, sizeof( FixFrac )*16 );
	for(int i=0; i<4*4; i++)
		m_matrix[i] = matrix[i];
}

void Matrix::reset()
{
	//memset( m_matrix, 0, sizeof( FixFrac )*16 );
	for(int i=0; i<4*4; i++)
		m_matrix[i] = 0;
	m_matrix[0] = m_matrix[5] = m_matrix[10] = m_matrix[15] = INTBASIS(1);
}

void Matrix::inverseRotateVect( FixFrac *pVect )
{
	FixFrac vec[3];

	vec[0] = pVect[0]*m_matrix[0]+pVect[1]*m_matrix[1]+pVect[2]*m_matrix[2];
	vec[1] = pVect[0]*m_matrix[4]+pVect[1]*m_matrix[5]+pVect[2]*m_matrix[6];
	vec[2] = pVect[0]*m_matrix[8]+pVect[1]*m_matrix[9]+pVect[2]*m_matrix[10];

	//memcpy( pVect, vec, sizeof( FixFrac )*3 );
		for(int i=0; i<3; i++)
			pVect[i] = vec[i];
}

void Matrix::inverseTranslateVect( FixFrac *pVect )
{
	pVect[0] = pVect[0]-m_matrix[12];
	pVect[1] = pVect[1]-m_matrix[13];
	pVect[2] = pVect[2]-m_matrix[14];
}

//no longer used for light matrix concatenation (and skeletal animation?)
void Matrix::postmult( const Matrix& matrix )
{
	FixFrac newMatrix[16];

#if 0

	const FixFrac *m1 = m_matrix, *m2 = matrix.m_matrix;

	newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
	newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
	newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
	newMatrix[3] = 0;

	newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
	newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
	newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
	newMatrix[7] = 0;

	newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
	newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
	newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
	newMatrix[11] = 0;

	newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
	newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
	newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
	newMatrix[15] = 1;

#else

	const FixFrac *a = m_matrix, *b = matrix.m_matrix;

	newMatrix[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8] * b[2]   + a[12] * b[3];
	newMatrix[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9] * b[2]   + a[13] * b[3];
	newMatrix[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
	newMatrix[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];

	newMatrix[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8] * b[6]   + a[12] * b[7];
	newMatrix[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9] * b[6]   + a[13] * b[7];
	newMatrix[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
	newMatrix[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];

	newMatrix[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8] * b[10]  + a[12] * b[11];
	newMatrix[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9] * b[10]  + a[13] * b[11];
	newMatrix[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
	newMatrix[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];

	newMatrix[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14]  + a[12] * b[15];
	newMatrix[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14]  + a[13] * b[15];
	newMatrix[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	newMatrix[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

#endif

	set( newMatrix );
}

//used only for light matrix (and skeletal animation?), doesn't work with perspective projection because that is not an affine transformation
void Matrix::postmult2( const Matrix& matrix )
{
	FixFrac newMatrix[16];

#if 1

	const FixFrac *m1 = m_matrix, *m2 = matrix.m_matrix;

	newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
	newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
	newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
	newMatrix[3] = 0;

	newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
	newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
	newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
	newMatrix[7] = 0;

	newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
	newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
	newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
	newMatrix[11] = 0;

	newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
	newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
	newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
	newMatrix[15] = 1;

#else

	const FixFrac *a = m_matrix, *b = matrix.m_matrix;

	newMatrix[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8] * b[2]   + a[12] * b[3];
	newMatrix[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9] * b[2]   + a[13] * b[3];
	newMatrix[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
	newMatrix[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];

	newMatrix[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8] * b[6]   + a[12] * b[7];
	newMatrix[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9] * b[6]   + a[13] * b[7];
	newMatrix[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
	newMatrix[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];

	newMatrix[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8] * b[10]  + a[12] * b[11];
	newMatrix[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9] * b[10]  + a[13] * b[11];
	newMatrix[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
	newMatrix[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];

	newMatrix[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14]  + a[12] * b[15];
	newMatrix[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14]  + a[13] * b[15];
	newMatrix[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	newMatrix[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

#endif

	set( newMatrix );
}

void Matrix::translation( const FixFrac *translation )
{
#if 0
	m_matrix[0] = m_matrix[5] =  m_matrix[10] = m_matrix[15] = 1.0;
	m_matrix[1] = m_matrix[2] = m_matrix[3] = m_matrix[4] = 0.0;
	m_matrix[6] = m_matrix[7] = m_matrix[8] = m_matrix[9] = 0.0;
	m_matrix[11] = 0.0;

#elif 1

	m_matrix[12] = translation[0];
	m_matrix[13] = translation[1];
	m_matrix[14] = translation[2];

#elif 0

#define M(row,col)  m_matrix[col*4+row]
	//#define M(row,col)  m_matrix[row*4+col]
	/*
	// http://stackoverflow.com/questions/13293469/why-does-my-translation-matrix-needs-to-be-transposed
	1, 0, 0, 0
	0, 1, 0, 0
	0, 0, 1, 0
	x, y, z, 1
	*/
	M(0,0) = 1;
	M(0,1) = 0;
	M(0,2) = 0;
	M(0,3) = translation[0];
	M(1,0) = 0;
	M(1,1) = 1;
	M(1,2) = 0;
	M(1,3) = translation[1];
	M(2,0) = 0;
	M(2,1) = 0;
	M(2,2) = 1;
	M(2,3) = translation[2];
	M(3,0) = 0;
	M(3,1) = 0;
	M(3,2) = 0;
	M(3,3) = 1;
#undef M

#endif
}

void Matrix::invtrans( const FixFrac *translation )
{
	m_matrix[12] = FixFrac(0)-translation[0];
	m_matrix[13] = FixFrac(0)-translation[1];
	m_matrix[14] = FixFrac(0)-translation[2];
}

void Matrix::scale( const FixFrac *scale )
{
	reset();

	m_matrix[0] = scale[0];
	m_matrix[5] = scale[1];
	m_matrix[10] = scale[2];
	m_matrix[15] = 1;
}

void Matrix::rotdeg( const FixFrac *angles )
{
	FixFrac vec[3];
	vec[0] = ( angles[0]*180.0/M_PI );
	vec[1] = ( angles[1]*180.0/M_PI );
	vec[2] = ( angles[2]*180.0/M_PI );
	rotrad( vec );
}

void Matrix::invrotdeg( const FixFrac *angles )
{
	FixFrac vec[3];
	vec[0] = ( angles[0]*180.0/M_PI );
	vec[1] = ( angles[1]*180.0/M_PI );
	vec[2] = ( angles[2]*180.0/M_PI );
	invrotrad( vec );
}

void Matrix::rotrad( const FixFrac *angles )
{
	FixFrac cr = cos( angles[0] );
	FixFrac sr = sin( angles[0] );
	FixFrac cp = cos( angles[1] );
	FixFrac sp = sin( angles[1] );
	FixFrac cy = cos( angles[2] );
	FixFrac sy = sin( angles[2] );

	m_matrix[0] = ( cp*cy );
	m_matrix[1] = ( cp*sy );
	m_matrix[2] = ( FixFrac(0)-sp );

	FixFrac srsp = sr*sp;
	FixFrac crsp = cr*sp;

	m_matrix[4] = ( srsp*cy-cr*sy );
	m_matrix[5] = ( srsp*sy+cr*cy );
	m_matrix[6] = ( sr*cp );

	m_matrix[8] = ( crsp*cy+sr*sy );
	m_matrix[9] = ( crsp*sy-sr*cy );
	m_matrix[10] = ( cr*cp );
}

void Matrix::invrotrad( const FixFrac *angles )
{
	FixFrac cr = cos( angles[0] );
	FixFrac sr = sin( angles[0] );
	FixFrac cp = cos( angles[1] );
	FixFrac sp = sin( angles[1] );
	FixFrac cy = cos( angles[2] );
	FixFrac sy = sin( angles[2] );

	m_matrix[0] = ( cp*cy );
	m_matrix[4] = ( cp*sy );
	m_matrix[8] = ( FixFrac(0)-sp );

	FixFrac srsp = sr*sp;
	FixFrac crsp = cr*sp;

	m_matrix[1] = ( srsp*cy-cr*sy );
	m_matrix[5] = ( srsp*sy+cr*cy );
	m_matrix[9] = ( sr*cp );

	m_matrix[2] = ( crsp*cy+sr*sy );
	m_matrix[6] = ( crsp*sy-sr*cy );
	m_matrix[10] = ( cr*cp );
}

void Matrix::rotquat( const Quaternion& quat )
{
	m_matrix[0] = ( FixFrac(1.0f) - FixFrac(2)*quat[1]*quat[1] - FixFrac(2)*quat[2]*quat[2] );
	m_matrix[1] = ( FixFrac(2)*quat[0]*quat[1] + FixFrac(2)*quat[3]*quat[2] );
	m_matrix[2] = ( FixFrac(2)*quat[0]*quat[2] - FixFrac(2)*quat[3]*quat[1] );

	m_matrix[4] = ( FixFrac(2)*quat[0]*quat[1] - FixFrac(2)*quat[3]*quat[2] );
	m_matrix[5] = ( FixFrac(1.0f) - FixFrac(2)*quat[0]*quat[0] - FixFrac(2)*quat[2]*quat[2] );
	m_matrix[6] = ( FixFrac(2)*quat[1]*quat[2] + FixFrac(2)*quat[3]*quat[0] );

	m_matrix[8] = ( FixFrac(2)*quat[0]*quat[2] + FixFrac(2)*quat[3]*quat[1] );
	m_matrix[9] = ( FixFrac(2)*quat[1]*quat[2] - FixFrac(2)*quat[3]*quat[0] );
	m_matrix[10] = ( FixFrac(1.0f) - FixFrac(2)*quat[0]*quat[0] - FixFrac(2)*quat[1]*quat[1] );
}

