#include <windows.h>
#include "intrinsic.h"

#ifdef __BORLANDC__
#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
#endif
#define bcb6_std_string_begin(s)                                     (s)->begin()
#define bcb6_std_string_end(s)                                       (s)->end()
#define bcb6_std_vector_TSSGAttributeElement                         vector<TSSGAttributeElement *>
#define offsetof_TSSGCtrl_strD                                       offsetof(TSSGCtrl, strD)
#define offsetof_TSSGCtrl_attributeSelector                          (size_t)((TSSGCtrl *)NULL)->GetAttributeSelector()
#define TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector) attributeSelector->GetNowAtteributeVec()
#else
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#define bcb6_std_vector_TSSGAttributeElement bcb6_std_vector
#define offsetof_TSSGCtrl_strD               offsetof(TSSGCtrl, strD)
#define offsetof_TSSGCtrl_attributeSelector  offsetof(TSSGCtrl, attributeSelector)
#endif

extern HANDLE hHeap;
size_t __stdcall ReplaceDefineByHeap(bcb6_std_vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity);

#ifdef __BORLANDC__
string __cdecl SplitElement(LPVOID SSGC_strD, string *Src, string Token, DWORD Index, DWORD Option)
#else
bcb6_std_string * __cdecl SplitElement(bcb6_std_string *Result, LPVOID SSGC_strD, bcb6_std_string *Src, bcb6_std_string Token, DWORD Index, DWORD Option)
#endif
{
	LPSTR  buffer;
	LPBYTE token, p, begin, end;

#ifndef __BORLANDC__
	bcb6_std_string_dtor(&Token);
#endif
	buffer = NULL;
	p = (LPBYTE)bcb6_std_string_begin(Src);
	while (__intrinsic_isspace(*p))
		p++;
	if (*p != '_')
	{
		begin = p;
		end = bcb6_std_string_end(Src);
		while (p < end)
		{
			if (!__intrinsic_isleadbyte(*p))
			{
				if (*p != '=')
				{
					p++;
				}
				else
				{
					token = p;
					goto FOUND_TOKEN;
				}
			}
			else
			{
				p += 2;
			}
		}
	}
	else
	{
		size_t                               length, capacity;
		unsigned long                        bits;
		TSSGAttributeSelector                *attributeSelector;
		bcb6_std_vector_TSSGAttributeElement *attributes;

		length = bcb6_std_string_end(Src) - p;
		if (!length || (INT_PTR)(length + (sizeof(DWORD) - 1)) < 0)
			goto FAILED1;
#ifndef _WIN64
		_BitScanReverse(&bits, length + (sizeof(DWORD) - 1));
#else
		_BitScanReverse64(&bits, length + (sizeof(DWORD) - 1));
#endif
		capacity = (size_t)1 << (bits + 1);
		buffer = (LPSTR)HeapAlloc(hHeap, 0, capacity);
		if (!buffer)
			goto FAILED1;
		memcpy(buffer, p, length + 1);
		attributeSelector = (TSSGAttributeSelector *)((LPBYTE)SSGC_strD - (offsetof_TSSGCtrl_strD - offsetof_TSSGCtrl_attributeSelector));
		attributes = TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector);
		if (attributes)
		{
			length = ReplaceDefineByHeap(attributes, &buffer, length, capacity);
			if (!length || length == SIZE_MAX)
				goto FAILED2;
#ifndef _WIN64
			_BitScanReverse(&bits, length);
#else
			_BitScanReverse64(&bits, length);
#endif
			capacity = (size_t)1 << (bits + 1);
			if (capacity < length + sizeof(DWORD))
			{
				p = (LPBYTE)HeapReAlloc(hHeap, 0, buffer, length + sizeof(DWORD));
				if (!p)
					goto FAILED2;
				buffer = p;
			}
		}
		begin = buffer;
		end = buffer + length;
		*(LPDWORD)end = 0;
		p = begin + 1;
		while (__intrinsic_isspace(*p))
			p++;
		if (*p == 'L' && __intrinsic_isspace(*(p + 1)))
		{
			p += 2;
			while (__intrinsic_isspace(*p))
				p++;
		}
		token = NULL;
		do
		{
			BYTE c;

			c = *p;
			if (!__intrinsic_isleadbyte(c))
			{
				if (c != '$')
				{
					if (c == '=')
						token = p;
					p++;
				}
				else
				{
					if (++p >= end)
						break;
					c = *p;
					if ((char)c < '1' || c > (BYTE)'4')
						continue;
					p++;
					while (p < end)
					{
						c = *(p++);
						if (!__intrinsic_isleadbyte(c))
						{
							if (c != '$' || *p != '$')
								continue;
							p++;
							break;
						}
						else
						{
							p++;
						}
					}
				}
			}
			else
			{
				p += 2;
			}
		} while (p < end);
		if (token)
			goto FOUND_TOKEN;
	FAILED2:
		HeapFree(hHeap, 0, buffer);
	}
FAILED1:
#ifdef __BORLANDC__
	return "";
#else
	bcb6_std_string_ctor(Result);
	return Result;
#endif

FOUND_TOKEN:
	*(token++) = '\0';
#ifdef __BORLANDC__
	string Result((LPCSTR)begin);
	*Src = (LPCSTR)token;
	if (buffer)
		HeapFree(hHeap, 0, buffer);
#else
	bcb6_std_string_ctor_assign_cstr(Result, begin);
	bcb6_std_string_assign_cstr_with_length(Src, token, end - token);
	if (buffer)
		HeapFree(hHeap, 0, buffer);
#endif
	return Result;
}
