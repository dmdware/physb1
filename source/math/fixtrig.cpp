

#include "fixtrig.h"

#if 0
//(0,2*M_PI)->(-1,1)
FixFrac COSTABLE[TABLESZ];
FixFrac SINTABLE[TABLESZ];
FixFrac TANTABLE[TABLESZ];
//(-1,1)->(0,2*M_PI)
FixFrac ACOSTABLE[TABLESZ];
FixFrac ASINTABLE[TABLESZ];
FixFrac ATANTABLE[TABLESZ];

int FromRadToIndex(double r)
{
	return ((TABLESZ+(int)(r/M_PI/2.0*TABLESZ)%TABLESZ)%TABLESZ);
}

int FromRatioToIndex(double r)
{
	return ((TABLESZ+(int)((1.0+r/2.0)*TABLESZ)%TABLESZ)%TABLESZ);
}

void FillTrig()
{
	for(int i=0; i<TABLESZ; ++i)
	{
		COSTABLE[i] = ToFrac(cos((double)i/(double)TABLESZ*M_PI*2.0));
		SINTABLE[i] = ToFrac(sin((double)i/(double)TABLESZ*M_PI*2.0));
		TANTABLE[i] = ToFrac(tan((double)i/(double)TABLESZ*M_PI*2.0));

		ACOSTABLE[i] = ToFrac(acos((double)(i-(TABLESZ/2))/(double)(TABLESZ/2)));
		ASINTABLE[i] = ToFrac(asin((double)(i-(TABLESZ/2))/(double)(TABLESZ/2)));
		ATANTABLE[i] = ToFrac(atan((double)(i-(TABLESZ/2))/(double)(TABLESZ/2)));
	}
}

FixFrac cos(FixFrac rad)
{
	//return ToBasis((FixFrac(TABLESZ)-((rad*FixFrac(TABLESZ))/ToFrac(M_PI*2.0)%FixFrac(TABLESZ))));
	
	//return (rad*FixFrac(TABLESZ))/ToFrac(M_PI*2.0)%FixFrac(TABLESZ);
	return COSTABLE[
	FromRadToIndex(ToBasis2(rad))
	];
}

FixFrac sin(FixFrac rad)
{
	return SINTABLE[
	FromRadToIndex(ToBasis2(rad))
	];
}

FixFrac tan(FixFrac rad)
{
	return TANTABLE[
	FromRadToIndex(ToBasis2(rad))
	];
}

FixFrac acos(FixFrac ratio)
{
	return ACOSTABLE[
	FromRatioToIndex(ToBasis2(ratio))
	];
}

FixFrac asin(FixFrac ratio)
{
	return ASINTABLE[
	FromRatioToIndex(ToBasis2(ratio))
	];
}

FixFrac atan(FixFrac ratio)
{
	return ATANTABLE[
	FromRatioToIndex(ToBasis2(ratio))
	];
}

FixFrac atan2(FixFrac x, FixFrac y)
{
	FixFrac ratio = x/y;
	return atan(ratio);
}
#endif


//https://en.wikibooks.org/wiki/Trigonometry/Power_Series_for_Cosine_and_Sine
FixFrac cos(FixFrac rad)
{
	FixFrac r = ToFrac(1);
	FixFrac end = ToFrac(1)/ToFrac(100000);
	FixFrac last = ToFrac(1);
	FixFrac i = ToFrac(2);

	while(last >= end)
	{
		r = r - pow(rad, i) / fac(i);
		i = i * ToFrac(2);
		
		last = pow(rad, i) / fac(i);
		r = r + last;
		i = i * ToFrac(2);
	}

	return r;
}

FixFrac sin(FixFrac rad)
{
	FixFrac r = ToFrac(1);
	FixFrac end = ToFrac(1)/ToFrac(100000);
	FixFrac last = ToFrac(1);
	FixFrac i = ToFrac(2);

	while(last >= end)
	{
		r = r - pow(rad, i) / fac(i);
		i = (i-ToFrac(1)) * ToFrac(2) + ToFrac(1);
		
		last = pow(rad, i) / fac(i);
		r = r + last;
		i = (i-ToFrac(1)) * ToFrac(2) + ToFrac(1);
	}
}

//http://math.stackexchange.com/questions/1238965/working-out-tan-x-using-sin-and-cos-expansion
FixFrac tan(FixFrac rad)
{
	//sinx = cosx tanx
	//sinx/cosx = tanx

	return (sin(x)/cos(x));
}

FixFrac acos(FixFrac ratio)
{
}

FixFrac asin(FixFrac ratio)
{
}

FixFrac atan(FixFrac ratio)
{
}


FixFrac atan2(FixFrac x, FixFrac y)
{
}

