#include <windows.h>
#include <intrin.h>
#include "ShowErrorMessage/ErrorMessage.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "TSSGActionListner.h"
#include "TMainForm.h"

#pragma intrinsic(_ReturnAddress)

#define ALLOCATE_SUPPORT 1

typedef struct {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;

extern size_t             nNumberOfProcessMemory;
extern PROCESSMEMORYBLOCK *lpProcessMemory;
extern FILETIME           ftProcessCreationTime;
extern TSSGSubject        dummySSGS;

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

BOOL   IsProcessAttached = FALSE;
BOOL   InProcessDetached = FALSE;
string ProcessAttachCode = { NULL };
vector *ProcessAttachAttribute = NULL;
string ProcessDetachCode = { NULL };
vector *ProcessDetachAttribute = NULL;

static void __cdecl FreeExternalDependency(TProcessCtrl *const proc)
{
#if ALLOCATE_SUPPORT
	if (lpProcessMemory && nNumberOfProcessMemory && (ftProcessCreationTime.dwLowDateTime || ftProcessCreationTime.dwHighDateTime))
	{
		FILETIME creationTime, exitTime, kernelTime, userTime;
		HANDLE   hProcess = NULL;
		size_t   i = nNumberOfProcessMemory;

		if ((long)proc->entry.th32ProcessID > 0
			&& (hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION, FALSE, proc->entry.th32ProcessID))
			&& GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)
			&& creationTime.dwLowDateTime  == ftProcessCreationTime.dwLowDateTime
			&& creationTime.dwHighDateTime == ftProcessCreationTime.dwHighDateTime
			)
		{
			do
				if (lpProcessMemory[--i].Protect && lpProcessMemory[i].Address)
				{
					LPSTR lpBuffer;
					if (!VirtualFreeEx(hProcess, lpProcessMemory[i].Address, 0, MEM_RELEASE) &&
						TMainForm_GetUserMode(MainForm) > 1 &&
						FormatMessageA(
							FORMAT_MESSAGE_MAX_WIDTH_MASK |
							FORMAT_MESSAGE_ALLOCATE_BUFFER |
							FORMAT_MESSAGE_IGNORE_INSERTS |
							FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							GetLastError(),
							0,
							(LPSTR)&lpBuffer,
							sizeof(double),
							NULL))
					{
						TMainForm_Guide(lpBuffer, 0);
						LocalFree(lpBuffer);
					}
					lpProcessMemory[i].Address = NULL;
				}
			while (i);
		}
		else
		{
			do
				if (lpProcessMemory[--i].Protect)
					lpProcessMemory[i].Address = NULL;
			while (i);
		}
		if (hProcess) CloseHandle(hProcess);

		ftProcessCreationTime = (const FILETIME) { 0, 0 };
	}
#endif
}

//---------------------------------------------------------------------
//実行ファイル名を元に、対象プロセスを所得する
//
//	＜成功するとtrueを返します＞
//	・OpenProcess等の前に、必ず行うこと
//	・実行ファイル名は、大文字小文字を区別しません。
//---------------------------------------------------------------------
BOOLEAN __cdecl TProcessCtrl_AttachByProcessName(TProcessCtrl *this, string ProcessName)
{
	static __inline void OnProcessAttach();

	BOOLEAN         Result;
	PROCESSENTRY32A PE32;

	if (Result = TProcessCtrl_FindProcess(this, &ProcessName, &PE32) == 0)
	{
		this->entry = PE32;
		SetLastError(ClearErrorMessageId());

		// 取得可能だったファイル名を保持
		string_assign(&this->attachedProcessName, &ProcessName);

		// 成功ならヒープブロック/モジュールまで所得しておく
		TProcessCtrl_LoadHeapList(this);
		TProcessCtrl_LoadModuleList(this);

		OnProcessAttach();
	}
	string_dtor(&ProcessName);
	return Result;
}
//---------------------------------------------------------------------
//実行ファイル名を元に、対象プロセスを所得する
//
//	＜成功するとtrueを返します＞
//	・事前に、SetProcessNameVec()でベクタを指定しておくこと。
//	・OpenProcess等の前に、必ず行うこと
//	・実行ファイル名は、大文字小文字を区別しません。
//	・該当するプロセス名のプロセスが一つでもあればtrueを返します。
//---------------------------------------------------------------------
BOOLEAN __cdecl TProcessCtrl_Attach(TProcessCtrl *this)
{
	void __cdecl OnProcessDetach(TProcessCtrl *proc);

	if ((long)this->entry.th32ProcessID > 0 && (uintptr_t)_ReturnAddress() == 0x004A61B0u)// TProcessCtrl::Open
	{// reset process infomation when lost target
		vector_string pnames = this->processNameVec;
		FreeExternalDependency(this);
		this->processNameVec = (const vector_string) { NULL };
		TProcessCtrl_Clear(this);
		this->processNameVec = pnames;
		OnProcessDetach(this);
	}

	// ベクタに積まれたプロセス名を順々にチェック
	if (!InProcessDetached) for (const string *it = vector_begin(&this->processNameVec); it < vector_end(&this->processNameVec); it++)
	{
		string ProcessName;

		string_ctor_assign(&ProcessName, it);
		if (TProcessCtrl_AttachByProcessName(this, ProcessName))
			// Attachに成功した時点で、trueを返す
			return TRUE;
	}

	if (!vector_empty(&this->processNameVec)) SetLastError(SetErrorMessageId(ERROR_FLT_INSTANCE_NOT_FOUND));
	return FALSE;
}
//---------------------------------------------------------------------
static __inline void OnProcessAttach()
{
	if (!IsProcessAttached)
	{
		IsProcessAttached = TRUE;
		if (!string_empty(&ProcessAttachCode))
		{
			TSSGActionListner_OnProcessOpen(&MainForm->adjustmentListner, NULL, 0);
			dummySSGS.attribute = ProcessAttachAttribute;
			Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessAttachCode, 0);
			dummySSGS.attribute = NULL;
		}
	}
}
//---------------------------------------------------------------------
void __cdecl OnProcessDetach(TProcessCtrl *const proc)
{
	if (IsProcessAttached)
	{
		IsProcessAttached = FALSE;
		if (!string_empty(&ProcessDetachCode))
		{
			TSSGActionListner_OnProcessOpen(&MainForm->adjustmentListner, NULL, 0);
			InProcessDetached = TRUE;
			dummySSGS.attribute = ProcessDetachAttribute;
			Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessDetachCode, 0);
			dummySSGS.attribute = NULL;
			InProcessDetached = FALSE;
		}
		{
			static void(__cdecl *const tree_string_vecstr_erase)(struct _Rb_tree *this, struct _Rb_tree_node *__x) = (void *)0x00469A5C;

			map *const dataFileMap = &MainForm->ssgCtrl.dataFileMap;
			map_iterator it = map_begin(dataFileMap);
			while (it != map_end(dataFileMap))
			{
				const string *const key = pair_first(it);
				if (string_at(key, 0) == '_')
				{
					bcb6_std_map_node *const node = tree_erase_sans_delete(dataFileMap, it);
					map *const tmpM = pair_second_aligned(node, string);

					if (tmpM->_M_node_count)
						tree_string_vecstr_erase(tmpM, map_end(tmpM)->_M_parent);
					node_alloc_deallocate(map_end(tmpM)
#if !OPTIMIZE_ALLOCATOR
										  , sizeof(bcb6_std_set_node) + sizeof(string) + sizeof(vector_string)
#endif
					);
					string_dtor(pair_first(node));

					map_iterator_increment(it);
					node_alloc_deallocate(node
#if !OPTIMIZE_ALLOCATOR
										  , sizeof(bcb6_std_set_node) + sizeof(string) + sizeof(map)
#endif
					);
				}
				else
					map_iterator_increment(it);
			}
		}
		FreeExternalDependency(proc);
		{
			vector_string pnames = proc->processNameVec;
			proc->processNameVec = (const vector_string) { NULL };
			TProcessCtrl_Clear(proc);
			proc->processNameVec = pnames;
		}
	}
}
