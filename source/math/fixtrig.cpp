

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

