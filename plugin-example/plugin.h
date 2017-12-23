#ifndef PLUGIN_H
#define PLUGIN_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#define PLUGIN_CALL __cdecl

typedef enum {
	PLUGIN_PARSING = 0,
} PLUGIN_TYPE;

typedef enum {
	RETURN_VOID = 0,
	RETURN_DWORD,
	RETURN_QWORD,
	RETURN_DOUBLE,
} RETURN_TYPE;

typedef enum {
	PARAM_DWORD = 0,
	PARAM_QWORD,
	PARAM_DOUBLE,
	PARAM_STRING_A,
	PARAM_STRING_W,
	END_OF_PARAM_TYPES = -1
} PARAM_TYPE;

#pragma pack(push, 1)
#pragma warning(push)
#pragma warning(disable:4200)
typedef struct {
	LPCVOID     Address;
	RETURN_TYPE ReturnType;
	PARAM_TYPE  ParamTypes[];
} PLUGIN_FUNCTION;
#pragma warning(pop)
#pragma pack(pop)

#define PLUGIN_EXPORT(ReturnType, Function, ...) \
	EXTERN_C __declspec(dllexport) const PLUGIN_FUNCTION PluginFunction_ ## Function = { \
		(LPCVOID)Function, \
		ReturnType, \
		{ __VA_ARGS__, END_OF_PARAM_TYPES } \
	}

#pragma pack(push, 1)
typedef struct {
	struct {
		BYTE Major;
		BYTE Minor;
		BYTE Build;
		BYTE Revision;
	}              Version;
	DWORD          Reserved1;
	DWORD          Reserved2;
	DWORD          Reserved3;
	BOOL           *ParsingContinue;
	void(__stdcall *Guide)(LPCSTR Message);
	void(__stdcall *OnParsingProcess)(LPCSTR Message, unsigned __int64 Value);
	void(__stdcall *OnParsingDoubleProcess)(LPCSTR Message, double Value);
	void(__stdcall *OnProcessOpenError)();
	void(__stdcall *OnSubjectReadError)(LPCVOID Address);
	void(__stdcall *OnSubjectWriteError)(LPCVOID Address);
	void(__stdcall *OnParsingError)(LPCSTR Message);
} PLUGIN_INTERFACE;
#pragma pack(pop)

EXTERN_C __declspec(dllexport) const PLUGIN_INTERFACE *PluginInterface;

#define ParsingContinue              (*PluginInterface->ParsingContinue)
#define Guide                        PluginInterface->Guide
#define OnParsingProcess             PluginInterface->OnParsingProcess
#define OnParsingDoubleProcess       PluginInterface->OnParsingDoubleProcess
#define OnProcessOpenError()         do { PluginInterface->OnProcessOpenError();         ParsingContinue = FALSE; } while (0)
#define OnSubjectReadError(Address)  do { PluginInterface->OnSubjectReadError(Address);  ParsingContinue = FALSE; } while (0)
#define OnSubjectWriteError(Address) do { PluginInterface->OnSubjectWriteError(Address); ParsingContinue = FALSE; } while (0)
#define OnParsingError(Message)      do { PluginInterface->OnParsingError(Message);      ParsingContinue = FALSE; } while (0)

#endif	// PLUGIN_H
