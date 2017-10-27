#ifndef _LONGDOUBLE_H_
#define _LONGDOUBLE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdbool.h>
#include <stdint.h>

#define CW_EM_MASK       0x003F
#define CW_EM_INVALID    0x0001
#define CW_EM_DENORMAL   0x0002
#define CW_EM_ZERODIVIDE 0x0004
#define CW_EM_OVERFLOW   0x0008
#define CW_EM_UNDERFLOW  0x0010
#define CW_EM_INEXACT    0x0020
#define CW_EM_DEFAULT    0x003F
#define CW_RESERVED      0x0040
#define CW_RC_MASK       0x0C00
#define CW_RC_NEAR       0x0000
#define CW_RC_DOWN       0x0400
#define CW_RC_UP         0x0800
#define CW_RC_CHOP       0x0C00
#define CW_RC_DEFAULT    CW_RC_NEAR
#define CW_PC_MASK       0x0300
#define CW_PC_24         0x0100
#define CW_PC_53         0x0200
#define CW_PC_64         0x0300
#define CW_PC_DEFAULT    CW_PC_64
#define CW_IC_MASK       0x1000
#define CW_IC_PROJECTIVE 0x0000
#define CW_IC_AFFINE     0x1000
#define CW_IC_DEFAULT    CW_IC_PROJECTIVE

#define SW_C0 0x0100
#define SW_C1 0x0200
#define SW_C2 0x0400
#define SW_C3 0x4000

#ifdef __cplusplus
struct _longdouble;
uint16_t _fstcw();
void _fldcw(const uint16_t cw);
_longdouble _fldz();
_longdouble _fld1();
_longdouble _fldln2();
#define _fld_r4(x) _fld_r8((const float)(x))
_longdouble _fld_r8(const double x);
_longdouble _fld_i2(const int16_t x);
_longdouble _fld_i4(const int32_t x);
_longdouble _fld_i8(const int64_t x);
#define _fst_r4(x) (float)_fst_r8(x)
double _fst_r8(const _longdouble x);
int16_t _fst_i2(const _longdouble x);
int32_t _fst_i4(const _longdouble x);
int64_t _fst_i8(const _longdouble x);
_longdouble _fadd(const _longdouble x, const _longdouble y);
_longdouble _fsub(const _longdouble x, const _longdouble y);
_longdouble _fmul(const _longdouble x, const _longdouble y);
_longdouble _fdiv(const _longdouble x, const _longdouble y);
_longdouble _fmod(const _longdouble x, const _longdouble y);
_longdouble _finc(const _longdouble x);
_longdouble _fdec(const _longdouble x);
_longdouble _fchs(const _longdouble x);
_longdouble _fabs(const _longdouble x);
_longdouble _frndint(const _longdouble x);
uint16_t _ftst(const _longdouble x);
uint16_t _fcom(const _longdouble x, const _longdouble y);
#define _fcom_r4(x, y) _fcom_r8(x, (const float)(y))
uint16_t _fcom_r8(const _longdouble x, const double y);
uint16_t _fcom_i2(const _longdouble x, const int16_t y);
uint16_t _fcom_i4(const _longdouble x, const int32_t y);
uint16_t _fcom_i8(const _longdouble x, const int64_t y);
_longdouble _f2xm1(const _longdouble x);
_longdouble _fxtract(const _longdouble x, const _longdouble *expptr);
_longdouble _fscale(const _longdouble x, const _longdouble exp);
_longdouble _fyl2x(const _longdouble x, const _longdouble y);
#endif

typedef struct _longdouble {
	uint64_t mantissa;
	union {
		struct {
			uint16_t exponent : 15;
			uint16_t sign : 1;
		};
		uint16_t extension;
	};

#ifdef __cplusplus
	__inline _longdouble()                                  { mantissa = 0; extension = 0; }
	__inline _longdouble(const _longdouble& x)              { mantissa = x.mantissa; extension = x.extension; }
	__inline _longdouble(const float x)                     { *this = _fld_r4(x); }
	__inline _longdouble(const double x)                    { *this = _fld_r8(x); }
	__inline _longdouble(const bool x)                      { *this = _fld_i2(x); }
	__inline _longdouble(const int8_t x)                    { *this = _fld_i2(x); }
	__inline _longdouble(const int16_t x)                   { *this = _fld_i2(x); }
	__inline _longdouble(const int32_t x)                   { *this = _fld_i4(x); }
	__inline _longdouble(const int64_t x)                   { *this = _fld_i8(x); }
	__inline _longdouble operator++(int)                    { _longdouble x; x.mantissa = mantissa; x.extension = extension; *this = _finc(*this); return x; }
	__inline _longdouble operator--(int)                    { _longdouble x; x.mantissa = mantissa; x.extension = extension; *this = _fdec(*this); return x; }
	__inline _longdouble operator++()                       { return *this = _finc(*this); }
	__inline _longdouble operator--()                       { return *this = _fdec(*this); }
	__inline _longdouble operator+()                        { return *this; }
	__inline _longdouble operator-()                        { return _fchs(*this); }
	__inline bool operator!()                               { return (_ftst(*this) & (SW_C0 | SW_C2 | SW_C3)) == SW_C3; }
	__inline explicit operator bool()                       { return (_ftst(*this) & (SW_C0 | SW_C2 | SW_C3)) != SW_C3; }
	__inline explicit operator float()                      { return _fst_r4(*this); }
	__inline explicit operator double()                     { return _fst_r8(*this); }
	__inline explicit operator int8_t()                     { return (int8_t)_fst_i2(*this); }
	__inline explicit operator int16_t()                    { return _fst_i2(*this); }
	__inline explicit operator int32_t()                    { return _fst_i4(*this); }
	__inline explicit operator int64_t()                    { return _fst_i8(*this); }
	__inline _longdouble operator*(const _longdouble& x)    { return _fmul(*this, x); }
	__inline _longdouble operator*(const float x)           { return _fmul(*this, x); }
	__inline _longdouble operator*(const double x)          { return _fmul(*this, x); }
	__inline _longdouble operator*(const int8_t x)          { return _fmul(*this, x); }
	__inline _longdouble operator*(const int16_t x)         { return _fmul(*this, x); }
	__inline _longdouble operator*(const int32_t x)         { return _fmul(*this, x); }
	__inline _longdouble operator*(const int64_t x)         { return _fmul(*this, x); }
	__inline _longdouble operator/(const _longdouble& x)    { return _fdiv(*this, x); }
	__inline _longdouble operator/(const float x)           { return _fdiv(*this, x); }
	__inline _longdouble operator/(const double x)          { return _fdiv(*this, x); }
	__inline _longdouble operator/(const int8_t x)          { return _fdiv(*this, x); }
	__inline _longdouble operator/(const int16_t x)         { return _fdiv(*this, x); }
	__inline _longdouble operator/(const int32_t x)         { return _fdiv(*this, x); }
	__inline _longdouble operator/(const int64_t x)         { return _fdiv(*this, x); }
	__inline _longdouble operator%(const _longdouble& x)    { return _fmod(*this, x); }
	__inline _longdouble operator%(const float x)           { return _fmod(*this, x); }
	__inline _longdouble operator%(const double x)          { return _fmod(*this, x); }
	__inline _longdouble operator%(const int8_t x)          { return _fmod(*this, x); }
	__inline _longdouble operator%(const int16_t x)         { return _fmod(*this, x); }
	__inline _longdouble operator%(const int32_t x)         { return _fmod(*this, x); }
	__inline _longdouble operator%(const int64_t x)         { return _fmod(*this, x); }
	__inline _longdouble operator+(const _longdouble& x)    { return _fadd(*this, x); }
	__inline _longdouble operator+(const float x)           { return _fadd(*this, x); }
	__inline _longdouble operator+(const double x)          { return _fadd(*this, x); }
	__inline _longdouble operator+(const int8_t x)          { return _fadd(*this, x); }
	__inline _longdouble operator+(const int16_t x)         { return _fadd(*this, x); }
	__inline _longdouble operator+(const int32_t x)         { return _fadd(*this, x); }
	__inline _longdouble operator+(const int64_t x)         { return _fadd(*this, x); }
	__inline _longdouble operator-(const _longdouble& x)    { return _fsub(*this, x); }
	__inline _longdouble operator-(const float x)           { return _fsub(*this, x); }
	__inline _longdouble operator-(const double x)          { return _fsub(*this, x); }
	__inline _longdouble operator-(const int8_t x)          { return _fsub(*this, x); }
	__inline _longdouble operator-(const int16_t x)         { return _fsub(*this, x); }
	__inline _longdouble operator-(const int32_t x)         { return _fsub(*this, x); }
	__inline _longdouble operator-(const int64_t x)         { return _fsub(*this, x); }
	__inline bool operator<(const _longdouble& x)           { return (_fcom(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C0; }
	__inline bool operator<(const float x)                  { return (_fcom_r4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C0; }
	__inline bool operator<(const double x)                 { return (_fcom_r8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C0; }
	__inline bool operator<(const int16_t x)                { return (_fcom_i2(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C0; }
	__inline bool operator<(const int32_t x)                { return (_fcom_i4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C0; }
	__inline bool operator<(const int64_t x)                { return (_fcom_i8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C0; }
	__inline bool operator<=(const _longdouble& x)          { uint16_t sw = _fcom(*this, x) & (SW_C0 | SW_C2 | SW_C3); return sw == SW_C0 || sw == SW_C3; }
	__inline bool operator<=(const float x)                 { uint16_t sw = _fcom_r4(*this, x) & (SW_C0 | SW_C2 | SW_C3); return sw == SW_C0 || sw == SW_C3; }
	__inline bool operator<=(const double x)                { uint16_t sw = _fcom_r8(*this, x) & (SW_C0 | SW_C2 | SW_C3); return sw == SW_C0 || sw == SW_C3; }
	__inline bool operator<=(const int16_t x)               { uint16_t sw = _fcom_i2(*this, x) & (SW_C0 | SW_C2 | SW_C3); return sw == SW_C0 || sw == SW_C3; }
	__inline bool operator<=(const int32_t x)               { uint16_t sw = _fcom_i4(*this, x) & (SW_C0 | SW_C2 | SW_C3); return sw == SW_C0 || sw == SW_C3; }
	__inline bool operator<=(const int64_t x)               { uint16_t sw = _fcom_i8(*this, x) & (SW_C0 | SW_C2 | SW_C3); return sw == SW_C0 || sw == SW_C3; }
	__inline bool operator>(const _longdouble& x)           { return (_fcom(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == 0; }
	__inline bool operator>(const float x)                  { return (_fcom_r4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == 0; }
	__inline bool operator>(const double x)                 { return (_fcom_r8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == 0; }
	__inline bool operator>(const int16_t x)                { return (_fcom_i2(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == 0; }
	__inline bool operator>(const int32_t x)                { return (_fcom_i4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == 0; }
	__inline bool operator>(const int64_t x)                { return (_fcom_i8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == 0; }
	__inline bool operator>=(const _longdouble& x)          { uint16_t sw = _fcom(*this, x) & (SW_C0 | SW_C2 | SW_C3); return !sw || sw == SW_C3; }
	__inline bool operator>=(const float x)                 { uint16_t sw = _fcom_r4(*this, x) & (SW_C0 | SW_C2 | SW_C3); return !sw || sw == SW_C3; }
	__inline bool operator>=(const double x)                { uint16_t sw = _fcom_r8(*this, x) & (SW_C0 | SW_C2 | SW_C3); return !sw || sw == SW_C3; }
	__inline bool operator>=(const int16_t x)               { uint16_t sw = _fcom_i2(*this, x) & (SW_C0 | SW_C2 | SW_C3); return !sw || sw == SW_C3; }
	__inline bool operator>=(const int32_t x)               { uint16_t sw = _fcom_i4(*this, x) & (SW_C0 | SW_C2 | SW_C3); return !sw || sw == SW_C3; }
	__inline bool operator>=(const int64_t x)               { uint16_t sw = _fcom_i8(*this, x) & (SW_C0 | SW_C2 | SW_C3); return !sw || sw == SW_C3; }
	__inline bool operator==(const _longdouble& x)          { return (_fcom(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C3; }
	__inline bool operator==(const float x)                 { return (_fcom_r4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C3; }
	__inline bool operator==(const double x)                { return (_fcom_r8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C3; }
	__inline bool operator==(const int16_t x)               { return (_fcom_i2(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C3; }
	__inline bool operator==(const int32_t x)               { return (_fcom_i4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C3; }
	__inline bool operator==(const int64_t x)               { return (_fcom_i8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) == SW_C3; }
	__inline bool operator!=(const _longdouble& x)          { return (_fcom(*this, x) & (SW_C0 | SW_C2 | SW_C3)) != SW_C3; }
	__inline bool operator!=(const float x)                 { return (_fcom_r4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) != SW_C3; }
	__inline bool operator!=(const double x)                { return (_fcom_r8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) != SW_C3; }
	__inline bool operator!=(const int16_t x)               { return (_fcom_i2(*this, x) & (SW_C0 | SW_C2 | SW_C3)) != SW_C3; }
	__inline bool operator!=(const int32_t x)               { return (_fcom_i4(*this, x) & (SW_C0 | SW_C2 | SW_C3)) != SW_C3; }
	__inline bool operator!=(const int64_t x)               { return (_fcom_i8(*this, x) & (SW_C0 | SW_C2 | SW_C3)) != SW_C3; }
	__inline _longdouble& operator=(const _longdouble& x)   { mantissa = x.mantissa; extension = x.extension; return *this; }
	__inline _longdouble& operator=(const float x)          { return *this = _fld_r4(x); }
	__inline _longdouble& operator=(const double x)         { return *this = _fld_r8(x); }
	__inline _longdouble& operator=(const bool x)           { return *this = _fld_i2(x); }
	__inline _longdouble& operator=(const int8_t x)         { return *this = _fld_i2(x); }
	__inline _longdouble& operator=(const int16_t x)        { return *this = _fld_i2(x); }
	__inline _longdouble& operator=(const int32_t x)        { return *this = _fld_i4(x); }
	__inline _longdouble& operator=(const int64_t x)        { return *this = _fld_i8(x); }
	__inline _longdouble& operator*=(const _longdouble& x)  { return *this = _fmul(*this, x); }
	__inline _longdouble& operator/=(const _longdouble& x)  { return *this = _fdiv(*this, x); }
	__inline _longdouble& operator%=(const _longdouble& x)  { return *this = _fmod(*this, x); }
	__inline _longdouble& operator+=(const _longdouble& x)  { return *this = _fadd(*this, x); }
	__inline _longdouble& operator-=(const _longdouble& x)  { return *this = _fsub(*this, x); }
	static __inline unsigned short fstcw()                  { return _fstcw(); }
	static __inline void fldcw(const uint16_t cw)           { _fldcw(cw); }
	static __inline _longdouble fldz()                      { return _fldz(); }
	static __inline _longdouble fld1()                      { return _fld1(); }
	static __inline _longdouble fldln2()                    { return _fldln2(); }
	__inline _longdouble fabs()                             { return _fabs(*this); }
	__inline _longdouble frndint()                          { return _frndint(*this); }
	__inline _longdouble f2xm1()                            { return _f2xm1(*this); }
	__inline _longdouble fxtract(const _longdouble *expptr) { return _fxtract(*this, expptr); }
	__inline _longdouble fscale(const _longdouble exp)      { return _fscale(*this, exp); }
	__inline _longdouble fyl2x(const _longdouble x)         { return _fyl2x(*this, x); }
#endif
} longdouble;

#ifdef _M_IX86
__forceinline uint16_t _fstcw()
{
	__asm
	{
		push    ax
		fstcw   word ptr [esp]
		pop     ax
	}
}
__forceinline void _fldcw(const uint16_t cw)
{
	__asm
	{
		fldcw   word ptr [cw]
	}
}
__forceinline longdouble _fldz()
{
	longdouble r;
	__asm
	{
		fldz
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fld1()
{
	longdouble r;
	__asm
	{
		fld1
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fldln2()
{
	longdouble r;
	__asm
	{
		fldln2
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fld_r8(const double x)
{
	longdouble r;
	__asm
	{
		fld     qword ptr [x]
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fld_i2(const int16_t x)
{
	longdouble r;
	__asm
	{
		fild    word ptr [x]
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fld_i4(const int32_t x)
{
	longdouble r;
	__asm
	{
		fild    dword ptr [x]
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fld_i8(const int64_t x)
{
	longdouble r;
	__asm
	{
		fild    qword ptr [x]
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline double _fst_r8(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
	}
}
__forceinline int16_t _fst_i2(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		push    ax
		fistp   word ptr [esp]
		pop     ax
	}
}
__forceinline int32_t _fst_i4(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		push    eax
		fistp   dword ptr [esp]
		pop     eax
	}
}
__forceinline int64_t _fst_i8(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		sub     esp, 8
		fistp   qword ptr [esp]
		pop     eax
		pop     edx
	}
}
__forceinline longdouble _fadd(const longdouble x, const longdouble y)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fld     tbyte ptr [y]
		fadd
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fsub(const longdouble x, const longdouble y)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fld     tbyte ptr [y]
		fsub
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fmul(const longdouble x, const longdouble y)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fld     tbyte ptr [y]
		fmul
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fdiv(const longdouble x, const longdouble y)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fld     tbyte ptr [y]
		fdiv
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fmod(const longdouble x, const longdouble y)
{
	longdouble r;
	__asm
	{
		fld     qword ptr [y]
		fld     qword ptr [x]
	L1:
		fprem
		fstsw   ax
		test    ax, 0400H
		jnz     L1
		fstp    tbyte ptr [r]
		fstp    st(0)
	}
	return r;
}
__forceinline longdouble _finc(const longdouble x)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fld1
		fadd
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fdec(const longdouble x)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fld1
		fsub
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fchs(const longdouble x)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fchs
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fabs(const longdouble x)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fabs
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _frndint(const longdouble x)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		frndint
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline uint16_t _ftst(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		ftst
		fstsw   ax
		fstp    st(0)
	}
}
__forceinline uint16_t _fcom(const longdouble x, const longdouble y)
{
	__asm
	{
		fld     tbyte ptr [y]
		fld     tbyte ptr [x]
		fcompp
		fstsw   ax
	}
}
__forceinline uint16_t _fcom_r8(const longdouble x, const double y)
{
	__asm
	{
		fld     qword ptr [y]
		fld     tbyte ptr [x]
		fcompp
		fstsw   ax
	}
}
__forceinline uint16_t _fcom_i2(const longdouble x, const int16_t y)
{
	__asm
	{
		fild    word ptr [y]
		fld     tbyte ptr [x]
		fcompp
		fstsw   ax
	}
}
__forceinline uint16_t _fcom_i4(const longdouble x, const int32_t y)
{
	__asm
	{
		fild    dword ptr [y]
		fld     tbyte ptr [x]
		fcompp
		fstsw   ax
	}
}
__forceinline uint16_t _fcom_i8(const longdouble x, const int64_t y)
{
	__asm
	{
		fild    qword ptr [y]
		fld     tbyte ptr [x]
		fcompp
		fstsw   ax
	}
}
__forceinline longdouble _f2xm1(const longdouble x)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		f2xm1
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fxtract(const longdouble x, const longdouble *expptr)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [x]
		fxtract
		fstp    tbyte ptr [r]
		mov     eax, dword ptr [expptr]
		fstp    tbyte ptr [eax]
	}
	return r;
}
__forceinline longdouble _fscale(const longdouble x, const longdouble exp)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [exp]
		fld     tbyte ptr [x]
		fscale
		fstp    st(1)
		fstp    tbyte ptr [r]
	}
	return r;
}
__forceinline longdouble _fyl2x(const longdouble x, const longdouble y)
{
	longdouble r;
	__asm
	{
		fld     tbyte ptr [y]
		fld     tbyte ptr [x]
		fyl2x
		fstp    st(1)
		fstp    tbyte ptr [r]
	}
	return r;
}
#endif

#endif
