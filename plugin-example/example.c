#define _CRT_SECURE_NO_WARNINGS
#include "plugin.h"
#include <stdio.h>

//------------------------------------------------------------------------------
void PLUGIN_CALL HelloWorldA(LPCSTR s)
{
	MessageBoxA(NULL, s, "HelloWorldA", MB_TOPMOST);
}
PLUGIN_EXPORT(RETURN_VOID, HelloWorldA, PARAM_STRING_A);
//------------------------------------------------------------------------------
void PLUGIN_CALL HelloWorldW(LPCWSTR s)
{
	MessageBoxW(NULL, s, L"HelloWorldW", MB_TOPMOST);
}
PLUGIN_EXPORT(RETURN_VOID, HelloWorldW, PARAM_STRING_W);
//------------------------------------------------------------------------------
void PLUGIN_CALL HelloWorldVA(LPCSTR format, ...)
{
	char    s[512];
	va_list valist;

	va_start(valist, format);
	vsnprintf(s, _countof(s), format, valist);
	va_end(valist);
	MessageBoxA(NULL, s, "HelloWorldVA", MB_TOPMOST);
}
PLUGIN_EXPORT(RETURN_VOID, HelloWorldVA, PARAM_STRING_A);
//------------------------------------------------------------------------------
void PLUGIN_CALL HelloWorldVW(LPCWSTR format, ...)
{
	wchar_t s[512];
	va_list valist;

	va_start(valist, format);
	_vsnwprintf(s, _countof(s), format, valist);
	va_end(valist);
	MessageBoxW(NULL, s, L"HelloWorldVW", MB_TOPMOST);
}
PLUGIN_EXPORT(RETURN_VOID, HelloWorldVW, PARAM_STRING_W);
//------------------------------------------------------------------------------
DWORD PLUGIN_CALL Add32(DWORD a, DWORD b)
{
	return a + b;
}
PLUGIN_EXPORT(RETURN_DWORD, Add32, PARAM_DWORD, PARAM_DWORD);
//------------------------------------------------------------------------------
DWORD PLUGIN_CALL Sub32(DWORD a, DWORD b)
{
	return a - b;
}
PLUGIN_EXPORT(RETURN_DWORD, Sub32, PARAM_DWORD, PARAM_DWORD);
//------------------------------------------------------------------------------

