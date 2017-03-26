#pragma once

#include <windows.h>
#include "TSSGSubject.h"
#include "bcb6_std_string.h"

EXTERN_C void __stdcall TSSGActionListner_OnParsingProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, long TopVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingDoubleProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, double TopVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingProcess(LPCSTR code, size_t codeLength, unsigned __int64 topVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingError(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code);
EXTERN_C void __stdcall TSSGActionListner_OnParsingDoubleProcess(LPCSTR code, size_t codeLength, double topVal);

typedef void(__cdecl *LPFN_TSSGACTIONLISTNER_ONPROCESSOPEN)(LPVOID ssgActionListner, TSSGSubject *SSGS, DWORD Mode);
#define TSSGActionListner_OnProcessOpen ((LPFN_TSSGACTIONLISTNER_ONPROCESSOPEN)0x0052EB20)

typedef void(__cdecl *LPFN_TSSGACTIONLISTNER_ONSUBJECTDISABLED)(LPVOID ssgActionListner, bcb6_std_string Code);
#define TSSGActionListner_OnSubjectDisabled ((LPFN_TSSGACTIONLISTNER_ONSUBJECTDISABLED)0x0052EBC4)

typedef void(__cdecl *LPFN_TSSGACTIONLISTNER_ONPROCESSOPENERROR)(LPVOID ssgActionListner, TSSGSubject *SSGS);
#define TSSGActionListner_OnProcessOpenError ((LPFN_TSSGACTIONLISTNER_ONPROCESSOPENERROR)0x0052EE78)

typedef void(__cdecl *LPFN_TSSGACTIONLISTNER_ONSUBJECTREADERROR)(LPVOID ssgActionListner, TSSGSubject *SSGS, unsigned long Address);
#define TSSGActionListner_OnSubjectReadError ((LPFN_TSSGACTIONLISTNER_ONSUBJECTREADERROR)0x0052EF8C)

typedef void(__cdecl *LPFN_TSSGACTIONLISTNER_ONSUBJECTWRITEERROR)(LPVOID ssgActionListner, TSSGSubject *SSGS, unsigned long Address);
#define TSSGActionListner_OnSubjectWriteError ((LPFN_TSSGACTIONLISTNER_ONSUBJECTWRITEERROR)0x0052F13C)

typedef void(__cdecl *LPFN_TSSGACTIONLISTNER_ONSUBJECTREADSUCCESS)(LPVOID ssgActionListner, TSSGSubject *SSGS, unsigned long Address);
#define TSSGActionListner_OnSubjectReadSuccess ((LPFN_TSSGACTIONLISTNER_ONSUBJECTREADSUCCESS)0x0052F31C)

typedef void(__cdecl *LPFN_TSSGACTIONLISTNER_ONSUBJECTWRITESUCCESS)(LPVOID ssgActionListner, TSSGSubject *SSGS, unsigned long Address);
#define TSSGActionListner_OnSubjectWriteSuccess ((LPFN_TSSGACTIONLISTNER_ONSUBJECTWRITESUCCESS)0x0052F6D0)
