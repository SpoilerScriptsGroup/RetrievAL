#pragma once

#include <windows.h>
#include "TSSGSubject.h"
#include "bcb6_std_string.h"

EXTERN_C void __stdcall TSSGActionListner_OnParsingProcess61(LPVOID this, TSSGSubject *SSGS, LPCSTR Code, long TopVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingDoubleProcess61(LPVOID this, TSSGSubject *SSGS, LPCSTR Code, double TopVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingProcess(LPCSTR code, size_t codeLength, unsigned __int64 topVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingError(LPVOID this, TSSGSubject *SSGS, LPCSTR Code);
EXTERN_C void __stdcall TSSGActionListner_OnParsingDoubleProcess(LPCSTR code, size_t codeLength, double topVal);

EXTERN_C void(__cdecl *TSSGActionListner_OnProcessOpen)(LPVOID this, TSSGSubject *SSGS, DWORD Mode);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectDisabled)(LPVOID this, TSSGSubject *SSGS, bcb6_std_string Code);
EXTERN_C void(__cdecl *TSSGActionListner_OnProcessOpenError)(LPVOID this, TSSGSubject *SSGS);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectReadError)(LPVOID this, TSSGSubject *SSGS, unsigned long Address);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectWriteError)(LPVOID this, TSSGSubject *SSGS, unsigned long Address);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectReadSuccess)(LPVOID this, TSSGSubject *SSGS, unsigned long Address);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectWriteSuccess)(LPVOID this, TSSGSubject *SSGS, unsigned long Address);

#define TSSGActionListner_OnParsingProcess61(this, SSGS, Code, TopVal)       TSSGActionListner_OnParsingProcess61(this, NULL, Code, TopVal)
#define TSSGActionListner_OnParsingDoubleProcess61(this, SSGS, Code, TopVal) TSSGActionListner_OnParsingDoubleProcess61(this, NULL, Code, TopVal)
#define TSSGActionListner_OnParsingError(this, SSGS, Code)                   TSSGActionListner_OnParsingError(this, NULL, Code)
#define TSSGActionListner_OnProcessOpen(this, SSGS, Mode)                    TSSGActionListner_OnProcessOpen(this, NULL, Mode)
#define TSSGActionListner_OnSubjectDisabled(this, SSGS, Code)                TSSGActionListner_OnSubjectDisabled(this, NULL, Code)
#define TSSGActionListner_OnProcessOpenError(this, SSGS)                     TSSGActionListner_OnProcessOpenError(this, NULL)
#define TSSGActionListner_OnSubjectReadError(this, SSGS, Address)            TSSGActionListner_OnSubjectReadError(this, NULL, Address)
#define TSSGActionListner_OnSubjectWriteSuccess(this, SSGS, Address)         TSSGActionListner_OnSubjectWriteSuccess(this, NULL, Address)
