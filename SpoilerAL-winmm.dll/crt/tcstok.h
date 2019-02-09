#ifdef _MBCS
#ifndef _mbstok
static unsigned char *_mbstok_context = NULL;
#define _tcstok_context _mbstok_context
#define _tcstok_reset _mbstok_reset
#endif
#define internal_tcstok internal_mbstok
#elif !defined(_UNICODE)
#ifndef strtok
static char *strtok_context = NULL;
#define _tcstok_context strtok_context
#define _tcstok_reset strtok_reset
#endif
#define internal_tcstok internal_strtok
#endif

#ifndef _M_IX86
#if !defined(_UNICODE) && (defined(_MBCS) && !defined(_mbstok) || !defined(_MBCS) && !defined(strtok))
void __cdecl _tcstok_reset()
{
	_tcstok_context = NULL;
}

TCHAR *__cdecl _tcstok(TCHAR *string, const TCHAR *delimiter)
{
	TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context);

	return internal_tcstok(string, delimiter, &_tcstok_context);
}
#endif

#ifdef _UNICODE
wchar_t *__cdecl wcstok(wchar_t *string, const wchar_t *delimiter, wchar_t **context)
#else
TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context)
#endif
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
#ifdef _MBCS
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
#endif
			*(token++) = '\0';
#ifdef _MBCS
		else
			*(((unsigned short *)token)++) = '\0\0';
#endif
	*context = token;
	return string;

#ifdef _UNICODE
	#undef _tcstok_context
#endif
}
#else
#if !defined(_UNICODE) && (defined(_MBCS) && !defined(_mbstok) || !defined(_MBCS) && !defined(strtok))
__declspec(naked) void __cdecl _tcstok_reset()
{
	__asm
	{
		mov     dword ptr [_tcstok_context], 0
		ret
	}
}

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

#ifdef _UNICODE
__declspec(naked) wchar_t *__cdecl wcstok(wchar_t *string, const wchar_t *delimiter, wchar_t **context)
#else
__declspec(naked) TCHAR *__fastcall internal_tcstok(TCHAR *string, const TCHAR *delimiter, TCHAR **context)
#endif
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
#ifdef _UNICODE
		#define string    (esp + 4)
		#define delimiter (esp + 8)
		#define context   (esp + 12)
#else
		#define string    ecx
		#define delimiter edx
		#define context   (esp + 4)
#endif

		push    ebx
		push    esi
		push    edi
#ifdef _UNICODE
		mov     ebx, dword ptr [string + 12]
		mov     esi, dword ptr [delimiter + 12]
#else
		mov     ebx, string
		mov     esi, delimiter
#endif
		mov     edi, dword ptr [context + 12]
		test    ebx, ebx
		jnz     L1
		mov     ebx, dword ptr [edi]
		test    ebx, ebx
		jz      L5
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
		jz      L4
#ifndef _MBCS
		cmp     tchar_ptr [esi], '\0'
		je      L3
		mov     tchar_ptr [esi], '\0'
		inc_tchar(esi)
#else
		mov     al, byte ptr [esi]
		and     eax, 0FFH
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		jnz     L2
		mov     byte ptr [esi], '\0'
		inc     esi
		jmp     L3
	L2:
		mov     word ptr [esi], '\0\0'
		add     esi, 2
#endif
	L3:
		mov     dword ptr [edi], esi
		mov     eax, ebx
		pop     edi
		pop     esi
		pop     ebx
#ifdef _UNICODE
		ret
#else
		ret     4
#endif

		align   16
	L4:
		mov     dword ptr [edi], 0
	L5:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
#ifdef _UNICODE
		ret
#else
		ret     4
#endif

		#undef string
		#undef delimiter
		#undef context
	}

	#undef tchar_ptr
	#undef inc_tchar
}
#endif
