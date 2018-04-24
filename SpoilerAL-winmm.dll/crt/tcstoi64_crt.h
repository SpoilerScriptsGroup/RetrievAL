#ifndef _M_IX86

TYPE
#if IS_INT64
	__msreturn
#endif
	__cdecl FUNCTION_NAME(const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef ERRNO
	return (TYPE)INTERNAL_FUNCTION(IS_UNSIGNED, IS_INT64, &ERRNO, nptr, endptr, base);
#else
	errno_t e = 0;
	TYPE r = (TYPE)INTERNAL_FUNCTION(IS_UNSIGNED, IS_INT64, &e, nptr, endptr, base);
	if (e)
		errno = e;
	return r;
#endif
}

#else

__declspec(naked) TYPE
#if IS_INT64
	__msreturn
#endif
	__cdecl FUNCTION_NAME(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
#ifdef ERRNO
		push    IS_INT64
		push    0
		push    offset ERRNO
		push    IS_UNSIGNED
		call    INTERNAL_FUNCTION
		add     esp, 16
		ret
#else
		push    IS_INT64
		push    0
		push    esp
		push    IS_UNSIGNED
		call    INTERNAL_FUNCTION
		mov     ecx, dword ptr [esp + 8]
		add     esp, 16
		test    ecx, ecx
		jz      L1
		push    eax
#if IS_INT64
		push    edx
#endif
		push    ecx
		call    _errno
		pop     ecx
#if IS_INT64
		pop     edx
#endif
		mov     dword ptr [eax], ecx
		pop     eax
	L1:
		ret
#endif
	}
}

#endif

#undef FUNCTION_NAME
#undef TYPE
#undef IS_UNSIGNED
#undef IS_INT64
