#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <errno.h>

double __cdecl ldexp(double x, int exp)
{
	if (!_isnan(x))
	{
		if (x)
		{
			unsigned char sign;

			sign = x < 0;
			if (sign)
			{
				x = -x;
			}
			if (x <= DBL_MAX)
			{
				int currexp;

				x = frexp(x, &currexp);
				exp += currexp;
				if (exp > 0)
				{
					if (exp <= DBL_MAX_EXP)
					{
						while (exp > 30)
						{
							x *= (double)(1L << 30);
							exp -= 30;
						}
						x *= (double)(1L << exp);
					}
					else
					{
						errno = ERANGE;
						x = HUGE_VAL;
					}
				}
				else
				{
					if (exp >= DBL_MIN_EXP - DBL_MANT_DIG)
					{
						while (exp < -30)
						{
							x /= (double)(1L << 30);
							exp += 30;
						}
						x /= (double)(1L << -exp);
					}
					else
					{
						x = 0;
						sign = 0;
					}
				}
			}
			else
			{
				errno = ERANGE;
			}
			if (sign)
			{
				x = -x;
			}
		}
		else
		{
			x = 0;
		}
	}
	else
	{
		errno = EDOM;
	}
	return x;
}
#else
__declspec(naked) double __cdecl ldexp(double x, int exp)
{
	__asm
	{
		emms
		fild    dword ptr [esp + 12]    ; Load exp as integer
		fld     qword ptr [esp + 4]     ; Load real from stack
		fscale                          ; Compute 2 to the x
		fstp    st(1)                   ; Set new top of stack
		ret
	}
}
#endif
