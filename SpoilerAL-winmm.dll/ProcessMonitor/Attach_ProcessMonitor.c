#include <windows.h>
#include "TProcessCtrl.h"
#include "TMainForm.h"
#include "TCanvas.h"
#include "TProcessSearchReportListnerBase.h"

EXTERN_C void __cdecl TSearchReportListner_OnReport_InvokeDrawProgress(WPARAM searchForm, unsigned long Pos);
EXTERN_C void __cdecl TSearchForm_AdjustValToString_GetStart(LPVOID activeElement, LPVOID ssgCtrl);
EXTERN_C void __fastcall TSearchForm_DrawCanvas(TProcessSearchReportListnerBase *reportListner, long ImageWidth, unsigned long Pos, TCanvas *Canv);
EXTERN_C void __cdecl TSearchForm_AddressLBoxDblClick_SubjectAccess(TMainForm* mainForm, TSSGSubject* SelectS);
EXTERN_C void __cdecl LoadHeapList(TProcessCtrl *this);

#define USE_INTERNAL_SPECIFICATION_OF_HEAP_ID 1

#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
EXTERN_C BOOL __cdecl VerifyInternalSpecificationOfHeapID();
#endif

#define JMP_REL32 (BYTE )0xE9
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void __cdecl Attach_ProcessMonitor()
{
	// TSearchReportListner::OnReport
	//	must call from same thread
	*(LPDWORD)0x00490706 = (DWORD)TSearchReportListner_OnReport_InvokeDrawProgress - (0x00490706 + sizeof(DWORD));

	// TSearchReportListner::OnFindAddeess
	//	This->GetActiveElement()->GetStart(*This->GetSSGCtrl()) => this.start
	*(LPWORD )0x004907AE = 0x9066;
	*(LPWORD )0x004907B0 = 0x478B;
	*(LPBYTE )0x004907B2 =   0x14;

	// TSearchForm::UpdateStruct
	//	normalize calculating order
	*(LPDWORD)0x00494175 = 0x006218A0;
	*(LPDWORD)0x00494188 = 0x004D1858 - (0x00494188 + sizeof(DWORD));
	*(LPDWORD)0x004941B4 = 0x334;

	*(LPDWORD)0x00494219 = 0x0062188E;
	*(LPDWORD)0x0049422C = 0x004D187C - (0x0049422C + sizeof(DWORD));
	*(LPDWORD)0x00494258 = 0x32C;

	*(LPDWORD)0x004942BD = 0x00621897;
	*(LPDWORD)0x004942D0 = 0x004D18A0 - (0x004942D0 + sizeof(DWORD));
	*(LPDWORD)0x004942FC = 0x330;

	// TSearchForm::AdjustValToString
	//	activeElement->GetStart(*ssgCtrl) => (searchMode == 1 ? reportListner.start : activeElement->GetStart(*ssgCtrl))
	*(LPDWORD)0x004946F5 = (DWORD)TSearchForm_AdjustValToString_GetStart - (0x004946F5 + sizeof(DWORD));

	// TSearchForm::DrawProgress
#ifndef USE_ORIGINAL
	/*
		push    esi                                     ; 0049492D _ 56
		lea     ecx, [ebx + 400H]                       ; 0049492E _ 8D. 8B, 00000400
		push    dword ptr [ebp + 0CH]                   ; 00494934 _ FF. 75, 0C
		call    TSearchForm_DrawCanvas                  ; 00494937 _ E8, ????????
		jmp     00494B39H                               ; 0049493C _ E9, 000001F8
	*/
	*(LPBYTE )0x0049492D =       0x56;
	*(LPWORD )0x0049492E =     0x8B8D;
	*(LPDWORD)0x00494930 = 0x00000400;
	*(LPDWORD)0x00494934 = 0xE80C75FF;
	*(LPDWORD)0x00494938 = (DWORD)TSearchForm_DrawCanvas - (0x00494938 + sizeof(DWORD));
	*(LPBYTE )0x0049493C = JMP_REL32;
	*(LPDWORD)0x0049493D = 0x00494B39 - (0x0049493D + sizeof(DWORD));
#else
	//	activeElement->GetStart(*ssgCtrl) => reportListner.start
	*(LPBYTE )0x00494941 =       0x8D;
	*(LPWORD )0x00494942 =     0x008B;
	*(LPDWORD)0x00494944 = 0x83000004;
	*(LPDWORD)0x00494948 = 0x418B04EC;
	*(LPBYTE )0x0049494C =       0x14;
	//	activeElement->GetMin(*ssgCtrl)   => reportListner.min
	*(LPBYTE )0x00494973 =       0x8D;
	*(LPDWORD)0x00494974 = 0x0004008B;
	*(LPDWORD)0x00494978 = 0x04EC8300;
	*(LPWORD )0x0049497C =     0x418B;
	*(LPBYTE )0x0049497E =       0x18;
	//	activeElement->GetMax(*ssgCtrl)   => reportListner.max
	*(LPDWORD)0x00494A3C = 0x04008B8D;
	*(LPDWORD)0x00494A40 = 0xEC830000;
	*(LPDWORD)0x00494A44 = 0x1C418B04;
#endif

	// TSearchForm::SerachStartToolBtnClick
	//	normalize calculating order
	*(LPDWORD)0x00495026 = 0x004D1858 - (0x00495026 + sizeof(DWORD));
	*(LPDWORD)0x0049502F = -0x104;

	*(LPDWORD)0x0049505E = 0x004D18A0 - (0x0049505E + sizeof(DWORD));
	*(LPDWORD)0x00495064 = -0xFC;

	// TSearchForm::SerachStopToolBtnClick
	//	if(searchMode==0) return; => if(searchMode!=1 || reportListner.reserveMode==prcsCtrl::rsEND) return;
	*(LPDWORD)0x004955B4 = 0x0003CC05;
	*(LPDWORD)0x004955B8 = 0x01388000;
	*(LPDWORD)0x004955BC = 0x78830975;
	*(LPDWORD)0x004955C0 = 0x03740338;
	*(LPDWORD)0x004955C4 = 0xC30300C6;

	// TSearchForm::AddressLBoxDblClick
	*(LPDWORD)0x00497048 = (DWORD)TSearchForm_AddressLBoxDblClick_SubjectAccess - (0x00497048 + sizeof(DWORD));

	// TProcessCtrl::Clear
	//   adjust register so that return false
	*(LPBYTE )(0x004A34A6 + 1) = 0xC0;
	*(LPBYTE )(0x004A34A8 + 1) = 0x83;

	// TProcessCtrl::LoadHeapList
#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
	if (VerifyInternalSpecificationOfHeapID())
#endif
	{
		*(LPBYTE )0x004A3980 = JMP_REL32;
		*(LPDWORD)0x004A3981 = (DWORD)LoadHeapList - (0x004A3981 + sizeof(DWORD));
		*(LPDWORD)0x004A3985 = NOP_X4;
	}

	// TProcessCtrl::FindProcess
	*(LPBYTE )0x004A5ACC = JMP_REL32;
	*(LPDWORD)0x004A5ACD = (DWORD)TProcessCtrl_FindProcess - (0x004A5ACD + sizeof(DWORD));
	*(LPDWORD)0x004A5AD1 = NOP_X4;

	// TProcessCtrl::Attach(string ProcessName)
	*(LPBYTE )0x004A5FF0 = JMP_REL32;
	*(LPDWORD)0x004A5FF1 = (DWORD)TProcessCtrl_AttachByProcessName - (0x004A5FF1 + sizeof(DWORD));
	*(LPDWORD)0x004A5FF5 = NOP_X4;

	// TProcessCtrl::Attach(void)
	*(LPBYTE )0x004A610C = JMP_REL32;
	*(LPDWORD)0x004A610D = (DWORD)TProcessCtrl_Attach - (0x004A610D + sizeof(DWORD));
	*(LPBYTE )0x004A6111 = NOP;

	// TProcessCtrl::SearchFunc
	//	missing Listner->OnSearchEnd()
	*(LPBYTE )0x004A6B16 = JMP_REL32;
	*(LPDWORD)0x004A6B17 = 0x004A6D9E - (0x004A6B17 + sizeof(DWORD));
	//	missing Listner->OnSearchEnd()
	*(LPBYTE )0x004A6C73 = JMP_REL32;
	*(LPDWORD)0x004A6C74 = 0x004A6B9A - (0x004A6C74 + sizeof(DWORD));
}
