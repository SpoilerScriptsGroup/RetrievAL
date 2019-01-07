#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

EXTERN_C void __cdecl AddressNamingAdditionalType();

static void __fastcall TSSGCtrl_AddressNaming_return_Data(
	      string*        const retVal,
	const string*        const Data,
	const vector_string* const tmpV,
	      unsigned long  const DataSize)
{
	string* format = &vector_type_at(tmpV, string, 5);
	string_ctor_assign(retVal, Data);
	if (!string_empty(format)) {
		int len;
		string_reserve(retVal, DataSize);
		len = _snprintf(string_begin(retVal), DataSize + 1, string_c_str(format), string_c_str(Data));
		if (len >= 0) string_end(retVal) = string_begin(retVal) + len;
	}
}

static __declspec(naked) void __cdecl TSSGCtrl_AddressNaming_return_DataStub(string* retVal, string* Data) {
	__asm {
		mov  edx, [ebp - 0x01EC]
		xchg edx, [esp]
		push esi
		push edx
		mov  edx, ecx
		mov  ecx, eax
		jmp  TSSGCtrl_AddressNaming_return_Data
	}
}

EXTERN_C void __cdecl Attach_AddressNamingAdditionalType()
{
	// TSSGCtrl::AddressNaming
	*(LPDWORD)(0x00505E11 + 2) = (DWORD)AddressNamingAdditionalType - (0x00505E11 + 2 + sizeof(DWORD));

	//   tmpV[2] => tmpV[3]
	*(LPBYTE )(0x0050673D + 2) = sizeof(string) * 3;

	//   return Data;
	*(LPDWORD)(0x00506A21 + 1) = (DWORD)TSSGCtrl_AddressNaming_return_DataStub - (0x00506A21 + 1 + sizeof(DWORD));
}
