

#ifndef FIXTRIG_H
#define FIXTRIG_H

#include "fixmath.h"
#include "fixfrac.h"

#if 01
//#define TABLESZ		1024
#define TABLESZ		RATIO_DENOM

//(0,2*M_PI)->(-1,1)
extern FixFrac COSTABLE[TABLESZ];
extern FixFrac SINTABLE[TABLESZ];
extern FixFrac TANTABLE[TABLESZ];
//(-1,1)->(0,2*M_PI)
extern FixFrac ACOSTABLE[TABLESZ];
extern FixFrac ASINTABLE[TABLESZ];
extern FixFrac ATANTABLE[TABLESZ];

FixFrac cos(FixFrac rad);
FixFrac sin(FixFrac rad);
FixFrac tan(FixFrac rad);
FixFrac acos(FixFrac ratio);
FixFrac asin(FixFrac ratio);
FixFrac atan(FixFrac ratio);
FixFrac atan2(FixFrac x, FixFrac y);
void FillTrig();
#endif

#endif
