#include <windows.h>

void __cdecl TSSDoubleList_SwitchStatus();

__declspec(naked) void __cdecl TSSDoubleList_Read_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004C4390,
		0x004C445A,
		0x004C4B4D,
		0x004C5209
	};

	__asm
	{
		#define ListFile (ebp - 160H)
		#define i        (ebp - 16CH)

		mov     ecx, dword ptr [ListFile]
		mov     edx, dword ptr [i]
		mov     eax, offset AddressTable
		jmp     TSSDoubleList_SwitchStatus

		#undef ListFile
		#undef i
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Write_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004C5477,
		0x004C555A,
		0x004C5BF4,
		0x004C627F
	};

	__asm
	{
		#define ListFile (ebp - 15CH)
		#define Index    (ebp - 160H)

		mov     ecx, dword ptr [ListFile]
		mov     edx, dword ptr [Index]
		mov     eax, offset AddressTable
		jmp     TSSDoubleList_SwitchStatus

		#undef ListFile
		#undef Index
	}
}

__declspec(naked) void __cdecl TSSDoubleList_ToByteCode_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004C6755,
		0x004C6A01,
		0x004C721F,
		0x004C7A1E
	};

	__asm
	{
		#define ListFile (ebp - 27CH)
		#define Index    (ebp - 288H)

		mov     ecx, dword ptr [ListFile]
		mov     edx, dword ptr [Index]
		mov     eax, offset AddressTable
		jmp     TSSDoubleList_SwitchStatus

		#undef ListFile
		#undef Index
	}
}

__declspec(naked) void __cdecl TSSDoubleList_SwitchStatus()
{
	__asm
	{
		#define ListFile     ecx
		#define Index        edx
		#define AddressTable eax

		mov     ecx, dword ptr [ListFile]
		lea     edx, [Index + Index * 2]
		mov     ecx, dword ptr [ecx + edx * 8]
		mov     cl, byte ptr [ecx]
		cmp     cl, ','
		je      L1
		cmp     cl, '@'
		je      L2
		test    cl, cl
		jz      L3
		jmp     dword ptr [AddressTable     ]
	L1:
		jmp     dword ptr [AddressTable +  4]
	L2:
		jmp     dword ptr [AddressTable +  8]
	L3:
		mov     edx, dword ptr [AddressTable + 12]
		mov     eax, 4// ssgCtrl::reINDEX_ERROR
		jmp     edx

		#undef ListFile
		#undef Index
		#undef AddressTable
	}
}

