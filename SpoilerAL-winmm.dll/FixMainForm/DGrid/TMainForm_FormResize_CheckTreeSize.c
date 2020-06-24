#include <windows.h>

extern const DWORD _TControl_SetWidth;

__declspec(naked) void __cdecl TMainForm_FormResize_CheckTreeSize()
{
	__asm
	{
		#define NextCallAddress        00446E88H
		#define AllWidthCheck          (esp + 8)
		#define MainForm               ebx
		#define DGrid                  (ebx + 3A4H)
		#define Splitter               (ebx + 2FCH)
		#define offsetof_Splitter_Left 40H

		mov     dword ptr [AllWidthCheck], 1
		mov     edx, dword ptr [Splitter]
		mov     eax, dword ptr [DGrid]
		mov     edx, dword ptr [edx + offsetof_Splitter_Left]
		push    NextCallAddress
		jmp     dword ptr [_TControl_SetWidth]

		#undef NextCallAddress
		#undef AllWidthCheck
		#undef MainForm
		#undef DGrid
		#undef Splitter
		#undef offsetof_Splitter_Left
	}
}
