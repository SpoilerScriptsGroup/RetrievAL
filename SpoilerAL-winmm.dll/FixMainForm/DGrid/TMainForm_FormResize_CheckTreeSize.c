
__declspec(naked) void __cdecl TMainForm_FormResize_CheckTreeSize()
{
	__asm
	{
		#define NextCallAddress        00446E88H
		#define DGrid_SetWidth         00580430H
		#define AllWidthCheck          (esp + 8)
		#define MainForm               ebx
		#define DGrid                  (ebx + 3A4H)
		#define Splitter               (ebx + 2FCH)
		#define offsetof_Splitter_Left 40H

		mov     dword ptr [AllWidthCheck], 1
		mov     ecx, DGrid_SetWidth
		mov     edx, dword ptr [Splitter]
		mov     eax, dword ptr [DGrid]
		mov     edx, dword ptr [edx + offsetof_Splitter_Left]
		push    NextCallAddress
		jmp     ecx

		#undef NextCallAddress
		#undef DGrid_SetWidth
		#undef AllWidthCheck
		#undef MainForm
		#undef DGrid
		#undef Splitter
		#undef offsetof_Splitter_Left
	}
}
