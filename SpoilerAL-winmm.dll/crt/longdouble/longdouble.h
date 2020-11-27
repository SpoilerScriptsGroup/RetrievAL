#ifndef _LONGDOUBLE_H_
#define _LONGDOUBLE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdbool.h>
#include <stdint.h>

#define X87_MCW_EM                         0x003F
#define X87_EM_INVALID                     0x0001
#define X87_EM_DENORMAL                    0x0002
#define X87_EM_ZERODIVIDE                  0x0004
#define X87_EM_OVERFLOW                    0x0008
#define X87_EM_UNDERFLOW                   0x0010
#define X87_EM_INEXACT                     0x0020
#define X87_MCW_PC                         0x0300
#define X87_PC_24                          0x0100
#define X87_PC_53                          0x0200
#define X87_PC_64                          0x0300
#define X87_MCW_RC                         0x0C00
#define X87_RC_NEAR                        0x0000
#define X87_RC_DOWN                        0x0400
#define X87_RC_UP                          0x0800
#define X87_RC_CHOP                        0x0C00
#define X87_MCW_IC                         0x1000
#define X87_IC_PROJECTIVE                  0x0000
#define X87_IC_AFFINE                      0x1000
#define X87_MCW_DN                         0x8040
#define X87_DN_SAVE                        0x0000
#define X87_DN_FLUSH_OPERANDS_SAVE_RESULTS 0x0040
#define X87_DN_SAVE_OPERANDS_FLUSH_RESULTS 0x8000
#define X87_DN_FLUSH                       0x8040
#define X87_CW_DEFAULT                     0x027F

#define X87_SW_INVALID                     0x0001
#define X87_SW_DENORMAL                    0x0002
#define X87_SW_ZERODIVIDE                  0x0004
#define X87_SW_OVERFLOW                    0x0008
#define X87_SW_UNDERFLOW                   0x0010
#define X87_SW_INEXACT                     0x0020
#define X87_SW_STACKFAULT                  0x0040
#define X87_SW_ERRORSUMMARY                0x0080
#define X87_SW_C0                          0x0100
#define X87_SW_C1                          0x0200
#define X87_SW_C2                          0x0400
#define X87_SW_TOP                         0x0800
#define X87_SW_C3                          0x4000
#define X87_SW_BUSY                        0x8000
#define X87_SW_DEFAULT                     0x0000

#ifdef __cplusplus
struct longdouble;
longdouble _fld_r4(const float x);
longdouble _fld_r8(const double x);
longdouble _fld_i1(const int8_t x);
longdouble _fld_i2(const int16_t x);
longdouble _fld_i4(const int32_t x);
longdouble _fld_i8(const int64_t x);
longdouble _fld_u1(const uint8_t x);
longdouble _fld_u2(const uint16_t x);
longdouble _fld_u4(const uint32_t x);
longdouble _fld_u8(const uint64_t x);
float _fst_r4(const longdouble x);
double _fst_r8(const longdouble x);
int8_t _fst_i1(const longdouble x);
int16_t _fst_i2(const longdouble x);
int32_t _fst_i4(const longdouble x);
int64_t _fst_i8(const longdouble x);
uint8_t _fst_u1(const longdouble x);
uint16_t _fst_u2(const longdouble x);
uint32_t _fst_u4(const longdouble x);
uint64_t _fst_u8(const longdouble x);
longdouble _fadd(const longdouble x, const longdouble y);
longdouble _fsub(const longdouble x, const longdouble y);
longdouble _fmul(const longdouble x, const longdouble y);
longdouble _fdiv(const longdouble x, const longdouble y);
longdouble _fmod(const longdouble x, const longdouble y);
longdouble _finc(const longdouble x);
longdouble _fdec(const longdouble x);
longdouble _fabs(const longdouble x);
longdouble _fchs(const longdouble x);
longdouble _fsqrt(const longdouble x);
longdouble _frndint(const longdouble x);
longdouble _fxtract(const longdouble x, const longdouble *expptr);
uint16_t _fcom(const longdouble x, const longdouble y);
uint16_t _fcom_r4(const longdouble x, const float y);
uint16_t _fcom_r8(const longdouble x, const double y);
uint16_t _fcom_i1(const longdouble x, const int8_t y);
uint16_t _fcom_i2(const longdouble x, const int16_t y);
uint16_t _fcom_i4(const longdouble x, const int32_t y);
uint16_t _fcom_i8(const longdouble x, const int64_t y);
uint16_t _fcom_u1(const longdouble x, const uint8_t y);
uint16_t _fcom_u2(const longdouble x, const uint16_t y);
uint16_t _fcom_u4(const longdouble x, const uint32_t y);
uint16_t _fcom_u8(const longdouble x, const uint64_t y);
uint16_t _ftst(const longdouble x);
uint16_t _fxam(const longdouble x);
longdouble _fsin(const longdouble x);
longdouble _fcos(const longdouble x);
longdouble _fptan(const longdouble x);
longdouble _fpatan(const longdouble x, const longdouble y);
longdouble _fld1();
longdouble _fldl2t();
longdouble _fldl2e();
longdouble _fldpi();
longdouble _fldlg2();
longdouble _fldln2();
longdouble _fldz();
longdouble _fyl2x(const longdouble x, const longdouble y);
longdouble _fyl2xp1(const longdouble x, const longdouble y);
longdouble _f2xm1(const longdouble x);
longdouble _fscale(const longdouble x, const longdouble exp);
longdouble __ldexp(const longdouble x, const int exp);
void _fclex();
void _fldcw(const uint16_t cw);
uint16_t _fstcw();
#endif

#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
struct longdouble {
#else
typedef struct _longdouble {
#endif
	uint64_t mantissa;
	union {
		uint16_t extension;
		struct {
			uint16_t exponent : 15;
			uint16_t sign : 1;
		};
	};
	uint8_t padding[sizeof(uintptr_t) - 10 % sizeof(uintptr_t)];

#ifdef __cplusplus
	static __inline uint16_t initialize()                                          { uint16_t cw = _fstcw(); _fldcw(cw | X87_PC_64); return cw; }
	static __inline void finalize(const uint16_t cw)                               { _fldcw(cw); }
	__inline longdouble()                                                          { mantissa = 0; extension = 0; }
	__inline longdouble(uint64_t mantissa, uint16_t extension)                     { this->mantissa = mantissa; this->extension = extension; }
	__inline longdouble(uint64_t mantissa, uint16_t exponent, uint16_t sign)       { this->mantissa = mantissa; this->exponent = exponent; this->sign = sign; }
	__inline longdouble(const longdouble& x)                                       { mantissa = x.mantissa; extension = x.extension; }
	__inline longdouble(const float x)                                             { *this = _fld_r4(x); }
	__inline longdouble(const double x)                                            { *this = _fld_r8(x); }
	__inline longdouble(const bool x)                                              { *this = _fld_u1(x); }
	__inline longdouble(const int8_t x)                                            { *this = _fld_i1(x); }
	__inline longdouble(const int16_t x)                                           { *this = _fld_i2(x); }
	__inline longdouble(const int32_t x)                                           { *this = _fld_i4(x); }
	__inline longdouble(const int64_t x)                                           { *this = _fld_i8(x); }
	__inline longdouble(const uint8_t x)                                           { *this = _fld_u1(x); }
	__inline longdouble(const uint16_t x)                                          { *this = _fld_u2(x); }
	__inline longdouble(const uint32_t x)                                          { *this = _fld_u4(x); }
	__inline longdouble(const uint64_t x)                                          { *this = _fld_u8(x); }
	__inline longdouble operator++(int)                                            { longdouble x(*this); *this = _finc(*this); return x; }
	__inline longdouble operator--(int)                                            { longdouble x(*this); *this = _fdec(*this); return x; }
	__inline longdouble operator++()                                               { return *this = _finc(*this); }
	__inline longdouble operator--()                                               { return *this = _fdec(*this); }
	__inline longdouble operator+()                                                { return *this; }
	__inline longdouble operator-()                                                { return _fchs(*this); }
	__inline bool operator!()                                                      { return _ftst(*this) == X87_SW_C3; }
	__inline explicit operator bool()                                              { return _ftst(*this) != X87_SW_C3; }
	__inline explicit operator float()                                             { return _fst_r4(*this); }
	__inline explicit operator double()                                            { return _fst_r8(*this); }
	__inline explicit operator int8_t()                                            { return _fst_i1(*this); }
	__inline explicit operator int16_t()                                           { return _fst_i2(*this); }
	__inline explicit operator int32_t()                                           { return _fst_i4(*this); }
	__inline explicit operator int64_t()                                           { return _fst_i8(*this); }
	__inline explicit operator uint8_t()                                           { return _fst_u1(*this); }
	__inline explicit operator uint16_t()                                          { return _fst_u2(*this); }
	__inline explicit operator uint32_t()                                          { return _fst_u4(*this); }
	__inline explicit operator uint64_t()                                          { return _fst_u8(*this); }
	__inline longdouble operator*(const longdouble& x)                             { return _fmul(*this, x); }
	__inline longdouble operator*(const float x)                                   { return _fmul(*this, x); }
	__inline longdouble operator*(const double x)                                  { return _fmul(*this, x); }
	__inline longdouble operator*(const int8_t x)                                  { return _fmul(*this, x); }
	__inline longdouble operator*(const int16_t x)                                 { return _fmul(*this, x); }
	__inline longdouble operator*(const int32_t x)                                 { return _fmul(*this, x); }
	__inline longdouble operator*(const int64_t x)                                 { return _fmul(*this, x); }
	__inline longdouble operator*(const uint8_t x)                                 { return _fmul(*this, x); }
	__inline longdouble operator*(const uint16_t x)                                { return _fmul(*this, x); }
	__inline longdouble operator*(const uint32_t x)                                { return _fmul(*this, x); }
	__inline longdouble operator*(const uint64_t x)                                { return _fmul(*this, x); }
	__inline friend longdouble operator*(const longdouble &x, const longdouble& y) { return _fmul(x, y); }
	__inline friend longdouble operator*(const float x, const longdouble& y)       { return _fmul(x, y); }
	__inline friend longdouble operator*(const double x, const longdouble& y)      { return _fmul(x, y); }
	__inline friend longdouble operator*(const int8_t x, const longdouble& y)      { return _fmul(x, y); }
	__inline friend longdouble operator*(const int16_t x, const longdouble& y)     { return _fmul(x, y); }
	__inline friend longdouble operator*(const int32_t x, const longdouble& y)     { return _fmul(x, y); }
	__inline friend longdouble operator*(const int64_t x, const longdouble& y)     { return _fmul(x, y); }
	__inline friend longdouble operator*(const uint8_t x, const longdouble& y)     { return _fmul(x, y); }
	__inline friend longdouble operator*(const uint16_t x, const longdouble& y)    { return _fmul(x, y); }
	__inline friend longdouble operator*(const uint32_t x, const longdouble& y)    { return _fmul(x, y); }
	__inline friend longdouble operator*(const uint64_t x, const longdouble& y)    { return _fmul(x, y); }
	__inline longdouble operator/(const longdouble& x)                             { return _fdiv(*this, x); }
	__inline longdouble operator/(const float x)                                   { return _fdiv(*this, x); }
	__inline longdouble operator/(const double x)                                  { return _fdiv(*this, x); }
	__inline longdouble operator/(const int8_t x)                                  { return _fdiv(*this, x); }
	__inline longdouble operator/(const int16_t x)                                 { return _fdiv(*this, x); }
	__inline longdouble operator/(const int32_t x)                                 { return _fdiv(*this, x); }
	__inline longdouble operator/(const int64_t x)                                 { return _fdiv(*this, x); }
	__inline longdouble operator/(const uint8_t x)                                 { return _fdiv(*this, x); }
	__inline longdouble operator/(const uint16_t x)                                { return _fdiv(*this, x); }
	__inline longdouble operator/(const uint32_t x)                                { return _fdiv(*this, x); }
	__inline longdouble operator/(const uint64_t x)                                { return _fdiv(*this, x); }
	__inline friend longdouble operator/(const longdouble& x, const longdouble& y) { return _fdiv(x, y); }
	__inline friend longdouble operator/(const float x, const longdouble& y)       { return _fdiv(x, y); }
	__inline friend longdouble operator/(const double x, const longdouble& y)      { return _fdiv(x, y); }
	__inline friend longdouble operator/(const int8_t x, const longdouble& y)      { return _fdiv(x, y); }
	__inline friend longdouble operator/(const int16_t x, const longdouble& y)     { return _fdiv(x, y); }
	__inline friend longdouble operator/(const int32_t x, const longdouble& y)     { return _fdiv(x, y); }
	__inline friend longdouble operator/(const int64_t x, const longdouble& y)     { return _fdiv(x, y); }
	__inline friend longdouble operator/(const uint8_t x, const longdouble& y)     { return _fdiv(x, y); }
	__inline friend longdouble operator/(const uint16_t x, const longdouble& y)    { return _fdiv(x, y); }
	__inline friend longdouble operator/(const uint32_t x, const longdouble& y)    { return _fdiv(x, y); }
	__inline friend longdouble operator/(const uint64_t x, const longdouble& y)    { return _fdiv(x, y); }
	__inline longdouble operator%(const longdouble& x)                             { return _fmod(*this, x); }
	__inline longdouble operator%(const float x)                                   { return _fmod(*this, x); }
	__inline longdouble operator%(const double x)                                  { return _fmod(*this, x); }
	__inline longdouble operator%(const int8_t x)                                  { return _fmod(*this, x); }
	__inline longdouble operator%(const int16_t x)                                 { return _fmod(*this, x); }
	__inline longdouble operator%(const int32_t x)                                 { return _fmod(*this, x); }
	__inline longdouble operator%(const int64_t x)                                 { return _fmod(*this, x); }
	__inline longdouble operator%(const uint8_t x)                                 { return _fmod(*this, x); }
	__inline longdouble operator%(const uint16_t x)                                { return _fmod(*this, x); }
	__inline longdouble operator%(const uint32_t x)                                { return _fmod(*this, x); }
	__inline longdouble operator%(const uint64_t x)                                { return _fmod(*this, x); }
	__inline friend longdouble operator%(const longdouble& x, const longdouble& y) { return _fmod(x, y); }
	__inline friend longdouble operator%(const float x, const longdouble& y)       { return _fmod(x, y); }
	__inline friend longdouble operator%(const double x, const longdouble& y)      { return _fmod(x, y); }
	__inline friend longdouble operator%(const int8_t x, const longdouble& y)      { return _fmod(x, y); }
	__inline friend longdouble operator%(const int16_t x, const longdouble& y)     { return _fmod(x, y); }
	__inline friend longdouble operator%(const int32_t x, const longdouble& y)     { return _fmod(x, y); }
	__inline friend longdouble operator%(const int64_t x, const longdouble& y)     { return _fmod(x, y); }
	__inline friend longdouble operator%(const uint8_t x, const longdouble& y)     { return _fmod(x, y); }
	__inline friend longdouble operator%(const uint16_t x, const longdouble& y)    { return _fmod(x, y); }
	__inline friend longdouble operator%(const uint32_t x, const longdouble& y)    { return _fmod(x, y); }
	__inline friend longdouble operator%(const uint64_t x, const longdouble& y)    { return _fmod(x, y); }
	__inline longdouble operator+(const longdouble& x)                             { return _fadd(*this, x); }
	__inline longdouble operator+(const float x)                                   { return _fadd(*this, x); }
	__inline longdouble operator+(const double x)                                  { return _fadd(*this, x); }
	__inline longdouble operator+(const int8_t x)                                  { return _fadd(*this, x); }
	__inline longdouble operator+(const int16_t x)                                 { return _fadd(*this, x); }
	__inline longdouble operator+(const int32_t x)                                 { return _fadd(*this, x); }
	__inline longdouble operator+(const int64_t x)                                 { return _fadd(*this, x); }
	__inline longdouble operator+(const uint8_t x)                                 { return _fadd(*this, x); }
	__inline longdouble operator+(const uint16_t x)                                { return _fadd(*this, x); }
	__inline longdouble operator+(const uint32_t x)                                { return _fadd(*this, x); }
	__inline longdouble operator+(const uint64_t x)                                { return _fadd(*this, x); }
	__inline friend longdouble operator+(const longdouble& x, const longdouble& y) { return _fadd(x, y); }
	__inline friend longdouble operator+(const float x, const longdouble& y)       { return _fadd(x, y); }
	__inline friend longdouble operator+(const double x, const longdouble& y)      { return _fadd(x, y); }
	__inline friend longdouble operator+(const int8_t x, const longdouble& y)      { return _fadd(x, y); }
	__inline friend longdouble operator+(const int16_t x, const longdouble& y)     { return _fadd(x, y); }
	__inline friend longdouble operator+(const int32_t x, const longdouble& y)     { return _fadd(x, y); }
	__inline friend longdouble operator+(const int64_t x, const longdouble& y)     { return _fadd(x, y); }
	__inline friend longdouble operator+(const uint8_t x, const longdouble& y)     { return _fadd(x, y); }
	__inline friend longdouble operator+(const uint16_t x, const longdouble& y)    { return _fadd(x, y); }
	__inline friend longdouble operator+(const uint32_t x, const longdouble& y)    { return _fadd(x, y); }
	__inline friend longdouble operator+(const uint64_t x, const longdouble& y)    { return _fadd(x, y); }
	__inline longdouble operator-(const longdouble& x)                             { return _fsub(*this, x); }
	__inline longdouble operator-(const float x)                                   { return _fsub(*this, x); }
	__inline longdouble operator-(const double x)                                  { return _fsub(*this, x); }
	__inline longdouble operator-(const int8_t x)                                  { return _fsub(*this, x); }
	__inline longdouble operator-(const int16_t x)                                 { return _fsub(*this, x); }
	__inline longdouble operator-(const int32_t x)                                 { return _fsub(*this, x); }
	__inline longdouble operator-(const int64_t x)                                 { return _fsub(*this, x); }
	__inline longdouble operator-(const uint8_t x)                                 { return _fsub(*this, x); }
	__inline longdouble operator-(const uint16_t x)                                { return _fsub(*this, x); }
	__inline longdouble operator-(const uint32_t x)                                { return _fsub(*this, x); }
	__inline longdouble operator-(const uint64_t x)                                { return _fsub(*this, x); }
	__inline friend longdouble operator-(const longdouble& x, const longdouble& y) { return _fsub(x, y); }
	__inline friend longdouble operator-(const float x, const longdouble& y)       { return _fsub(x, y); }
	__inline friend longdouble operator-(const double x, const longdouble& y)      { return _fsub(x, y); }
	__inline friend longdouble operator-(const int8_t x, const longdouble& y)      { return _fsub(x, y); }
	__inline friend longdouble operator-(const int16_t x, const longdouble& y)     { return _fsub(x, y); }
	__inline friend longdouble operator-(const int32_t x, const longdouble& y)     { return _fsub(x, y); }
	__inline friend longdouble operator-(const int64_t x, const longdouble& y)     { return _fsub(x, y); }
	__inline friend longdouble operator-(const uint8_t x, const longdouble& y)     { return _fsub(x, y); }
	__inline friend longdouble operator-(const uint16_t x, const longdouble& y)    { return _fsub(x, y); }
	__inline friend longdouble operator-(const uint32_t x, const longdouble& y)    { return _fsub(x, y); }
	__inline friend longdouble operator-(const uint64_t x, const longdouble& y)    { return _fsub(x, y); }
	__inline longdouble operator<<(const int x)                                    { return __ldexp(*this, x); }
	__inline longdouble operator>>(const int x)                                    { return __ldexp(*this, -x); }
	__inline bool operator<(const longdouble& x)                                   { return _fcom(*this, x) == X87_SW_C0; }
	__inline bool operator<(const float x)                                         { return _fcom_r4(*this, x) == X87_SW_C0; }
	__inline bool operator<(const double x)                                        { return _fcom_r8(*this, x) == X87_SW_C0; }
	__inline bool operator<(const int8_t x)                                        { return _fcom_i1(*this, x) == X87_SW_C0; }
	__inline bool operator<(const int16_t x)                                       { return _fcom_i2(*this, x) == X87_SW_C0; }
	__inline bool operator<(const int32_t x)                                       { return _fcom_i4(*this, x) == X87_SW_C0; }
	__inline bool operator<(const int64_t x)                                       { return _fcom_i8(*this, x) == X87_SW_C0; }
	__inline bool operator<(const uint8_t x)                                       { return _fcom_u1(*this, x) == X87_SW_C0; }
	__inline bool operator<(const uint16_t x)                                      { return _fcom_u2(*this, x) == X87_SW_C0; }
	__inline bool operator<(const uint32_t x)                                      { return _fcom_u4(*this, x) == X87_SW_C0; }
	__inline bool operator<(const uint64_t x)                                      { return _fcom_u8(*this, x) == X87_SW_C0; }
	__inline friend bool operator<(const float x, const longdouble& y)             { return !_fcom_r4(y, x); }
	__inline friend bool operator<(const double x, const longdouble& y)            { return !_fcom_r8(y, x); }
	__inline friend bool operator<(const int8_t x, const longdouble& y)            { return !_fcom_i1(y, x); }
	__inline friend bool operator<(const int16_t x, const longdouble& y)           { return !_fcom_i2(y, x); }
	__inline friend bool operator<(const int32_t x, const longdouble& y)           { return !_fcom_i4(y, x); }
	__inline friend bool operator<(const int64_t x, const longdouble& y)           { return !_fcom_i8(y, x); }
	__inline friend bool operator<(const uint8_t x, const longdouble& y)           { return !_fcom_u1(y, x); }
	__inline friend bool operator<(const uint16_t x, const longdouble& y)          { return !_fcom_u2(y, x); }
	__inline friend bool operator<(const uint32_t x, const longdouble& y)          { return !_fcom_u4(y, x); }
	__inline friend bool operator<(const uint64_t x, const longdouble& y)          { return !_fcom_u8(y, x); }
	__inline bool operator<=(const longdouble& x)                                  { uint16_t sw = _fcom(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const float x)                                        { uint16_t sw = _fcom_r4(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const double x)                                       { uint16_t sw = _fcom_r8(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const int8_t x)                                       { uint16_t sw = _fcom_i1(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const int16_t x)                                      { uint16_t sw = _fcom_i2(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const int32_t x)                                      { uint16_t sw = _fcom_i4(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const int64_t x)                                      { uint16_t sw = _fcom_i8(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const uint8_t x)                                      { uint16_t sw = _fcom_u1(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const uint16_t x)                                     { uint16_t sw = _fcom_u2(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const uint32_t x)                                     { uint16_t sw = _fcom_u4(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator<=(const uint64_t x)                                     { uint16_t sw = _fcom_u8(*this, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator<=(const float x, const longdouble& y)            { uint16_t sw = _fcom_r4(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const double x, const longdouble& y)           { uint16_t sw = _fcom_r8(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const int8_t x, const longdouble& y)           { uint16_t sw = _fcom_i1(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const int16_t x, const longdouble& y)          { uint16_t sw = _fcom_i2(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const int32_t x, const longdouble& y)          { uint16_t sw = _fcom_i4(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const int64_t x, const longdouble& y)          { uint16_t sw = _fcom_i8(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const uint8_t x, const longdouble& y)          { uint16_t sw = _fcom_u1(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const uint16_t x, const longdouble& y)         { uint16_t sw = _fcom_u2(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const uint32_t x, const longdouble& y)         { uint16_t sw = _fcom_u4(y, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator<=(const uint64_t x, const longdouble& y)         { uint16_t sw = _fcom_u8(y, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>(const longdouble& x)                                   { return !_fcom(*this, x); }
	__inline bool operator>(const float x)                                         { return !_fcom_r4(*this, x); }
	__inline bool operator>(const double x)                                        { return !_fcom_r8(*this, x); }
	__inline bool operator>(const int8_t x)                                        { return !_fcom_i1(*this, x); }
	__inline bool operator>(const int16_t x)                                       { return !_fcom_i2(*this, x); }
	__inline bool operator>(const int32_t x)                                       { return !_fcom_i4(*this, x); }
	__inline bool operator>(const int64_t x)                                       { return !_fcom_i8(*this, x); }
	__inline bool operator>(const uint8_t x)                                       { return !_fcom_u1(*this, x); }
	__inline bool operator>(const uint16_t x)                                      { return !_fcom_u2(*this, x); }
	__inline bool operator>(const uint32_t x)                                      { return !_fcom_u4(*this, x); }
	__inline bool operator>(const uint64_t x)                                      { return !_fcom_u8(*this, x); }
	__inline friend bool operator>(const float x, const longdouble& y)             { return _fcom_r4(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const double x, const longdouble& y)            { return _fcom_r8(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const int8_t x, const longdouble& y)            { return _fcom_i1(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const int16_t x, const longdouble& y)           { return _fcom_i2(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const int32_t x, const longdouble& y)           { return _fcom_i4(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const int64_t x, const longdouble& y)           { return _fcom_i8(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const uint8_t x, const longdouble& y)           { return _fcom_u1(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const uint16_t x, const longdouble& y)          { return _fcom_u2(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const uint32_t x, const longdouble& y)          { return _fcom_u4(y, x) == X87_SW_C0; }
	__inline friend bool operator>(const uint64_t x, const longdouble& y)          { return _fcom_u8(y, x) == X87_SW_C0; }
	__inline bool operator>=(const longdouble& x)                                  { uint16_t sw = _fcom(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const float x)                                        { uint16_t sw = _fcom_r4(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const double x)                                       { uint16_t sw = _fcom_r8(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const int8_t x)                                       { uint16_t sw = _fcom_i1(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const int16_t x)                                      { uint16_t sw = _fcom_i2(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const int32_t x)                                      { uint16_t sw = _fcom_i4(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const int64_t x)                                      { uint16_t sw = _fcom_i8(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const uint8_t x)                                      { uint16_t sw = _fcom_u1(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const uint16_t x)                                     { uint16_t sw = _fcom_u2(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const uint32_t x)                                     { uint16_t sw = _fcom_u4(*this, x); return !sw || sw == X87_SW_C3; }
	__inline bool operator>=(const uint64_t x)                                     { uint16_t sw = _fcom_u8(*this, x); return !sw || sw == X87_SW_C3; }
	__inline friend bool operator>=(const float x, const longdouble& y)            { uint16_t sw = _fcom_r4(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const double x, const longdouble& y)           { uint16_t sw = _fcom_r8(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const int8_t x, const longdouble& y)           { uint16_t sw = _fcom_i1(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const int16_t x, const longdouble& y)          { uint16_t sw = _fcom_i2(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const int32_t x, const longdouble& y)          { uint16_t sw = _fcom_i4(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const int64_t x, const longdouble& y)          { uint16_t sw = _fcom_i8(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const uint8_t x, const longdouble& y)          { uint16_t sw = _fcom_u1(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const uint16_t x, const longdouble& y)         { uint16_t sw = _fcom_u2(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const uint32_t x, const longdouble& y)         { uint16_t sw = _fcom_u4(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline friend bool operator>=(const uint64_t x, const longdouble& y)         { uint16_t sw = _fcom_u8(y, x); return sw == X87_SW_C0 || sw == X87_SW_C3; }
	__inline bool operator==(const longdouble& x)                                  { return _fcom(*this, x) == X87_SW_C3; }
	__inline bool operator==(const float x)                                        { return _fcom_r4(*this, x) == X87_SW_C3; }
	__inline bool operator==(const double x)                                       { return _fcom_r8(*this, x) == X87_SW_C3; }
	__inline bool operator==(const int8_t x)                                       { return _fcom_i1(*this, x) == X87_SW_C3; }
	__inline bool operator==(const int16_t x)                                      { return _fcom_i2(*this, x) == X87_SW_C3; }
	__inline bool operator==(const int32_t x)                                      { return _fcom_i4(*this, x) == X87_SW_C3; }
	__inline bool operator==(const int64_t x)                                      { return _fcom_i8(*this, x) == X87_SW_C3; }
	__inline bool operator==(const uint8_t x)                                      { return _fcom_u1(*this, x) == X87_SW_C3; }
	__inline bool operator==(const uint16_t x)                                     { return _fcom_u2(*this, x) == X87_SW_C3; }
	__inline bool operator==(const uint32_t x)                                     { return _fcom_u4(*this, x) == X87_SW_C3; }
	__inline bool operator==(const uint64_t x)                                     { return _fcom_u8(*this, x) == X87_SW_C3; }
	__inline friend bool operator==(const float x, const longdouble& y)            { return _fcom_r4(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const double x, const longdouble& y)           { return _fcom_r8(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const int8_t x, const longdouble& y)           { return _fcom_i1(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const int16_t x, const longdouble& y)          { return _fcom_i2(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const int32_t x, const longdouble& y)          { return _fcom_i4(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const int64_t x, const longdouble& y)          { return _fcom_i8(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const uint8_t x, const longdouble& y)          { return _fcom_u1(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const uint16_t x, const longdouble& y)         { return _fcom_u2(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const uint32_t x, const longdouble& y)         { return _fcom_u4(y, x) == X87_SW_C3; }
	__inline friend bool operator==(const uint64_t x, const longdouble& y)         { return _fcom_u8(y, x) == X87_SW_C3; }
	__inline bool operator!=(const longdouble& x)                                  { return _fcom(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const float x)                                        { return _fcom_r4(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const double x)                                       { return _fcom_r8(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const int8_t x)                                       { return _fcom_i1(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const int16_t x)                                      { return _fcom_i2(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const int32_t x)                                      { return _fcom_i4(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const int64_t x)                                      { return _fcom_i8(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const uint8_t x)                                      { return _fcom_u1(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const uint16_t x)                                     { return _fcom_u2(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const uint32_t x)                                     { return _fcom_u4(*this, x) != X87_SW_C3; }
	__inline bool operator!=(const uint64_t x)                                     { return _fcom_u8(*this, x) != X87_SW_C3; }
	__inline friend bool operator!=(const float x, const longdouble& y)            { return _fcom_r4(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const double x, const longdouble& y)           { return _fcom_r8(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const int8_t x, const longdouble& y)           { return _fcom_i1(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const int16_t x, const longdouble& y)          { return _fcom_i2(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const int32_t x, const longdouble& y)          { return _fcom_i4(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const int64_t x, const longdouble& y)          { return _fcom_i8(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const uint8_t x, const longdouble& y)          { return _fcom_u1(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const uint16_t x, const longdouble& y)         { return _fcom_u2(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const uint32_t x, const longdouble& y)         { return _fcom_u4(y, x) != X87_SW_C3; }
	__inline friend bool operator!=(const uint64_t x, const longdouble& y)         { return _fcom_u8(y, x) != X87_SW_C3; }
	__inline const longdouble& operator=(const longdouble& x)                      { mantissa = x.mantissa; extension = x.extension; return *this; }
	__inline const longdouble& operator=(const float x)                            { return *this = _fld_r4(x); }
	__inline const longdouble& operator=(const double x)                           { return *this = _fld_r8(x); }
	__inline const longdouble& operator=(const bool x)                             { return *this = _fld_u1(x); }
	__inline const longdouble& operator=(const int8_t x)                           { return *this = _fld_i1(x); }
	__inline const longdouble& operator=(const int16_t x)                          { return *this = _fld_i2(x); }
	__inline const longdouble& operator=(const int32_t x)                          { return *this = _fld_i4(x); }
	__inline const longdouble& operator=(const int64_t x)                          { return *this = _fld_i8(x); }
	__inline const longdouble& operator=(const uint8_t x)                          { return *this = _fld_u1(x); }
	__inline const longdouble& operator=(const uint16_t x)                         { return *this = _fld_u2(x); }
	__inline const longdouble& operator=(const uint32_t x)                         { return *this = _fld_u4(x); }
	__inline const longdouble& operator=(const uint64_t x)                         { return *this = _fld_u8(x); }
	__inline const longdouble& operator*=(const longdouble& x)                     { return *this = _fmul(*this, x); }
	__inline const longdouble& operator/=(const longdouble& x)                     { return *this = _fdiv(*this, x); }
	__inline const longdouble& operator%=(const longdouble& x)                     { return *this = _fmod(*this, x); }
	__inline const longdouble& operator+=(const longdouble& x)                     { return *this = _fadd(*this, x); }
	__inline const longdouble& operator-=(const longdouble& x)                     { return *this = _fsub(*this, x); }
	__inline const longdouble& operator<<=(const int x)                            { return *this = __ldexp(*this, x); }
	__inline const longdouble& operator>>=(const int x)                            { return *this = __ldexp(*this, -x); }
	__inline longdouble fabs()                                                     { return _fabs(*this); }
	__inline uint16_t fxam()                                                       { return _fxam(*this); }
	__inline longdouble fsin()                                                     { return _fsin(*this); }
	__inline longdouble fcos()                                                     { return _fcos(*this); }
	__inline longdouble fptan()                                                    { return _fptan(*this); }
	__inline longdouble fpatan(const longdouble y)                                 { return _fpatan(*this, y); }
	static __inline longdouble fld1()                                              { return _fld1(); }
	static __inline longdouble fldl2t()                                            { return _fldl2t(); }
	static __inline longdouble fldl2e()                                            { return _fldl2e(); }
	static __inline longdouble fldpi()                                             { return _fldpi(); }
	static __inline longdouble fldlg2()                                            { return _fldlg2(); }
	static __inline longdouble fldln2()                                            { return _fldln2(); }
	static __inline longdouble fldz()                                              { return _fldz(); }
	__inline longdouble fsqrt()                                                    { return _fsqrt(*this); }
	__inline longdouble frndint()                                                  { return _frndint(*this); }
	__inline longdouble fxtract(const longdouble *expptr)                          { return _fxtract(*this, expptr); }
	__inline longdouble fyl2x(const longdouble y)                                  { return _fyl2x(*this, y); }
	__inline longdouble fyl2xp1(const longdouble y)                                { return _fyl2xp1(*this, y); }
	__inline longdouble f2xm1()                                                    { return _f2xm1(*this); }
	__inline longdouble fscale(const longdouble exp)                               { return _fscale(*this, exp); }
	__inline longdouble ldexp(const int exp)                                       { return __ldexp(*this, exp); }
	static __inline void fclex()                                                   { _fclex(); }
	static __inline void fldcw(const uint16_t cw)                                  { _fldcw(cw); }
	static __inline uint16_t fstcw()                                               { return _fstcw(); }
	__inline int signbit()                                                         { return (int16_t)extension < 0; }
	static longdouble atold(const char *p)
	{
		uint16_t cw;
		longdouble x;
		int sign;
		const char *a;

		cw = longdouble::fstcw();
		longdouble::fldcw(cw | X87_PC_64);
		a = p;
		while (*p == ' ' || (unsigned char)(*p - '\t') < '\r' - '\t' + 1)
			p++;

		/* decimal part */
		sign = 1;
		if (*p == '-')
		{
			sign = -1;
			++p;
		}
		else if (*p == '+')
			++p;
		if ((unsigned char)(*p - '0') < '9' - '0' + 1)
		{
			x = (longdouble)(*(p++) - '0');
			while (*p && (unsigned char)(*p - '0') < '9' - '0' + 1)
			{
				x = x * 10 + (*p - '0');
				++p;
			}
			a = p;
		}
		else
		{
			x = 0;
			if (*p != '.')
				goto done;
		}
		x *= sign;

		/* fraction part */
		if (*p == '.')
		{
			longdouble f;
			longdouble base;

			f = 0;
			base = 0.1;
			++p;
			if ((unsigned char)(*p - '0') < '9' - '0' + 1)
			{
				while (*p && (unsigned char)(*p - '0') < '9' - '0' + 1)
				{
					f += base * (*p - '0');
					base /= 10;
					++p;
				}
			}
			x += f * sign;
			a = p;
		}

		/* exponential part */
		if (*p == 'E' || *p == 'e')
		{
			int e;

			e = 0;
			++p;
			sign = 1;
			if (*p == '-')
			{
				sign = -1;
				++p;
			}
			else if (*p == '+')
				++p;

			if ((unsigned char)(*p - '0') < '9' - '0' + 1)
			{
				while (*p == '0')
					++p;
				e = (int)(*(p++) - '0');
				while (*p && (unsigned char)(*p - '0') < '9' - '0' + 1)
				{
					e = e * 10 + (int)(*p - '0');
					++p;
				}
				e *= sign;
			}
			else if ((unsigned char)(*(a - 1) - '0') >= '9' - '0' + 1)
				goto done;

			if (e > 0)
				do
					x *= 10;
				while (--e);
			else if (e < 0)
				do
					x /= 10;
				while (++e);
		}
	done:
		longdouble::fldcw(cw);
		return x;
	}
#endif
#ifdef __cplusplus
};
#else
} longdouble;
#endif

#pragma pack(pop)

#ifdef _M_IX86
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
__forceinline longdouble _fld_u8(const uint64_t x)
{
	longdouble r;
	__asm
	{
		mov     eax, dword ptr [x    ]
		mov     edx, dword ptr [x + 4]
		test    edx, edx
		js      L1
		fild    qword ptr [x    ]
		fstp    tbyte ptr [r    ]
		jmp     L2

	L1:
		mov     dword ptr [r    ], eax
		mov     dword ptr [r + 4], edx
		mov     word  ptr [r + 8], 0x403E
	L2:
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
		fistp   word ptr [esp - 4]
		mov     ax, word ptr [esp - 4]
	}
}
__forceinline int32_t _fst_i4(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		fistp   dword ptr [esp - 4]
		mov     eax, dword ptr [esp - 4]
	}
}
__forceinline int64_t _fst_i8(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		fistp   qword ptr [esp - 8]
		mov     eax, dword ptr [esp - 8]
		mov     edx, dword ptr [esp - 4]
	}
}
__forceinline uint64_t _fst_u8(const longdouble x)
{
	__asm
	{
		mov     ax, word ptr [x + 8]
		mov     edx, -1
		cmp     ax, 0x403E
		jge     L1
		fld     tbyte ptr [x    ]
		fistp   qword ptr [esp - 8]
		mov     eax, dword ptr [esp - 8]
		mov     edx, dword ptr [esp - 4]
		jmp     L2

	L1:
		mov     eax, -1
		jne     L2
		mov     eax, dword ptr [x    ]
		mov     edx, dword ptr [x + 4]
	L2:
	}
}
#define __LONGDOUBLE_PROC1(instruction)                           \
longdouble _##instruction(const longdouble x, const longdouble y) \
{                                                                 \
    __asm                                                         \
    {                                                             \
    __asm   fld     tbyte ptr [x]                                 \
    __asm   fld     tbyte ptr [y]                                 \
    __asm   instruction                                           \
    __asm   fstp    tbyte ptr [x]                                 \
    }                                                             \
    return x;                                                     \
}
__forceinline __LONGDOUBLE_PROC1(fadd)
__forceinline __LONGDOUBLE_PROC1(fsub)
__forceinline __LONGDOUBLE_PROC1(fmul)
__forceinline __LONGDOUBLE_PROC1(fdiv)
__forceinline longdouble _fmod(const longdouble x, const longdouble y)
{
	__asm
	{
		fld     tbyte ptr [y]
		fld     tbyte ptr [x]
	L1:
		fprem
		fstsw   ax
		test    ax, X87_SW_C2
		jnz     L1
		fstp    tbyte ptr [x]
		fstp    st(0)
	}
	return x;
}
__forceinline longdouble _finc(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		fld1
		fadd
		fstp    tbyte ptr [x]
	}
	return x;
}
__forceinline longdouble _fdec(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		fld1
		fsub
		fstp    tbyte ptr [x]
	}
	return x;
}
#define __LONGDOUBLE_PROC2(instruction)       \
longdouble _##instruction(const longdouble x) \
{                                             \
    __asm                                     \
    {                                         \
    __asm   fld     tbyte ptr [x]             \
    __asm   instruction                       \
    __asm   fstp    tbyte ptr [x]             \
    }                                         \
    return x;                                 \
}
__forceinline __LONGDOUBLE_PROC2(fabs)
__forceinline __LONGDOUBLE_PROC2(fchs)
__forceinline __LONGDOUBLE_PROC2(fsqrt)
__forceinline __LONGDOUBLE_PROC2(frndint)
__forceinline longdouble _fxtract(const longdouble x, const longdouble *expptr)
{
	__asm
	{
		fld     tbyte ptr [x]
		fxtract
		fstp    tbyte ptr [x]
		mov     eax, dword ptr [expptr]
		fstp    tbyte ptr [eax]
	}
	return x;
}
__forceinline uint16_t _fcom(const longdouble x, const longdouble y)
{
	__asm
	{
		fld     tbyte ptr [y]
		fld     tbyte ptr [x]
		fcompp
		fstsw   ax
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
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
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
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
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
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
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
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
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
	}
}
__forceinline uint16_t _fcom_u8(const longdouble x, const uint64_t y)
{
	__asm
	{
		mov     eax, dword ptr [y    ]
		mov     edx, dword ptr [y + 4]
		test    edx, edx
		js      L1
		fild    qword ptr [y    ]
		jmp     L2

	L1:
		mov     dword ptr [esp - 12], eax
		mov     dword ptr [esp -  8], edx
		mov     dword ptr [esp -  4], 0x403E
		fld     tbyte ptr [esp - 12]
	L2:
		fld     tbyte ptr [x    ]
		fcompp
		fstsw   ax
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
	}
}
__forceinline uint16_t _ftst(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		ftst
		fstp    st(0)
		fstsw   ax
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
	}
}
__forceinline uint16_t _fxam(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		fxam
		fstp    st(0)
		fstsw   ax
		and     ax, X87_SW_C0 | X87_SW_C2 | X87_SW_C3
	}
}
__forceinline __LONGDOUBLE_PROC2(fsin)
__forceinline __LONGDOUBLE_PROC2(fcos)
__forceinline longdouble _fptan(const longdouble x)
{
	__asm
	{
		fld     tbyte ptr [x]
		fpatan
		fstp    st(0)
		fstp    tbyte ptr [x]
	}
	return x;
}
__forceinline __LONGDOUBLE_PROC1(fpatan)
#define __LONGDOUBLE_PROC3(instruction) \
longdouble _##instruction()             \
{                                       \
    longdouble x;                       \
    __asm                               \
    {                                   \
    __asm   instruction                 \
    __asm   fstp    tbyte ptr [x]       \
    }                                   \
    return x;                           \
}
__forceinline __LONGDOUBLE_PROC3(fld1)
__forceinline __LONGDOUBLE_PROC3(fldl2t)
__forceinline __LONGDOUBLE_PROC3(fldl2e)
__forceinline __LONGDOUBLE_PROC3(fldpi)
__forceinline __LONGDOUBLE_PROC3(fldlg2)
__forceinline __LONGDOUBLE_PROC3(fldln2)
__forceinline __LONGDOUBLE_PROC3(fldz)
#define __LONGDOUBLE_PROC4(instruction)                           \
longdouble _##instruction(const longdouble x, const longdouble y) \
{                                                                 \
    __asm                                                         \
    {                                                             \
    __asm   fld     tbyte ptr [y]                                 \
    __asm   fld     tbyte ptr [x]                                 \
    __asm   instruction                                           \
    __asm   fstp    tbyte ptr [x]                                 \
    }                                                             \
    return x;                                                     \
}
__forceinline __LONGDOUBLE_PROC4(fyl2x)
__forceinline __LONGDOUBLE_PROC4(fyl2xp1)
__forceinline __LONGDOUBLE_PROC2(f2xm1)
__forceinline longdouble _fscale(const longdouble x, const longdouble exp)
{
	__asm
	{
		fld     tbyte ptr [exp]
		fld     tbyte ptr [x]
		fscale
		fstp    tbyte ptr [x]
		fstp    st(0)
	}
	return x;
}
__forceinline longdouble __ldexp(const longdouble x, const int exp)
{
	__asm
	{
		fild    dword ptr [exp]
		fld     tbyte ptr [x]
		fscale
		fstp    tbyte ptr [x]
		fstp    st(0)
	}
	return x;
}
__forceinline void _fclex()
{
	__asm
	{
		fclex
	}
}
__forceinline void _fldcw(const uint16_t cw)
{
	__asm
	{
		fldcw   word ptr [cw]
	}
}
__forceinline uint16_t _fstcw()
{
	__asm
	{
		fstcw   word ptr [esp - 4]
		mov     ax, word ptr [esp - 4]
	}
}
#undef __LONGDOUBLE_PROC1
#undef __LONGDOUBLE_PROC2
#undef __LONGDOUBLE_PROC3
#undef __LONGDOUBLE_PROC4
#endif

__forceinline longdouble _fld_r4(const float x)
{
	return _fld_r8(x);
}
__forceinline longdouble _fld_i1(const int8_t x)
{
	return _fld_i2(x);
}
__forceinline longdouble _fld_u1(const uint8_t x)
{
	return _fld_i2(x);
}
__forceinline longdouble _fld_u2(const uint16_t x)
{
	return _fld_i4(x);
}
__forceinline longdouble _fld_u4(const uint32_t x)
{
	return _fld_i8(x);
}
__forceinline float _fst_r4(const longdouble x)
{
	return (float)_fst_r8(x);
}
__forceinline int8_t _fst_i1(const longdouble x)
{
	int16_t r = _fst_i2(x);
	if (r > INT8_MAX)
		return INT8_MAX;
	if (r < INT8_MIN)
		return INT8_MIN;
	return (int8_t)r;
}
__forceinline uint8_t _fst_u1(const longdouble x)
{
	int16_t r = _fst_i2(x);
	if (r > UINT8_MAX)
		return UINT8_MAX;
	if (r < INT8_MIN)
		return (uint8_t)INT8_MIN;
	return (uint8_t)r;
}
__forceinline uint16_t _fst_u2(const longdouble x)
{
	int32_t r = _fst_i4(x);
	if (r > UINT16_MAX)
		return UINT16_MAX;
	if (r < INT16_MIN)
		return (uint16_t)INT16_MIN;
	return (uint16_t)r;
}
__forceinline uint32_t _fst_u4(const longdouble x)
{
	int64_t r = _fst_i8(x);
	if (r > UINT32_MAX)
		return UINT32_MAX;
	if (r < INT32_MIN)
		return (uint32_t)INT32_MIN;
	return (uint32_t)r;
}
__forceinline uint16_t _fcom_r4(const longdouble x, const float y)
{
	return _fcom_r8(x, y);
}
__forceinline uint16_t _fcom_i1(const longdouble x, const int8_t y)
{
	return _fcom_i2(x, y);
}
__forceinline uint16_t _fcom_u1(const longdouble x, const uint8_t y)
{
	return _fcom_i2(x, y);
}
__forceinline uint16_t _fcom_u2(const longdouble x, const uint16_t y)
{
	return _fcom_i4(x, y);
}
__forceinline uint16_t _fcom_u4(const longdouble x, const uint32_t y)
{
	return _fcom_i8(x, y);
}

#endif
