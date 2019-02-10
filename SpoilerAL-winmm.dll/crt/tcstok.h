#ifdef _MBCS
#ifndef _mbstok
static unsigned char *_mbstok_context = NULL;
#define _tcstok_context _mbstok_context
#define _tcstok_reset _mbstok_reset
#endif
#define internal_tcstok internal_mbstok
#elif defined(_UNICODE)
#define internal_tcstok internal_wcstok
#else
#ifndef strtok
static char *strtok_context = NULL;
#define _tcstok_context strtok_context
#define _tcstok_reset strtok_reset
#endif
#define internal_tcstok internal_strtok
#endif

#ifndef _M_IX86
#if !defined(_UNICODE) && (!defined(_MBCS) && !defined(strtok) && defined(_MBCS) && !defined(_mbstok))
void __cdecl _tcstok_reset()
{
	_tcstok_context = NULL;
}
#endif

#ifdef _UNICODE
#ifndef wcstok
wchar_t *__cdecl wcstok(wchar_t *string, const wchar_t *delimiter, wchar_t **context)
{
	TCHAR *__fastcall internal_wcstok(wchar_t *string, const wchar_t *delimiter, wchar_t **context);

	return internal_wcstok(string, delimiter, context);
}
#endif
#else
#if !defined(_MBCS) && !defined(strtok) || defined(_MBCS) && !defined(_mbstok)
TCHAR *__cdecl _tcstok(TCHAR *string, const TCHAR *delimiter)
{
	TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context);

	return internal_tcstok(string, delimiter, &_tcstok_context);
}
#endif
#endif

TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context)
{
	size_t n;
	TCHAR  c;
	TCHAR  *token;

	if (!string && !(string = *context))
		return NULL;
	string += _tcsspn(string, delimiter);
	n = _tcscspn(string, delimiter);
	if (!n)
		return *context = NULL;
	if (c = *(token = string + n))
	{
		*(token++) = '\0';
#ifdef _MBCS
		if (IsDBCSLeadByteEx(CP_THREAD_ACP, c) && *token)
			*(token++) = '\0';
#endif
	}
	*context = token;
	return string;
}
#else
#if !defined(_UNICODE) && (!defined(_MBCS) && !defined(strtok) && defined(_MBCS) && !defined(_mbstok))
__declspec(naked) void __cdecl _tcstok_reset()
{
	__asm
	{
		mov     dword ptr [_tcstok_context], 0
		ret
	}
}
#endif

#ifdef _UNICODE
#ifndef wcstok
__declspec(naked) wchar_t *__cdecl wcstok(wchar_t *string, const wchar_t *delimiter)
{
	wchar_t *__fastcall internal_wcstok(wchar_t *string, const wchar_t *delimiter, wchar_t **context);

	__asm
	{
		#define string    (esp + 4)
		#define delimiter (esp + 8)
		#define context   (esp + 12)

		mov     eax, dword ptr [esp]
		mov     ecx, dword ptr [context]
		mov     dword ptr [esp], ecx
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [delimiter]
		push    eax
		jmp     internal_wcstok

		#undef string
		#undef delimiter
		#undef context
	}
}
#endif
#else
#if !defined(_MBCS) && !defined(strtok) || defined(_MBCS) && !defined(_mbstok)
__declspec(naked) TCHAR *__cdecl _tcstok(TCHAR *string, const TCHAR *delimiter)
{
	TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context);

	__asm
	{
		#define string    (esp + 4)
		#define delimiter (esp + 8)

		mov     eax, dword ptr [esp]
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [delimiter]
		mov     dword ptr [esp], offset _tcstok_context
		push    eax
		jmp     internal_tcstok

		#undef string
		#undef delimiter
	}
}
#endif
#endif

__declspec(naked) TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define inc_tchar(r) add r, 2
#else
	#define tchar_ptr    byte ptr
	#define inc_tchar(r) inc r
#endif

	__asm
	{
		#define string    ecx
		#define delimiter edx
		#define context   (esp + 4)

		push    ebx
		push    esi
		push    edi
		mov     ebx, string
		mov     esi, delimiter
		mov     edi, dword ptr [context + 12]
		test    ebx, ebx
		jnz     L1
		mov     ebx, dword ptr [edi]
		test    ebx, ebx
		jz      L4
	L1:
		push    esi
		push    ebx
		call    _tcsspn
		add     esp, 8
		add     ebx, eax
		push    esi
		push    ebx
		call    _tcscspn
		add     esp, 8
		lea     esi, [eax + ebx]
		test    eax, eax
		jz      L3
#ifdef _MBCS
		mov     al, byte ptr [esi]
		and     eax, 0FFH
		jz      L2
		mov     byte ptr [esi], '\0'
		inc     esi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		jz      L2
#endif
		cmp     tchar_ptr [esi], '\0'
		je      L2
		mov     tchar_ptr [esi], '\0'
		inc_tchar(esi)
	L2:
		mov     dword ptr [edi], esi
		mov     eax, ebx
		pop     edi
		pop     esi
		pop     ebx
		ret     4

		align   16
	L3:
		mov     dword ptr [edi], 0
	L4:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret     4

		#undef string
		#undef delimiter
		#undef context
	}

	#undef tchar_ptr
	#undef inc_tchar
}
#endif
