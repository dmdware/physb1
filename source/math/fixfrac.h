

#ifndef FIXFRAC_H
#define FIXFRAC_H

#include "../platform.h"
#include "bigInt.h"

#define INTBASIS	BigInt::Rossi
//#define INTBASIS	uint64_t
//#define INTBASIS	int128_t
//typedef int64_t INTBASIS;
//#define INTBASIS	int32_t
//typedef long double INTBASIS;

#define GLTYPE		GL_FLOAT

#define BIGN		INTBASIS((uint64_t(0)-1)/1024/1024/1024/512)
//#define BIGN		(uint32_t(-1)/1024/1024)
//

//#define .toDouble()	*1

#if 01
class FixFrac
{
public:
#if 0
	INTBASIS n;
	INTBASIS d;

	FixFrac()
	{
		n = 0;
		d = 1;
	}
	FixFrac(INTBASIS i)
	{
		n = i;
		d = 1;
	}
#else
	bool neg;
	INTBASIS n;
	INTBASIS d;

	FixFrac()
	{
		neg = false;
		n = INTBASIS(0);
		d = INTBASIS(1);
	}
	//FixFrac(FixFrac f);
	FixFrac(INTBASIS i);
	FixFrac(double i);
	FixFrac(int64_t i);
	FixFrac(int i);
#endif	

	bool operator==(const FixFrac b) const;
	bool operator!=(const FixFrac b) const;
	
	bool operator>(const FixFrac b) const;
	
	bool operator>=(const FixFrac b) const;
	
	bool operator<(const FixFrac b) const;
	bool operator<=(const FixFrac b) const;
	//b%dv = b-(b/dv)*dv
	//better to define b%dv = (0>=|b|>dv)
	//while(|b|>=dv) { if(b>=0) b-=dv else b+=dv }	dv>0
	FixFrac operator%(const FixFrac b) const;
	FixFrac operator-(const FixFrac b) const;
	FixFrac operator+(const FixFrac b) const;
	//this gives problem in g++, saying it must have one argument, but says "*this" is not
	//a member of the class if trying to use one in .cpp
	//FixFrac& operator +=(FixFrac& a, const FixFrac& b);
	//FixFrac& operator *=(FixFrac& a, const FixFrac& b);
	//FixFrac& operator *=(FixFrac& a, const double& b);
	FixFrac operator +=(const FixFrac b);
	FixFrac operator *=(const FixFrac b);
	FixFrac operator *=(const double b);
	FixFrac operator /=(const FixFrac b);
};
#endif

	FixFrac operator-(const FixFrac a);
	FixFrac operator*(const FixFrac a, const FixFrac b);
	FixFrac operator*(const FixFrac a, const double b);
	FixFrac operator/(const FixFrac a, const FixFrac b);

//typedef float FixFrac;

int64_t ToBasis(FixFrac f);
double ToBasis2(FixFrac f);
FixFrac simplify(FixFrac f);
FixFrac ToFrac(double d);
FixFrac ToFrac(INTBASIS d);
FixFrac ToFrac(int64_t d);
FixFrac ToFrac(int d);
FixFrac Abs(FixFrac f);
FixFrac sqrtf(FixFrac f);
FixFrac frexp(FixFrac value, int* eptr);
FixFrac fsign(FixFrac f);
FixFrac llmin(FixFrac a, FixFrac b);
FixFrac llmax(FixFrac a, FixFrac b);
bool _isnan(FixFrac f);
FixFrac pow(FixFrac x, FixFrac y);
FixFrac fac(FixFrac x);

#endif
