#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

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

typedef struct {
	LPCSTR      Name;
	size_t      NameLength;
	LPCVOID     Address;
	RETURN_TYPE ReturnType;
	size_t      StackSize;
	PARAM_TYPE  *ParamTypes;
	PARAM_TYPE  *EndOfParamTypes;
} PLUGIN_FUNCTION;

typedef struct {
	PLUGIN_FUNCTION *First;
	PLUGIN_FUNCTION *Last;
} PLUGIN_FUNCTION_VECTOR;

extern PLUGIN_FUNCTION        *PluginFunctions;
extern PLUGIN_FUNCTION        *EndOfPluginFunctions;
extern PLUGIN_FUNCTION_VECTOR PluginFunctionVector[256];

BOOL __cdecl PluginInitialize(const char DirectoryPath[MAX_PATH], const char ProfileName[MAX_PATH]);
void __cdecl PluginFinalize();
unsigned __int64 __cdecl CallPluginFunction(const void *Address, const void *Stack, size_t StackSize);

#ifdef __cplusplus
}
#endif

#endif	// _PLUGIN_H_
