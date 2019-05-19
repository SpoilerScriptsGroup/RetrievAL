#include <windows.h>
#include <math.h>

#undef signbit
#define signbit(x) (*(__int64 *)&(x) < 0)

typedef enum {
	_FpCodeUnspecified,
	_FpCodeAdd,
	_FpCodeSubtract,
	_FpCodeMultiply,
	_FpCodeDivide,
	_FpCodeSquareRoot,
	_FpCodeRemainder,
	_FpCodeCompare,
	_FpCodeConvert,
	_FpCodeRound,
	_FpCodeTruncate,
	_FpCodeFloor,
	_FpCodeCeil,
	_FpCodeAcos,
	_FpCodeAsin,
	_FpCodeAtan,
	_FpCodeAtan2,
	_FpCodeCabs,
	_FpCodeCos,
	_FpCodeCosh,
	_FpCodeExp,
	_FpCodeFabs,
	_FpCodeFmod,
	_FpCodeFrexp,
	_FpCodeHypot,
	_FpCodeLdexp,
	_FpCodeLog,
	_FpCodeLog10,
	_FpCodeModf,
	_FpCodePow,
	_FpCodeSin,
	_FpCodeSinh,
	_FpCodeTan,
	_FpCodeTanh,
	_FpCodeY0,
	_FpCodeY1,
	_FpCodeYn,
	_FpCodeLogb,
	_FpCodeNextafter,
	_FpCodeNegate,
	_FpCodeFmin,
	_FpCodeFmax,
	_FpCodeConvertTrunc,
	_XMMIAddps,
	_XMMIAddss,
	_XMMISubps,
	_XMMISubss,
	_XMMIMulps,
	_XMMIMulss,
	_XMMIDivps,
	_XMMIDivss,
	_XMMISqrtps,
	_XMMISqrtss,
	_XMMIMaxps,
	_XMMIMaxss,
	_XMMIMinps,
	_XMMIMinss,
	_XMMICmpps,
	_XMMICmpss,
	_XMMIComiss,
	_XMMIUComiss,
	_XMMICvtpi2ps,
	_XMMICvtsi2ss,
	_XMMICvtps2pi,
	_XMMICvtss2si,
	_XMMICvttps2pi,
	_XMMICvttss2si,
	_XMMIAddsubps,
	_XMMIHaddps,
	_XMMIHsubps,
	_XMMI2Addpd,
	_XMMI2Addsd,
	_XMMI2Subpd,
	_XMMI2Subsd,
	_XMMI2Mulpd,
	_XMMI2Mulsd,
	_XMMI2Divpd,
	_XMMI2Divsd,
	_XMMI2Sqrtpd,
	_XMMI2Sqrtsd,
	_XMMI2Maxpd,
	_XMMI2Maxsd,
	_XMMI2Minpd,
	_XMMI2Minsd,
	_XMMI2Cmppd,
	_XMMI2Cmpsd,
	_XMMI2Comisd,
	_XMMI2UComisd,
	_XMMI2Cvtpd2pi,
	_XMMI2Cvtsd2si,
	_XMMI2Cvttpd2pi,
	_XMMI2Cvttsd2si,
	_XMMI2Cvtps2pd,
	_XMMI2Cvtss2sd,
	_XMMI2Cvtpd2ps,
	_XMMI2Cvtsd2ss,
	_XMMI2Cvtdq2ps,
	_XMMI2Cvttps2dq,
	_XMMI2Cvtps2dq,
	_XMMI2Cvttpd2dq,
	_XMMI2Cvtpd2dq,
	_XMMI2Addsubpd,
	_XMMI2Haddpd,
	_XMMI2Hsubpd,
} _FP_OPERATION_CODE;

double __cdecl _except1(DWORD fpe, _FP_OPERATION_CODE op, double arg, double res, DWORD cw, void *unk)
{
	ULONG_PTR exception_arg;
	DWORD exception = 0;
	WORD operation;

#ifdef _WIN64
	cw = ((cw >> 7) & 0x3f) | ((cw >> 3) & 0xc00);
#endif
	operation = op << 5;
	exception_arg = (ULONG_PTR)&operation;

	if (fpe & 0x1)
	{
		if ((fpe != 0x1 || !(cw & 0x8)) && (fpe != 0x11 || !(cw & 0x28)))
			exception = EXCEPTION_FLT_OVERFLOW;
		else
			res = signbit(res) ? -INFINITY : INFINITY;
	}
	else if (fpe & 0x2)
	{
		if ((fpe != 0x2 || !(cw & 0x10)) && (fpe != 0x12 || !(cw & 0x30)))
			exception = EXCEPTION_FLT_UNDERFLOW;
		else
			res = signbit(res) ? -0.0 : 0.0;
	}
	else if (fpe & 0x4)
	{
		if ((fpe != 0x4 || !(cw & 0x4)) && (fpe != 0x14 || !(cw & 0x24)))
			exception = EXCEPTION_FLT_DIVIDE_BY_ZERO;
	}
	else if (fpe & 0x8)
	{
		if (fpe != 0x8 || !(cw & 0x1))
			exception = EXCEPTION_FLT_INVALID_OPERATION;
	}
	else if (fpe & 0x10)
	{
		if (fpe != 0x10 || !(cw & 0x20))
			exception = EXCEPTION_FLT_INEXACT_RESULT;
	}

	if (exception)
		RaiseException(exception, 0, 1, &exception_arg);

	return res;
}
