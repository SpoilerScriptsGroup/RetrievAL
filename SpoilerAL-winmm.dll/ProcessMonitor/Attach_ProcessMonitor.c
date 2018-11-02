#include <windows.h>
#include "TProcessCtrl.h"

#define USE_INTERNAL_SPECIFICATION_OF_HEAP_ID 1

#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
EXTERN_C BOOL __cdecl VerifyInternalSpecificationOfHeapID();
#endif

#define JMP_REL32 (BYTE )0xE9
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void __cdecl Attach_ProcessMonitor()
{
	// TSearchReportListner::OnFindAddeess
	//	This->GetActiveElement()->GetStart(*This->GetSSGCtrl()) => this.start
	*(LPWORD )0x004907AE = 0x9066;
	*(LPWORD )0x004907B0 = 0x478B;
	*(LPBYTE )0x004907B2 =   0x14;

	// TSearchForm::AdjustValToString
	//	activeElement->GetStart(*ssgCtrl) => reportListner.start
#ifdef WITHHOLD
	*(LPBYTE )0x004946ED =       0x8D;
	*(LPWORD )0x004946EE =     0x008E;
	*(LPDWORD)0x004946F0 = 0x83000004;
	*(LPDWORD)0x004946F4 = 0x418B04EC;
	*(LPBYTE )0x004946F8 =       0x14;
#endif

	// TSearchForm::DrawProgress
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

	// TSearchForm::SerachStopToolBtnClick
	//	if(searchMode==0) return; => if(searchMode!=1 || reportListner.reserveMode==prcsCtrl::rsEND) return;
	*(LPDWORD)0x004955B4 = 0x0003CC05;
	*(LPDWORD)0x004955B8 = 0x01388000;
	*(LPDWORD)0x004955BC = 0x78830975;
	*(LPDWORD)0x004955C0 = 0x03740338;
	*(LPDWORD)0x004955C4 = 0xC30300C6;

	// TProcessCtrl::LoadHeapList
#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
	if (VerifyInternalSpecificationOfHeapID())
#endif
	{
		*(LPBYTE )0x004A3980 = JMP_REL32;
		*(LPDWORD)0x004A3981 = (DWORD)TProcessCtrl_LoadHeapList - (0x004A3981 + sizeof(DWORD));
		*(LPDWORD)0x004A3985 = NOP_X4;
	}

	// TProcessCtrl::FindProcess
	*(LPBYTE )0x004A5ACC = JMP_REL32;
	*(LPDWORD)0x004A5ACD = (DWORD)TProcessCtrl_FindProcess - (0x004A5ACD + sizeof(DWORD));
	*(LPDWORD)0x004A5AD1 = NOP_X4;

	// TProcessCtrl::SearchFunc
	//	missing Listner->OnSearchEnd()
	*(LPBYTE )0x004A6B16 = JMP_REL32;
	*(LPDWORD)0x004A6B17 = 0x004A6D9E - (0x004A6B17 + sizeof(DWORD));
}
