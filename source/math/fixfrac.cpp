

#include "fixfrac.h"
#include "../utils.h"
#include "fixmath.h"


FixFrac pow(FixFrac x, FixFrac y)
{
	//y must be whole integer
	FixFrac i = ToFrac(0);
	FixFrac r = x;

	while(i < y)
	{
		r = r * x;
		i = i + ToFrac(1);
	}

	return r;
}

FixFrac fac(FixFrac x)
{
	//x must be whole integer
	FixFrac i = (x);
	FixFrac r = ToFrac(1);

	while(i > 0)
	{
		r = r * i;
		i = i - ToFrac(1);
	}

	return r;
}

#if 01
INTBASIS igcd(INTBASIS num1, INTBASIS num2)
{
	INTBASIS tmp;
	//num1 = Abs(num1);
	//num2 = Abs(num2);
	num1 = (num1);
	num2 = (num2);
	while (num1 > INTBASIS(0)) {
		tmp = num1;
		num1 = num2 % num1;
		num2 = tmp;
	}
	return num2;
}
#endif

void reduce(FixFrac *f)
{
#if 01
	INTBASIS gcd;
	gcd = igcd(f->n, f->d);
	f->n = f->n / gcd;
	f->d = f->d / gcd;
#endif
}
#if 01
FixFrac fsign(FixFrac f)
{
	FixFrac s = INTBASIS(1);

	//if(f.d < 0)
	//	s = -s;
	//if(f.n < 0)
	//	s = -s;
	//if(f.neg)
	//	s = -s;
	s.neg = f.neg;
	//if(s < 0)
	//	s = -s;

	return s;
}
#endif
//DBL_DECIMAL_DIG=10
#if 1
FixFrac ToFrac(double d)
{
	//if(d < 0)
	//exit(0);
		//InfoMess("<","<");
#if 01
	FixFrac f;
	//f.n = INTBASIS(d*(-1+2*(d>=0)) * RATIO_DENOM);
	if((d)<(0))
		f.neg = true;
#elif 0
	FixFrac f = d;
#endif
	f.n = INTBASIS( ((f.neg)?-1:1)*d * 10000000000);
	f.d = INTBASIS(10000000000);
	return f;
}
#endif
FixFrac ToFrac(int64_t d)
{
#if 0
	FixFrac f;
	f.n = INTBASIS(d*(-1+2*(d>=0)) * RATIO_DENOM);
	if((d)<(0))
		f.neg = true;
#else
	FixFrac f;
	if(d<0)
		f.neg = true;
	f.n = INTBASIS( ((f.neg)?-1:1)*d );
	//FixFrac f = d;
#endif
	//f.n = d * 10000000000;
	//f.d = 10000000000;
	return f;
}

FixFrac ToFrac(int d)
{
	FixFrac f;
	if(d<0)
		f.neg = true;
	f.n = INTBASIS( ((f.neg)?-1:1)*d );
	return f;
}
#if 1
FixFrac ToFrac(INTBASIS d)
{
	FixFrac f;
	//f.n = d * INTBASIS(RATIO_DENOM);
	f.n = d;
	f.d = INTBASIS(1);
	f.neg = false;
	//if(fsign(d)<0)
	//	f.neg = true;
	//f.n = d * 10000000000;
	//f.d = 10000000000;
	return f;
}
#endif
FixFrac Abs(FixFrac f)
{
	//if(f.n < 0)
	//	f.n = -f.n;
	//if(f.d < 0)
	//	f.d = -f.d;
	//f.n = INTBASIS(1);
	f.neg = false;
	//if(f < 0)
	//	f = -f;
	return f;
}

int64_t ToBasis(FixFrac f)
{
	//removes n/0 possibility
	//f = simplify(f);
	//return (int64_t)((f.n / INTBASIS(RATIO_DENOM))/*.toDouble()*/) * (-1+2*(int)f.neg);
	return ((f.neg?-1:1)*f.n.toDouble()/f.d.toDouble());
	//return f;
}

//preserves fractional
double ToBasis2(FixFrac f)
{
	return ((f.neg?-1:1)*f.n.toDouble()/f.d.toDouble());
	//return (double)f.n/*.toDouble()*/ / (double)RATIO_DENOM * (-1+2*(int)f.neg);
}

FixFrac llmin(FixFrac a, FixFrac b)
{
#ifdef PLATFORM_WIN
	return enmin(a,b);
#else
	return std::min(a,b);
#endif
	//return (a.n<b.n)?(((int)a.neg>(int)b.neg)?a:b):(((int)b.neg>(int)a.neg)?a:b);
}

FixFrac llmax(FixFrac a, FixFrac b)
{
#ifdef PLATFORM_WIN
	return enmax(a,b);
#else
	return std::max(a,b);
#endif
	//return (a.n>b.n)?(((int)a.neg<(int)b.neg)?a:b):(((int)b.neg<(int)a.neg)?a:b);
}

//http://stackoverflow.com/questions/19611198/finding-square-root-without-using-sqrt-function
#if 01
FixFrac sqrtf(FixFrac num)
{
#if 1
	FixFrac lower_bound=0; 
	FixFrac upper_bound=num;
	FixFrac temp=0;                    /* ek edited this line */

	int nCount = 5000;

	while(nCount != 0)
	{
		temp=(lower_bound+upper_bound)/2;
		if(temp*temp==num)
		{
			return temp;
		}
		else if(temp*temp > num)
		{
			upper_bound = temp;
		}
		else
		{
			lower_bound = temp;
		}
		nCount--;
	}
	return temp;
#else
	return ToFrac(isqrt(ToBasis(num)));
#endif
}
#endif
#if 0
FixFrac sqrtf(FixFrac x) {
    if (x <= 0)
        return 0;       // if negative number throw an exception?
    int exp = 0;
    x = frexp(x, &exp); // extract binary exponent from x
    if (exp & 1) {      // we want exponent to be even
        exp--;
        x *= 2;
    }
    FixFrac y = (1+x)/2; // first approximation
    FixFrac z = 0;
    while (y != z) {    // yes, we CAN compare FixFracs here!
        z = y;
        y = (y + x/y) / 2;
    }
    return ldexp(y, exp/2); // multiply answer by 2^(exp/2)
}
#endif

#if 0
//https://people.freebsd.org/~jake/frexp.c
/*
 * Split the given value into a FixFrac in the range [0.5, 1.0) and
 * an exponent, such that frac * (2^exp) == value.  If value is 0,
 * return 0.
 */
FixFrac
frexp(FixFrac value, int* eptr)
{
	union {
                FixFrac v;
		struct ieee_double s;
	} u;

	if (value) {
		/*
		 * FixFracs in [0.5..1.0) have an exponent of 2^-1.
		 * Leave Inf and NaN alone, however.
		 * WHAT ABOUT DENORMS?
		 */
		u.v = value;
		//if (u.s.dbl_exp != DBL_EXP_INFNAN)
		{
			//DBL_EXP_BIAS=1023
			*eptr = u.s.dbl_exp - (1023 - 1);
			u.s.dbl_exp = 1023 - 1;
		}
		return (u.v);
	} else {
		*eptr = 0;
		return (0.0);
	}
}
#endif

#if 01
FixFrac simplify(FixFrac f)
{
	if(f.d == INTBASIS(0))
	{
		f.d = INTBASIS(1);
		//if(f.n > INTBASIS(0))
		//	f.n = BIGN-1;
		//else
		//	f.n = -BIGN+1;
		return f;
	}
	reduce(&f);
	while( (Abs(f.n) > BIGN || Abs(f.d) > BIGN) && 
		(Abs(f.n) > 2 && Abs(f.d) > 2) )
	{
		f.n >>= 1;
		f.d >>= 1;
	}
	return f;	
}
#endif

#if 01
bool _isnan(FixFrac f)
{
	return false;
}
#endif

	//FixFrac::FixFrac(FixFrac f)
	//{
	//	*this = f;
	//}
	#if 01
	FixFrac::FixFrac(int64_t i)
	{
		*this = ToFrac((i));
	}
#if 1
	FixFrac::FixFrac(INTBASIS i)
	{
		*this = ToFrac(i);
	}
#endif
	FixFrac::FixFrac(double i)
	{
		*this = ToFrac(i);
	}
	FixFrac::FixFrac(int i)
	{
		neg = (i<0)?true:false;
		n = INTBASIS(i);
		d = INTBASIS(1);
	}
	bool FixFrac::operator==(const FixFrac b) const
	{
		//return (n == b.n && d == b.d);
		return (n == b.n && neg == b.neg) || (n==INTBASIS(0) && b.n == INTBASIS(0));
	}
	bool FixFrac::operator!=(const FixFrac b) const
	{
		//return (n != b.n || d != b.d);
		return (n != b.n || ( n==b.n && n!=INTBASIS(0) && neg!=b.neg));
	}
	
	bool FixFrac::operator>(const FixFrac b) const
	{
#if 0
		FixFrac a2 = simplify(*this);
		FixFrac b2 = simplify(b);
		INTBASIS neara = a2.n/a2.d;
		INTBASIS nearb = b2.n/b2.d;
		INTBASIS mul = 2;
		while(neara == 0 && nearb == 0 && mul < BIGN)
		{
			FixFrac ch;
			ch.n = mul;
			ch.d = 1;
			ch = simplify(ch);
			a2 = a2 * ch;
			b2 = b2 * ch;
			neara = a2.n/a2.d;
			nearb = b2.n/b2.d;
			mul <<= 1;
		}
		return (neara > nearb);
#else
		return (n>(b.n)&&!neg&&!b.neg) ||
			(neg&&b.neg&&n<b.n) ||
			(!neg&&b.neg);
		//return (llmin(*this,b)==b);
		//return (this->n > b.n);
#endif
	}
	
	bool FixFrac::operator>=(const FixFrac b) const
	{
		//return (*this > b || *this == b);
		//return (this->n >= b.n);
		return ((*this)>b)||(*this)==b;
	}
	
	bool FixFrac::operator<(const FixFrac b) const
	{
#if 0
		FixFrac a2 = simplify(*this);
		FixFrac b2 = simplify(b);
		INTBASIS neara = a2.n/a2.d;
		INTBASIS nearb = b2.n/b2.d;
		INTBASIS mul = 2;
		while(neara == 0 && nearb == 0 && mul < BIGN)
		{
			FixFrac ch;
			ch.n = mul;
			ch.d = 1;
			ch = simplify(ch);
			a2 = a2 * ch;
			b2 = b2 * ch;
			neara = a2.n/a2.d;
			nearb = b2.n/b2.d;
			mul <<= 1;
		}
		return (neara < nearb);
#else
		//return (this->n < b.n);
		//return (llmax(*this,b)==b);
		return (n<(b.n)&&!neg&&!b.neg) ||
			(neg&&b.neg&&n>b.n) ||
			(neg&&!b.neg);
#endif
	}

	bool FixFrac::operator<=(const FixFrac b) const
	{
		//return (*this < b || *this == b);
		//return (this->n <= b.n);
		return (*this)<b||(*this)==b;
	}

	FixFrac operator*(const FixFrac a, const FixFrac b)
	{
#if 0
		FixFrac a2 = simplify(*this);
		FixFrac b2 = simplify(b);
		FixFrac c;
		c.n = b2.n * a2.n;
		c.d = b2.d * a2.d;
		return simplify(c);
#endif	//TODO all these:
		FixFrac r;
		r.n = a.n * b.n / INTBASIS(RATIO_DENOM);
		r.d = a.d * b.d;
		r.neg = (a.neg != b.neg);
		return r;
	}
	//http://stackoverflow.com/questions/32199574/error-c2678-binary-no-operator-found-which-takes-a-left-hand-operand-of
	FixFrac operator*(const FixFrac a, const double b)
	{
		FixFrac r;
		r.n = INTBASIS( a.n.toDouble() * b / INTBASIS(RATIO_DENOM).toDouble() );
		r.d = INTBASIS( a.d.toDouble() * b );
		r.neg = (a.neg != (b<0));
		return r;
		//return (*this)*FixFrac(b);
	}
	FixFrac operator/(const FixFrac a, const FixFrac b)
	{
#if 0
		FixFrac a2 = simplify(*this);
		FixFrac b2 = simplify(b);
		FixFrac c;
		c.n = b2.d * a2.n;
		c.d = b2.n * a2.d;
		return simplify(c);
#endif
		FixFrac r;
		if(b.n == INTBASIS(0) || a.n == INTBASIS(0))
			//r.n = BIGN * fsign(r.n);
			r.n = INTBASIS(0);
		else
			r.n = a.n * RATIO_DENOM / b.n;
		r.d = a.d * RATIO_DENOM / b.d;
		r.neg = (a.neg != b.neg);
		return r;
	}
	//b%dv = b-(b/dv)*dv
	//better to define b%dv = (0>=|b|>dv)
	//while(|b|>=dv) { if(b>=0) b-=dv else b+=dv }	dv>0
	FixFrac FixFrac::operator%(const FixFrac b) const
	{
#if 0
		FixFrac a2 = simplify(*this);
		FixFrac b2 = simplify(Abs(b));
		//FixFrac c = a2;
		FixFrac z;
		z.n = 0;
		z.d = 1;
		while(Abs(a2)>=b2)
		{
			if(b2>=z)
				a2 = a2 - b2;
			else
				a2 = a2 + b2;
		}
		return simplify(a2);
#else
		FixFrac a2 = *this;	//simplify(*this);
		FixFrac b2 = Abs(b);	//simplify(Abs(b));
		//FixFrac c = a2;
		FixFrac z;
		z.n = INTBASIS(0);
		//z.d = 1;
		while(Abs(a2)>=b2)
		{
			if(b2>=z)
				a2 = a2 - b2;
			else
				a2 = a2 + b2;
		}
		return a2;
		//return simplify(a2);
#endif
	}
	FixFrac FixFrac::operator-(const FixFrac b) const
	{
#if 0
		FixFrac a2 = simplify(*this);
		FixFrac b2 = simplify(b);
		FixFrac c;
		c.n = (b2.d * a2.n) - (a2.d * b2.n);
		c.d = b2.d * a2.d;
		return simplify(c);
#endif
		FixFrac r;
		//r.n = n - b.n;

#if 0
		if(*this > b)
		{
			if(this->neg)
			{
				if(b.neg)
				{
					r.n = this->n + b.n;
					r.neg = true;
				}
				else
				{
					//this->n = -this->n;
					r.n = -(this->n) + b.n;
					r.neg = true;
					//this->n = -this->n;
				}
			}
			else
			{
				if(b.neg)
				{
					//b.n = -b.n;
					r.neg = false;
					r.n = this->n + -(b.n);
				}
				else
				{
					r.neg = false;
					r.n = this->n + b.n;
				}
			}	
		}
		else	//b>this
		{
			if(this->neg)
			{
				if(b.neg)
				{
					r.n = -(n) + b.n;
					r.neg = true;
				}
				else
				{
					
					r.neg = false;
				}
			}
		}

		int agreater = (*this>b);
		int aneg = (-1+2*neg);
		int bneg = (-1+2*b.neg);
#endif
		//-3 - -1 = -2
		//-3 - +1 = -4
		//+3 - -1 = +4
		//+3 - +1 = +2
		//-1 - -3 = +2
		//-1 - +3 = -4
		//+1 - -3 = +4
		//+1 - +3 = -2
		//r.neg = (Abs(b)>Abs(*this)&&(!b.neg)||((b.neg)&&(Abs(*this)>Abs(b)))||
		//	(this->neg&&());
		r.neg = (this->neg&&((this->n)>(b.n))) || (!b.neg&&((b.n)>(this->n)));

		FixFrac aadd = *this;
		FixFrac badd = b;

		//if(neg&&(this->n<b.n))
		if(neg)
			aadd.n = -aadd.n;
		//if(b.neg&&(b.n<this->n))
		if(b.neg)
			badd.n = -badd.n;
		badd.n = -badd.n;

		r.n = aadd.n+badd.n;

		if(r.neg)
			r.n = -r.n;
		return r;
		return r;



	}
	FixFrac operator-(const FixFrac a)
	{
		return (FixFrac(0)-a);
	}
	FixFrac FixFrac::operator+(const FixFrac b) const
	{
#if 0
		FixFrac a2 = simplify(*this);
		FixFrac b2 = simplify(b);
		FixFrac c;
		c.n = (b2.d * a2.n) + (a2.d * b2.n);
		c.d = b2.d * a2.d;
		return simplify(c);
#endif
		FixFrac r;
		//r.n = n + b.n;


		//-3 + -1 = -4
		//-3 + +1 = -2
		//+3 + -1 = +2
		//+3 + +1 = +4
		//-1 + -3 = -4
		//-1 + +3 = +2
		//+1 + -3 = -2
		//+1 + +3 = +4

		//r.neg = (neg&&b.neg)||(this->neg&&(this->n>b.n))||(b.neg&&(b.n>this->n));
		r.neg = (neg&&b.neg)||(neg&&(this->n)>(b.n))||(b.neg&&(b.n)>(this->n));

		FixFrac aadd = *this;
		FixFrac badd = b;

		//if(neg&&(this->n<b.n))
		if(neg)
			aadd.n = -aadd.n;
		//if(b.neg&&(b.n<this->n))
		if(b.neg)
			badd.n = -badd.n;

		r.n = aadd.n+badd.n;

		if(r.neg)
			r.n = -r.n;
		return r;
	}

	FixFrac FixFrac::operator+=( const FixFrac b)
	{
	//	return (*this) = (*this)+(b);
		FixFrac r;
		r = *this + b;
		return (*this) = r;
	}
	FixFrac FixFrac::operator*=( const FixFrac b)
	{
		return (*this) = (*this)*(b);
	}
	FixFrac FixFrac::operator*=( const double b)
	{
		return (*this) = (*this)*ToFrac(b);
	}
	
	FixFrac FixFrac::operator/=( const FixFrac b)
	{
		return (*this) = (*this)/(b);
	}
#endif
