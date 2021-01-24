#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

void(__cdecl * const TSSGCtrl_LockClear)(TSSGCtrl *this) = (LPVOID)0x004E4408;
BOOLEAN(__cdecl * const TSSGCtrl_IsEnabled)(TSSGCtrl *this, TSSGSubject *SSGS) = (LPVOID)0x005111E0;
TSSGSubject *(__cdecl * const TSSGCtrl_MakeSubjectClass)(const string *Tag) = (LPVOID)0x004EC3EC;
LPVOID(__cdecl * const TSSGCtrl_MakeAdjustmentClass)(const string *) = (LPVOID)0x004ED738;
TSSGSubject *(__cdecl * const TSSGCtrl_GetTargetSubject)(TSSGSubject *SSGS) = (LPVOID)0x004EDF84;
void(__cdecl * const TSSGCtrl_SetSSGDataFile)(TSSGCtrl *this, vector_string *Data, string *FileName, BOOLEAN IsSSL) = (LPVOID)0x004F0A48;
BOOLEAN (__cdecl * const TSSGCtrl_IsLocked)(TSSGCtrl *this, TSSGSubject *SSGS) = (LPVOID)0x004FE9B0;
vector_string *(__cdecl * const TSSGCtrl_ReadSSRFile)(vector_string *Result, TSSGCtrl *this, const string *Code, vector_dword *Indices, TSSGSubject *SSGS) = (LPVOID)0x004FEAEC;
void(__cdecl * const TSSGCtrl_LoopSSRFile)(TSSGCtrl *this, vector *FormatVec, vector *Dest, unsigned long LoopVal) = (LPVOID)0x00501C2C;
LPVOID(__cdecl * const TSSGCtrl_GetAttribute)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Type) = (LPVOID)0x005038E8;
unsigned long(__cdecl * const TSSGCtrl_GetAddress)(TSSGCtrl *this, TSSGSubject *SSGS, const string *AddressStr, unsigned long Mode) = (LPVOID)0x00503920;
unsigned long(__cdecl * const TSSGCtrl_GetDirLevel)(TSSGCtrl *this, TSSGSubject *SSGS) = (LPVOID)0x00503FA4;
unsigned long(__cdecl * const TSSGCtrl_AddressAttributeFilter)(TSSGCtrl *this, TSSGSubject *SSGS, DWORD *Address, unsigned long Mode) = (LPVOID)0x00504130;
BOOLEAN(__cdecl * const TSSGCtrl_CheckFunnel)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Val) = (LPVOID)0x0051014C;
BOOLEAN(__cdecl * const TSSGCtrl_Funneling)(TSSGCtrl *this, TSSGSubject *SSGS, const string *FileName, unsigned long Val) = (LPVOID)0x00510208;
unsigned long(__cdecl * const TSSGCtrl_CheckIO_FEP)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Val, BOOLEAN IsInput) = (LPVOID)0x00510F4C;
double(__cdecl * const TSSGCtrl_CheckIO_FEPDouble)(TSSGCtrl *this, TSSGSubject *SSGS, double Val, BOOLEAN IsInput) = (LPVOID)0x00511094;
vector_string *(__cdecl * const TSSGCtrl_GetSSGDataFile)(TSSGCtrl *this, TSSGSubject *SSGS, string FName, string DefaultExt, string *CurrentDir) = (LPVOID)0x004EDFB8;
string *(__cdecl * const TSSGCtrl_GetSimpleByteCode)(string *Result, TSSGCtrl *this, TSSGSubject *SSGS, string CodeStr) = (LPVOID)0x00506BAC;
BOOLEAN(__cdecl * const TSSGCtrl_IsChildRWProhibit)(TSSGCtrl *this, TSSGSubject *SSGS) = (LPVOID)0x00511318;
BOOLEAN(__cdecl * const TSSGCtrl_OneRead)(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size) = (LPVOID)0x0051C4D8;
BOOLEAN(__cdecl * const TSSGCtrl_OneWrite)(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size) = (LPVOID)0x0051C540;
void(__cdecl * const TSSGCtrl_SetLock)(TSSGCtrl *this, BOOLEAN IsLock, TSSGSubject *Target, void *Value) = (LPVOID)0x004FE200;
