
__declspec(naked) void __cdecl FixGetModuleFromName()
{
	__asm
	{
		#define ProcessCtrl                              ecx
		#define offsetof_ProcessCtrl_entry_th32ProcessID 200
		#define ProcessCtrl_LoadModuleList               004A4E4CH
		#define ProcessCtrl_Attach                       004A610CH

		mov     ecx, dword ptr [ProcessCtrl + offsetof_ProcessCtrl_entry_th32ProcessID]
		mov     eax, ProcessCtrl_LoadModuleList
		test    ecx, ecx
		jz      L1
		jmp     eax
	L1:
		mov     eax, ProcessCtrl_Attach
		jmp     eax

		#undef ProcessCtrl
		#undef offsetof_ProcessCtrl_entry_th32ProcessID
		#undef ProcessCtrl_LoadModuleList
		#undef ProcessCtrl_Attach
	}
}
