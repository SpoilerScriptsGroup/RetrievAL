#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

extern TSSGSubject dummySSGS;

BOOL   IsProcessAttached = 0;
string ProcessAttachCode = { NULL };
string ProcessDetachCode = { NULL };

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
	void __cdecl OnProcessDetach();

	string *it;

	// ベクタに積まれたプロセス名を順々にチェック
	for (it = vector_begin(&this->processNameVec); it != vector_end(&this->processNameVec); it++)
	{
		string ProcessName;

		string_ctor_assign(&ProcessName, it);
		if (TProcessCtrl_AttachByProcessName(this, ProcessName))
			// Attachに成功した時点で、trueを返す
			return TRUE;
	}

	OnProcessDetach();

	// all attaches failed, then clear cached process information
	TProcessCtrl_Clear(this);

	return FALSE;
}
//---------------------------------------------------------------------
static __inline void OnProcessAttach()
{
	if (IsProcessAttached)
		return;
	IsProcessAttached = TRUE;
	if (string_empty(&ProcessAttachCode))
		return;
	Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessAttachCode, 0);
}
//---------------------------------------------------------------------
void __cdecl OnProcessDetach()
{
	if (!IsProcessAttached)
		return;
	IsProcessAttached = FALSE;
	if (string_empty(&ProcessDetachCode))
		return;
	Parsing(&MainForm->ssgCtrl, &dummySSGS, &ProcessDetachCode, 0);
}
