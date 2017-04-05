#include "TProcessCtrl.h"

THeapListData *(__cdecl *TProcessCtrl_GetHeapList)(TProcessCtrl *_this, unsigned long ListNo) = (LPVOID)0x004A5A94;
HANDLE(__cdecl *TProcessCtrl_Open)(TProcessCtrl *_this, DWORD Mode) = (LPVOID)0x004A618C;
BOOLEAN(__cdecl *TProcessCtrl_OneRead)(HANDLE ProcessHandle, DWORD Address, void *Data, DWORD Size) = (LPVOID)0x004A6FF4;

