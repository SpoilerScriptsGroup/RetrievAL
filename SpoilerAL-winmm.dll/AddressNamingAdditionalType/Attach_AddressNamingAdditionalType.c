#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

EXTERN_C void __stdcall AddressNamingAdditionalType(void *, void *, void *);

static string* __fastcall TSSGCtrl_AddressNaming_return_Data(
	vector_string *const tmpV,
	unsigned long  const DataSize,
	string        *const retVal,
	string        *const Data)
{
	string *const format = &vector_at(tmpV, 5);
	if (string_empty(format))
	{
		*retVal = *Data;
		string_ctor_null(Data);
	}
	else
	{
		int len;
		string *const ErrorStr = &vector_at(tmpV, 4);
		*retVal = *ErrorStr;
		string_ctor_null(ErrorStr);
		string_reserve(retVal, DataSize);
		len = _snprintf(string_begin(retVal), string_storage_capacity(retVal), string_c_str(format), string_c_str(Data));
		if (len >= 0) string_end(retVal) = string_begin(retVal) + min(string_capacity(retVal), (size_t)len);
	}
	return retVal;
}

static __declspec(naked) string* __stdcall TSSGCtrl_AddressNaming_return_DataStub(string *retVal, string *Data) {
	__asm {
		mov  edx, [ebp - 0x01EC]
		mov  ecx, esi
		jmp  TSSGCtrl_AddressNaming_return_Data
	}
}

EXTERN_C void __cdecl Attach_AddressNamingAdditionalType()
{
	// TSSGCtrl::AddressNaming
	*(LPBYTE )(0x00505E04 + 1) = 0x00505E17 - (0x00505E04 + 1 + sizeof(BYTE));
	*(LPWORD ) 0x00505E06      = BSWAP16(0xFF75    );// push dword[SSGS]; push this
	*(LPDWORD) 0x00505E08      = BSWAP32(0x105756E8);// push offset tmpV; call
	*(LPDWORD)(0x00505E0B + 1) = (DWORD)AddressNamingAdditionalType - (0x00505E0B + 1 + sizeof(DWORD));
	*(LPWORD ) 0x00505E10      = BSWAP16(0x6690    );// nop
	*(LPBYTE )(0x00505E12 + 0) =         0xE9       ;// jmp

	//   tmpV[2] => tmpV[3]
	*(LPBYTE )(0x0050673D + 2) = sizeof(string) * 3;

	//   return Data;
	*(LPDWORD)(0x00506A21 + 1) = (DWORD)TSSGCtrl_AddressNaming_return_DataStub - (0x00506A21 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x00506A2D + 2) = 0;// stack size to discard
}
