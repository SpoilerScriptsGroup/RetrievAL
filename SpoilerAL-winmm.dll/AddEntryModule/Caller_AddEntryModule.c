void __stdcall AddEntryModule(void *moduleList, unsigned long th32ProcessID);

__declspec(naked) void __cdecl Caller_AddEntryModule()
{
	__asm
	{
		mov     eax, dword ptr [ebp + 8]
		sub     dword ptr [ebp - 56], 2

		#define _this                                     eax
		#define offsetof_TProcessCtrl_entry               192
		#define offsetof_TProcessCtrl_entry_th32ProcessID 200
		#define offsetof_TProcessCtrl_moduleList          512
		#define ReturnAddress                             004A50DDH

		mov     ecx, dword ptr [_this + offsetof_TProcessCtrl_entry_th32ProcessID]
		add     eax, offsetof_TProcessCtrl_moduleList
		push    ecx
		push    eax
		push    ReturnAddress
		jmp     AddEntryModule

		#undef _this
		#undef offsetof_TProcessCtrl_entry
		#undef offsetof_TProcessCtrl_entry_th32ProcessID
		#undef offsetof_TProcessCtrl_moduleList
		#undef ReturnAddress
	}
}
