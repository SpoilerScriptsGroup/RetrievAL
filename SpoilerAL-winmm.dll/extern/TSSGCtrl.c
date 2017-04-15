#include "TSSGCtrl.h"

void(__cdecl *TSSGCtrl_EnumReadSSG)(TSSGCtrl *_this, bcb6_std_vector *SSGFile, LPVOID ParentStack, LPVOID ADJElem, DWORD RepeatIndex, DWORD ParentRepeat) = (LPVOID)0x004E5090;
void(__cdecl *TSSGCtrl_SetSSGDataFile)(TSSGCtrl *_this, bcb6_std_vector *Data, bcb6_std_string *FileName, BOOLEAN IsSSL) = (LPVOID)0x004F0A48;
bcb6_std_vector *(__cdecl *TSSGCtrl_ReadSSRFile)(bcb6_std_vector *Result, TSSGCtrl *_this, bcb6_std_string *Code, bcb6_std_vector *Indices) = (LPVOID)0x004FEAEC;
void(__cdecl *TSSGCtrl_LoopSSRFile)(TSSGCtrl *_this, bcb6_std_vector *FormatVec, bcb6_std_vector *Dest, unsigned long LoopVal) = (LPVOID)0x00501C2C;
LPVOID(__cdecl *TSSGCtrl_GetAttribute)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Type) = (LPVOID)0x005038E8;
unsigned long(__cdecl *TSSGCtrl_GetDirLevel)(TSSGCtrl *_this, struct _TSSGSubject *SSGS) = (LPVOID)0x00503FA4;
unsigned long(__cdecl *TSSGCtrl_AddressAttributeFilter)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, DWORD *Address, unsigned long Mode) = (LPVOID)0x00504130;
unsigned long(__cdecl *TSSGCtrl_CheckIO_FEP)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Val, BOOLEAN IsInput) = (LPVOID)0x00510F4C;
double(__cdecl *TSSGCtrl_CheckIO_FEPDouble)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, double Val, BOOLEAN IsInput) = (LPVOID)0x00511094;
bcb6_std_vector *(__cdecl *TSSGCtrl_GetSSGDataFile)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, bcb6_std_string FName, bcb6_std_string DefaultExt, bcb6_std_string *CurrentDir) = (LPVOID)0x004EDFB8;
bcb6_std_string *(__cdecl *TSSGCtrl_GetSimpleByteCode)(bcb6_std_string *Result, TSSGCtrl *_this, struct _TSSGSubject *SSGS, bcb6_std_string CodeStr) = (LPVOID)0x00506BAC;
BOOLEAN(__cdecl *TSSGCtrl_IsChildRWProhibit)(TSSGCtrl *_this, struct _TSSGSubject *SSGS) = (LPVOID)0x00511318;
BOOLEAN(__cdecl *TSSGCtrl_OneRead)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size) = (LPVOID)0x0051C4D8;
BOOLEAN(__cdecl *TSSGCtrl_OneWrite)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size) = (LPVOID)0x0051C540;

