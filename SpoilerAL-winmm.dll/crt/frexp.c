
typedef struct _ieee_double {
	unsigned __int64 mantissa : 52;
	unsigned __int64 exp      : 11;
	unsigned __int64 sign     :  1;
} ieee_double;

#define DBL_EXP_BIAS 1023

double __cdecl frexp(double x, int *expptr)
{
	if (x) {
		*expptr = (int)((ieee_double *)&x)->exp - (DBL_EXP_BIAS - 1);
		((ieee_double *)&x)->exp = DBL_EXP_BIAS - 1;
		return x;
	}
	else {
		*expptr = 0;
		return 0;
	}
}
