#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"

void(__cdecl * const TProcessCtrl_Clear)(TProcessCtrl *this) = (LPVOID)0x004A3350;
void(__cdecl * const TProcessCtrl_LoadHeapList)(TProcessCtrl *this) = (LPVOID)0x004A3980;
THeapListData *(__cdecl * const TProcessCtrl_GetHeapList)(TProcessCtrl *this, unsigned long ListNo) = (LPVOID)0x004A5A94;
HANDLE(__cdecl * const TProcessCtrl_Open)(TProcessCtrl *this, DWORD Mode) = (LPVOID)0x004A618C;
BOOLEAN(__cdecl * const TProcessCtrl_OneRead)(HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size) = (LPVOID)0x004A6FF4;

extern const DWORD F004A61F8;

__declspec(naked) LPMODULEENTRY32A __fastcall TProcessCtrl_GetModuleFromName(TProcessCtrl *this, LPCSTR Name)
{
	__asm
	{
		sub     esp, size string
		mov     eax, ecx
		mov     ecx, esp
		push    eax
		call    string_ctor_assign_cstr
		call    dword ptr [F004A61F8]
		add     esp, 4 + size string
		ret
	}
}

//---------------------------------------------------------------------
//対象プロセスのモジュール一覧を所得
//
//  ・ATOKや各種DLLも引っ張ってくるので、選別は適時/各自行ってください
//	・すでにAttachを行っていること
//	　（というか、Attach()内から呼び出される）
//---------------------------------------------------------------------
void __cdecl TProcessCtrl_LoadModuleList(TProcessCtrl *const this)
{
	HANDLE const Snapshot//モジュール列挙用スナップショット
		= CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->entry.th32ProcessID);
	vector_clear(&this->moduleList);
	if (Snapshot)
	{
		MODULEENTRY32A ME32 = { sizeof(ME32) };
		//モジュールの所得
		if (Module32FirstA(Snapshot, &ME32))
		{
			MODULEENTRY32A const EntryModule = ME32;
			do//hModuleが、アクセス用アドレス
				vector_push_back(&this->moduleList, ME32);
			while (Module32NextA(Snapshot, &ME32));
			*(LPDWORD)EntryModule.szModule = BSWAP32('nul\0');
			vector_push_back(&this->moduleList, EntryModule);
		}
		CloseHandle(Snapshot);
	}
}
