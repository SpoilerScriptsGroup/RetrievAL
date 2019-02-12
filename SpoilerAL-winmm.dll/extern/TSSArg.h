#pragma once

#include <windows.h>
#include "bcb6_std_string.h"

typedef enum {
	atUNKNOWN,
	atLONG,
	atLONG_INDEX,
	atBOOL,
	atSTRING,
	atBOOL_VECTOR,
	atDOUBLE,
	atNONE,
	atDIR,
	atSPLIT
} ssgCtrl;

#pragma pack(push, 1)
typedef struct _TSSArg
{
	LPVOID       *VTable;
	unsigned long type;
} TSSArg;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgLong
{
	LPVOID       *VTable;
	unsigned long type;
	long          value;
} TSSArgLong, TSSArgLongIndex;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgBool
{
	LPVOID       *VTable;
	unsigned long type;
	BOOLEAN       value;
} TSSArgBool;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgString
{
	LPVOID         *VTable;
	unsigned long   type;
	bcb6_std_string value;
} TSSArgString;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _Bit_iter {
	unsigned int* _M_p;
	unsigned int  _M_offset;
} _Bit_iterator;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgBoolVector {
	LPVOID       *VTable;
	unsigned long type;
	struct _Bvector {
		_Bit_iterator _M_start;
		_Bit_iterator _M_finish;
		void         *padding1;
		void         *padding2;
		unsigned int *_M_end_of_storage;
		void         *padding3;
	} value;
} TSSArgBoolVector;
#pragma pack(pop)

#define vector_bool_size(bvec) (size_t)((bvec._M_finish._M_p - bvec._M_start._M_p << 5)\
	+ bvec._M_finish._M_offset - bvec._M_start._M_offset)

#pragma pack(push, 1)
typedef struct _TSSArgDouble
{
	LPVOID       *VTable;
	unsigned long type;
	double        value;
} TSSArgDouble;
#pragma pack(pop)

#define TSSArg_dtor(Arg, bfDel) ((void (__cdecl *)(void*, BYTE))(Arg)->VTable[0])(Arg, bfDel)
#define delete_TSSArg(Arg) TSSArg_dtor(Arg, 0x03)
typedef void(__cdecl * const LPFN_TSSARG_TOSTRING)(bcb6_std_string *ret, TSSArg *this);
#define TSSArg_ToString(ret, Arg) ((LPFN_TSSARG_TOSTRING)(Arg)->VTable[2])(ret, Arg)

