

#ifndef SIMDEF_H
#define SIMDEF_H

#include "../math/vec3f.h"

//#define HYP_DIMS	3
//#define REND_3PT_INS	1
//
#define HYP_DIMS	4
#define REND_3PT_INS	4	//4 3-plane intersections = 4 points
//#define REND_3PT_INS	1
//123,124,134,234
//#define HYP_DIMS	5
//#define REND_3PT_INS	10
//123,124,125,134,135,145,234,235,245,345
//#define HYP_DIMS	6
//#define REND_3PT_INS	20
//123,124,125,126,134,135,136,145,146,156,234,235,236,245,246,256,345,346,356,456
//#define HYP_DIMS	7
//#define REND_3PT_INS	35
//123,124,125,126,127,134,135,136,137,145,146,147,156,157,167,234,235,236,237,245,246,247,256,257,267,345,346,347,356,357,367,456,457,467,567

#define RESERVEDIMS	1

#define SECGV		((INTBASIS)(1.0/30.0))

//#define KMSC		((INTBASIS)(1.0/10.0))
//#define GVSC		((INTBASIS)KMSC*KMSC*KMSC*SECGV*SECGV)

//#define GVSC2		10000000000000000000000
//#define GVSC 		1
#define KMSC		(1/1)

#define GVER		((INTBASIS)637/KMSC)
#define GVEM		((INTBASIS)5.972/KMSC)

#define GVSR		((INTBASIS)690/KMSC)
#define GVSM		((INTBASIS)1.989/KMSC)

//earth-sun
#define ESR		((INTBASIS)150*10/KMSC)
#define ESV		((INTBASIS)300/KMSC)

//use this to deal with FixFrac precision issue by subtracting object positions instead of 
//supplying camera position to transform matrix
//#define		OFFSETVIEW

extern const char* AXNAME[HYP_DIMS];

void DefSim();
void Balance();
void MakeGrid(FixFrac* cen);

extern Vec3f axis[HYP_DIMS];

#endif
