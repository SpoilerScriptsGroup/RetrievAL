#include <windows.h>
#include "atoitbl.h"
#include "intrinsic.h"

#if (!defined(_MSC_VER) || _MSC_VER < 1200) && !defined(__assume)
#define __assume(expression)
#endif

#ifdef _WIN64
char * __fastcall UnescapeA(char *first, char **plast, BOOL breakSingleQuate)
{
	unsigned char *p, *last;

	if ((p = first) < (last = *plast))
#else
unsigned __int64 __fastcall __reg64return_UnescapeA(char *first, char *last, BOOL breakSingleQuate)
{
	unsigned char *p;

	if ((p = first) < last)
#endif
	{
		for (; ; )
		{
			unsigned char c, *src, x;
			size_t        size;

			if ((c = *(p++)) != '\\')
			{
				if (c != '"' && (c != '\'' || !breakSingleQuate))
					if (p < last && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c) || ++p < last))
						continue;
					else
						break;
				p--;
				break;
			}
			if ((src = p--) >= last)
				break;
			switch (c = *(src++))
			{
			default:
				if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c) || src >= last)
					break;
				*(p++) = c;
				c = *(src++);
				break;
			case '0':
				c = '\0';
				break;
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 'f':
				c = '\f';
				break;
			case 'n':
				c = '\n';
				break;
			case 'r':
				c = '\r';
				break;
			case 't':
				c = '\t';
				break;
			case 'v':
				c = '\v';
				break;
			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				c -= '0';
				while (++src < last && (x = *src - '0') < '7' - '0' + 1)
					c = c * 8 + x;
				break;
			case 'U':
			case 'u':
			case 'x':
				if (src >= last)
					goto DONE;
				if (ACTOX(&x, *src))
				{
					if (c == 'x')
					{
						c = x;
						while (++src < last && ACTOX(&x, *src))
							c = (c << 4) | x;
					}
					else
					{
						wchar_t w;

						w = x;
						while (++src < last && ACTOX(&x, *src))
							w = (w << 4) | x;
						p += (unsigned int)WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, p, 2, NULL, NULL);
						goto SHRINK;
					}
				}
				else
				{
					p = src;
					continue;
				}
				break;
			}
			*(p++) = c;
		SHRINK:
			if (!(size = (last -= src - p) - p))
				break;
			memcpy(p, src, size);
		}
	}
DONE:
#ifdef _WIN64
	*plast = last;
	return p;
#else
	return (unsigned __int32)p | ((unsigned __int64)last << 32);
#endif
}

#ifdef _WIN64
wchar_t * __fastcall UnescapeW(wchar_t *first, wchar_t **plast, BOOL breakSingleQuate)
{
	wchar_t *p, *last;

	if ((p = first) < (last = *plast))
#else
unsigned __int64 __fastcall __reg64return_UnescapeW(wchar_t *first, wchar_t *last, BOOL breakSingleQuate)
{
	wchar_t *p;

	if ((p = first) < last)
#endif
	{
		for (; ; )
		{
			wchar_t c, *src, x;
			size_t  size;

			if ((c = *(p++)) != L'\\')
			{
				if (c != L'"' && (c != L'\'' || !breakSingleQuate))
					if (p < last)
						continue;
					else
						break;
				p--;
				break;
			}
			if ((src = p--) >= last)
				break;
			switch (c = *(src++))
			{
			case L'0':
				c = L'\0';
				break;
			case L'a':
				c = L'\a';
				break;
			case L'b':
				c = L'\b';
				break;
			case L'f':
				c = L'\f';
				break;
			case L'n':
				c = L'\n';
				break;
			case L'r':
				c = L'\r';
				break;
			case L't':
				c = L'\t';
				break;
			case L'v':
				c = L'\v';
				break;
			case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7':
				c -= L'0';
				while (++src < last && (x = *src - L'0') < L'7' - L'0' + 1)
					c = c * 8 + x;
				break;
			case L'U':
			case L'u':
			case L'x':
				if (src >= last)
					goto DONE;
				if (WCTOX(&c, *src))
				{
					while (++src < last && WCTOX(&x, *src))
						c = (c << 4) | x;
				}
				else
				{
					p = src;
					continue;
				}
				break;
			}
			*(p++) = c;
			if (!(size = ((char *)last -= (char *)src - (char *)p) - (char *)p))
				break;
			memcpy(p, src, size);
		}
	}
DONE:
#ifdef _WIN64
	*plast = last;
	return p;
#else
	return (unsigned __int32)p | ((unsigned __int64)last << 32);
#endif
}

#ifdef _WIN64
unsigned char * __fastcall UnescapeU(unsigned char *first, unsigned char **plast, BOOL breakSingleQuate)
{
	unsigned char *p, *last;

	if ((p = first) < (last = *plast))
#else
unsigned __int64 __fastcall __reg64return_UnescapeU(unsigned char *first, unsigned char *last, BOOL breakSingleQuate)
{
	unsigned char *p;

	if ((p = first) < last)
#endif
	{
		for (; ; )
		{
			unsigned char c, *src, x;
			size_t        size;

			if ((c = *(p++)) != '\\')
			{
				if (c != '"' && (c != '\'' || !breakSingleQuate))
					if (p < last)
						continue;
					else
						break;
				p--;
				break;
			}
			if ((src = p--) >= last)
				break;
			switch (c = *(src++))
			{
			case '0':
				c = '\0';
				break;
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 'f':
				c = '\f';
				break;
			case 'n':
				c = '\n';
				break;
			case 'r':
				c = '\r';
				break;
			case 't':
				c = '\t';
				break;
			case 'v':
				c = '\v';
				break;
			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				c -= '0';
				while (++src < last && (x = *src - '0') < '7' - '0' + 1)
					c = c * 8 + x;
				break;
			case 'U':
			case 'u':
			case 'x':
				if (src >= last)
					goto DONE;
				if (ACTOX(&x, *src))
				{
					unsigned long u;

					u = x;
					while (++src < last && ACTOX(&x, *src))
						u = (u << 4) | x;
					do
						*(p++) = (unsigned char)u;
					while (u >>= 8);
					goto SHRINK;
				}
				else
				{
					p = src;
					continue;
				}
				break;
			}
			*(p++) = c;
		SHRINK:
			if (!(size = (last -= src - p) - p))
				break;
			memcpy(p, src, size);
		}
	}
DONE:
#ifdef _WIN64
	*plast = last;
	return p;
#else
	return (unsigned __int32)p | ((unsigned __int64)last << 32);
#endif
}

__int64 __fastcall UnescapeAnsiCharA(const char **pfirst, const char *last)
{
	unsigned long       n, stack;
	size_t              length, stackBits;
	const unsigned char *p, *src;

	n = 0;
	length = 0;
	stack = 0;
	stackBits = 0;
	for (p = *pfirst; (src = p) < last; )
	{
		unsigned char c, x;

		if ((c = *(p++)) != '\\')
			if (c != '\'')
				goto DEFAULT;
			else
				break;
		if (p >= last)
			break;
		switch (c = *(p++))
		{
		default:
		DEFAULT:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				goto NEXT;
			n = (n << 8) | c;
			length++;
			if ((src = p) >= last)
				break;
			c = *(p++);
			goto NEXT;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			c -= '0';
			while (++p < last && (x = *p - '0') < '7' - '0' + 1)
				c = c * 8 + x;
			goto PUSH;
		case 'U':
		case 'u':
		case 'x':
			if (p >= last)
				goto NEXT;
			if (!ACTOX(&x, *p))
				goto NEXT;
			if (c == 'x')
			{
				c = x;
				while (++p < last && ACTOX(&x, *p))
					c = (c << 4) | x;
			}
			else
			{
				unsigned char lpMultiByteStr[2], *s;
				unsigned int  cbMultiByte;
				wchar_t       w;

				w = x;
				while (++p < last && ACTOX(&x, *p))
					w = (w << 4) | x;
				if (!(cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, lpMultiByteStr, 2, NULL, NULL)))
					break;
				s = lpMultiByteStr;
				if (cbMultiByte != 1)
				{
					if (stackBits >= 32)
						continue;
					stackBits += 8;
					stack = (unsigned long)__ll_rshift(stack | ((unsigned __int64)*(s++) << 32), 8);
				}
				c = *s;
			}
		PUSH:
			if (stackBits >= 32)
				continue;
			stackBits += 8;
			stack = (unsigned long)__ll_rshift(stack | ((unsigned __int64)c << 32), 8);
			continue;
		case '0':
			c = '\0';
			goto NEXT;
		case 'a':
			c = '\a';
			goto NEXT;
		case 'b':
			c = '\b';
			goto NEXT;
		case 'f':
			c = '\f';
			goto NEXT;
		case 'n':
			c = '\n';
			goto NEXT;
		case 'r':
			c = '\r';
			goto NEXT;
		case 't':
			c = '\t';
			goto NEXT;
		case 'v':
			c = '\v';
		NEXT:
			if (stackBits)
			{
				n = stackBits < 32 ? (unsigned long)(__ll_lshift(((unsigned __int64)n << 32) | stack, stackBits) >> 32) : stack;
				length += stackBits / 8;
				stackBits = 0;
			}
			n = (n << 8) | c;
			length++;
			continue;
		}
		break;
	}
	*pfirst = src;
	if (stackBits)
	{
		n = stackBits < 32 ? (unsigned long)(__ll_lshift(((unsigned __int64)n << 32) | stack, stackBits) >> 32) : stack;
		length += stackBits / 8;
	}
	return (length != 1 || (char)n >= 0) ? n : (~(__int64)0xFF | n);
}

unsigned long __fastcall UnescapeUnicodeCharA(const char **pfirst, const char *last)
{
	unsigned long       n, stack;
	size_t              stackBits;
	const unsigned char *p, *src;

	n = 0;
	stack = 0;
	stackBits = 0;
	for (p = *pfirst; (src = p) < last; )
	{
		unsigned char c;
		wchar_t       w;
		unsigned int  cbMultiByte;

		if ((c = *(p++)) != '\\')
			if (c != '\'')
				goto DEFAULT;
			else
				break;
		if (p >= last)
			break;
		switch (c = *(p++))
		{
		default:
		DEFAULT:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cbMultiByte = 1;
			else if (p < last)
				cbMultiByte = 2;
			else
				break;
			if (!MultiByteToWideChar(CP_THREAD_ACP, 0, --p, cbMultiByte, &w, 1))
				break;
			p += cbMultiByte;
			goto NEXT;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			w = c - '0';
			while (++p < last && (c = *p - '0') < '7' - '0' + 1)
				w = w * 8 + c;
			goto PUSH;
		case 'U':
		case 'u':
		case 'x':
			w = c;
			if (p >= last || !ACTOX(&c, *p))
				goto NEXT;
			w = c;
			while (++p < last && ACTOX(&c, *p))
				w = (w << 4) | c;
		PUSH:
			if (stackBits >= 32)
				continue;
			stackBits += 16;
			stack = (unsigned long)__ll_rshift(stack | ((unsigned __int64)w << 32), 16);
			continue;
		case '0':
			w = L'\0';
			goto NEXT;
		case 'a':
			w = L'\a';
			goto NEXT;
		case 'b':
			w = L'\b';
			goto NEXT;
		case 'f':
			w = L'\f';
			goto NEXT;
		case 'n':
			w = L'\n';
			goto NEXT;
		case 'r':
			w = L'\r';
			goto NEXT;
		case 't':
			w = L'\t';
			goto NEXT;
		case 'v':
			w = L'\v';
		NEXT:
			if (stackBits)
			{
				n = stackBits < 32 ? (unsigned long)(__ll_lshift(((unsigned __int64)n << 32) | stack, stackBits) >> 32) : stack;
				stackBits = 0;
			}
			n = (n << 16) + w;
			continue;
		}
		break;
	}
	*pfirst = src;
	if (stackBits)
		n = stackBits < 32 ? (unsigned long)(__ll_lshift(((unsigned __int64)n << 32) | stack, stackBits) >> 32) : stack;
	return n;
}

unsigned long __fastcall UnescapeUtf8CharA(const char **pfirst, const char *last)
{
	unsigned long       n, stack;
	size_t              stackBits;
	const unsigned char *p, *src;

	n = 0;
	stack = 0;
	stackBits = 0;
	for (p = *pfirst; (src = p) < last; )
	{
		unsigned char c;
		unsigned long u;
		unsigned int  cbMultiByte, cbUtf8;
		wchar_t       w;
		size_t        bits;

		if ((c = *(p++)) != '\\')
			if (c != '\'')
				goto DEFAULT;
			else
				break;
		if (p >= last)
			break;
		switch (c = *(p++))
		{
		default:
		DEFAULT:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cbMultiByte = 1;
			else if (p < last)
				cbMultiByte = 2;
			else
				break;
			if (!MultiByteToWideChar(CP_THREAD_ACP, 0, --p, cbMultiByte, &w, 1))
				break;
			p += cbMultiByte;
			cbUtf8 = WideCharToMultiByte(CP_UTF8, 0, &w, 1, (char *)&u, sizeof(u), NULL, NULL);
			if (stackBits)
			{
				n = stackBits < 32 ? (unsigned long)(__ll_lshift(((unsigned __int64)n << 32) | stack, stackBits) >> 32) : stack;
				stackBits = 0;
			}
			switch (cbUtf8)
			{
			case 0:
				break;
			case 1:
				n = (n << 8) | (u & 0xFF);
				continue;
			case 2:
				n = (n << 16) | _byteswap_ulong(u << 16);
				continue;
			case 3:
				n = (n << 24) | _byteswap_ulong(u << 8);
				continue;
			case 4:
				n = _byteswap_ulong(u);
				continue;
			default:
				__assume(0);
			}
			break;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			u = c - '0';
			while (++p < last && (c = *p - '0') < '7' - '0' + 1)
				u = u * 8 + c;
			goto PUSH;
		case 'U':
		case 'u':
		case 'x':
			u = c;
			if (p >= last || !ACTOX(&c, *p))
				goto NEXT;
			u = c;
			while (++p < last && ACTOX(&c, *p))
				u = (u << 4) | c;
		PUSH:
			if (stackBits >= 32)
				continue;
			if (u < 0x00010000)
				if (u < 0x00000100)
					bits = 8;
				else
					bits = 16;
			else
				if (u < 0x01000000)
					bits = 24;
				else
					bits = 32;
			if ((stackBits += bits) > 32)
			{
				bits -= stackBits - 32;
				stackBits = 32;
			}
			stack = bits < 32 ? (unsigned long)__ll_rshift(stack | ((unsigned __int64)u << 32), bits) : u;
			continue;
		case '0':
			u = '\0';
			goto NEXT;
		case 'a':
			u = '\a';
			goto NEXT;
		case 'b':
			u = '\b';
			goto NEXT;
		case 'f':
			u = '\f';
			goto NEXT;
		case 'n':
			u = '\n';
			goto NEXT;
		case 'r':
			u = '\r';
			goto NEXT;
		case 't':
			u = '\t';
			goto NEXT;
		case 'v':
			u = '\v';
		NEXT:
			if (stackBits)
			{
				n = stackBits < 32 ? (unsigned long)(__ll_lshift(((unsigned __int64)n << 32) | stack, stackBits) >> 32) : stack;
				stackBits = 0;
			}
			n = (n << 8) | u;
			continue;
		}
		break;
	}
	*pfirst = src;
	if (stackBits)
		n = stackBits < 32 ? (unsigned long)(__ll_lshift(((unsigned __int64)n << 32) | stack, stackBits) >> 32) : stack;
	return n;
}
