#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

void(__cdecl *TSSGCtrl_EnumReadSSG)(TSSGCtrl *_this, vector_string *SSGFile, LPVOID ParentStack, LPVOID ADJElem, DWORD RepeatIndex, DWORD ParentRepeat) = (LPVOID)0x004E5090;
void(__cdecl *TSSGCtrl_SetSSGDataFile)(TSSGCtrl *_this, vector *Data, string *FileName, BOOLEAN IsSSL) = (LPVOID)0x004F0A48;
vector_string *(__cdecl *TSSGCtrl_ReadSSRFile)(vector_string *Result, TSSGCtrl *_this, string *Code, vector_dword *Indices) = (LPVOID)0x004FEAEC;
void(__cdecl *TSSGCtrl_LoopSSRFile)(TSSGCtrl *_this, vector *FormatVec, vector *Dest, unsigned long LoopVal) = (LPVOID)0x00501C2C;
LPVOID(__cdecl *TSSGCtrl_GetAttribute)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Type) = (LPVOID)0x005038E8;
unsigned long(__cdecl *TSSGCtrl_GetAddress)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, const string *AddressStr, unsigned long Mode) = (LPVOID)0x00503920;
unsigned long(__cdecl *TSSGCtrl_GetDirLevel)(TSSGCtrl *_this, struct _TSSGSubject *SSGS) = (LPVOID)0x00503FA4;
unsigned long(__cdecl *TSSGCtrl_AddressAttributeFilter)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, DWORD *Address, unsigned long Mode) = (LPVOID)0x00504130;
BOOLEAN(__cdecl *TSSGCtrl_CheckFunnel)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Val) = (LPVOID)0x0051014C;
BOOLEAN(__cdecl *TSSGCtrl_Funneling)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, const string *FileName, unsigned long Val) = (LPVOID)0x00510208;
unsigned long(__cdecl *TSSGCtrl_CheckIO_FEP)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, unsigned long Val, BOOLEAN IsInput) = (LPVOID)0x00510F4C;
double(__cdecl *TSSGCtrl_CheckIO_FEPDouble)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, double Val, BOOLEAN IsInput) = (LPVOID)0x00511094;
vector_string *(__cdecl *TSSGCtrl_GetSSGDataFile)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, string FName, string DefaultExt, string *CurrentDir) = (LPVOID)0x004EDFB8;
string *(__cdecl *TSSGCtrl_GetSimpleByteCode)(string *Result, TSSGCtrl *_this, struct _TSSGSubject *SSGS, string CodeStr) = (LPVOID)0x00506BAC;
BOOLEAN(__cdecl *TSSGCtrl_IsChildRWProhibit)(TSSGCtrl *_this, struct _TSSGSubject *SSGS) = (LPVOID)0x00511318;
BOOLEAN(__cdecl *TSSGCtrl_OneRead)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size) = (LPVOID)0x0051C4D8;
BOOLEAN(__cdecl *TSSGCtrl_OneWrite)(TSSGCtrl *_this, struct _TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size) = (LPVOID)0x0051C540;

