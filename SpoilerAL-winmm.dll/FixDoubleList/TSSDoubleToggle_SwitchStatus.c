#include <windows.h>

void __cdecl TSSDoubleToggle_SwitchStatus();

__declspec(naked) void __cdecl TSSDoubleToggle_Read_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004C9953,
		0x004CA096,
		0x004CA748
	};

	__asm
	{
		#define ListFile (ebp - 1C4H)
		#define i        (ebp - 1D0H)

		mov     ecx, dword ptr [ListFile]
		mov     edx, dword ptr [i]
		mov     eax, offset AddressTable
		jmp     TSSDoubleToggle_SwitchStatus

		#undef ListFile
		#undef i
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_Write_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004CC1FD,
		0x004CD030,
		0x004CD8F0
	};

	__asm
	{
		#define ListFile (ebp - 22CH)
		#define i        (ebp - 238H)

		mov     ecx, dword ptr [ListFile]
		mov     edx, dword ptr [i]
		mov     eax, offset AddressTable
		jmp     TSSDoubleToggle_SwitchStatus

		#undef ListFile
		#undef i
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_SwitchStatus()
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
		cmp     cl, '@'
		je      L1
		test    cl, cl
		jz      L2
		jmp     dword ptr [AddressTable     ]

	L1:
		jmp     dword ptr [AddressTable +  4]

	L2:
		jmp     dword ptr [AddressTable +  8]

		#undef ListFile
		#undef Index
		#undef AddressTable
	}
}

