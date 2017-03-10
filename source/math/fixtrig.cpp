

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

#define ATRIGSTEPS	10000000
FixFrac acos(FixFrac ratio)
{
	//range: 0, pi

/*
Find: acos(a) = Φ, a>=0

Given: a
a = xf
yf = √(1 - xf^2)
0≤Φ≤π/2
x0 = 1
y0 = 0

Then:

lim n→∞ ( Σi=1 to n ( √( (xi - xi-1)^2 + (yi - yi-1)^2 ) ) )

x(i) = x0 - xf*i/n

y(i) = √( 1 - x(i)^2 )
*/

	if(ratio >= FixFrac(0))
	{
		FixFrac n = FixFrac(ATRIGSTEPS);
		FixFrac i = FixFrac(0);
		FixFrac a = FixFrac(a);

		FixFrac lastx = FixFrac(1);
		FixFrac lasty = FixFrac(0);

		while(i < n)
		{
			FixFrac cury = i/n;
			FixFrac curx = sqrtf( FixFrac(1) - cury*cury );

			a = a + sqrtf( (curx-lastx)*(curx-lastx) + (cury-lasty)*(cury-lasty) );

			i=i+FixFrac(1);

			lastx = curx;
			lasty = cury;
		}

		return a;
	}

}

FixFrac asin(FixFrac ratio)
{
	//range: -pi/2 to pi/2

/*
Find: asin(o) = Φ, o>=0

Given: o
o = yf
xf = √(1 - yf^2)
0≤Φ≤π/2
x0 = 1
y0 = 0

Then:

lim n→∞ ( Σi=1 to n ( √( (xi - xi-1)^2 + (yi - yi-1)^2 ) ) )

y(i) = y0 + yf*i/n

x(i) = √( 1 - y(i)^2 )
*/

	if(ratio >= FixFrac(0))
	{
		FixFrac n = FixFrac(ATRIGSTEPS);
		FixFrac i = FixFrac(0);
		FixFrac a = FixFrac(a);

		FixFrac lastx = FixFrac(1);
		FixFrac lasty = FixFrac(0);

		while(i < n)
		{
			FixFrac cury = i/n;
			FixFrac curx = sqrtf( FixFrac(1) - cury*cury );

			a = a + sqrtf( (curx-lastx)*(curx-lastx) + (cury-lasty)*(cury-lasty) );

			i=i+FixFrac(1);

			lastx = curx;
			lasty = cury;
		}

		return a;
	}
/*
Find: asin(o) = Φ, o<0

Given: o
o = yf
xf = √(1 - yf^2)
-π/2≤Φ≤0
x0 = 1
y0 = 0

Then:

lim n→∞ ( Σi=1 to n ( -√( (xi - xi-1)^2 + (yi - yi-1)^2 ) ) )

y(i) = y0 + yf*i/n

x(i) = √( 1 - y(i)^2 )
*/

	else
	{
		FixFrac n = FixFrac(ATRIGSTEPS);
		FixFrac i = FixFrac(0);
		FixFrac a = FixFrac(a);

		FixFrac lastx = FixFrac(1);
		FixFrac lasty = FixFrac(0);

		while(i < n)
		{
			FixFrac cury = FixFrac(0) - i/n;
			FixFrac curx = sqrtf( FixFrac(1) - cury*cury );

			a = a - sqrtf( (curx-lastx)*(curx-lastx) + (cury-lasty)*(cury-lasty) );

			i=i+FixFrac(1);

			lastx = curx;
			lasty = cury;
		}

		return a;
	}
}

FixFrac atan(FixFrac ratio)
{
	//range: -pi/2 to pi/2

	return asin(ratio);
}


FixFrac atan2(FixFrac x, FixFrac y)
{
	//range: -pi to pi

	return atan(x);
}

