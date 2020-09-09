#include <windows.h>
#include "atoitbl.h"

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
				x = *src;
				if (ACTOI(&x, 'f', 16))
				{
					if (c == 'x')
					{
						c = x;
						while (++src < last)
						{
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							c = c * 0x10 + x;
						}
					}
					else
					{
						wchar_t w;

						w = x;
						while (++src < last)
						{
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
						}
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
				x = *src;
				if (WCTOI(&x, L'f', 16))
				{
					c = x;
					while (++src < last)
					{
						x = *src;
						if (!WCTOI(&x, L'f', 16))
							break;
						c = c * 0x10 + x;
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
				x = *src;
				if (ACTOI(&x, 'f', 16))
				{
					unsigned long u;

					u = x;
					while (++src < last)
					{
						x = *src;
						if (!ACTOI(&x, 'f', 16))
							break;
						u = u * 0x10 + x;
					}
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
	unsigned long       n, array;
	size_t              length, arrayLength;
	const unsigned char *p, *src;
	unsigned char       c;

	n = 0;
	array = 0;
	length = 0;
	arrayLength = 0;
	for (p = *pfirst; (src = p) < last; )
	{
		unsigned char x;

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
			n = (n << 8) + c;
			length++;
			if ((src = p) >= last)
				break;
			c = *(p++);
			goto NEXT;
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
			if (arrayLength)
			{
				length += arrayLength;
				do
				{
					n = (n << 8) + (unsigned char)array;
					array >>= 8;
				} while (--arrayLength);
			}
			n = (n << 8) + c;
			length++;
			continue;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			c -= '0';
			while (++p < last && (x = *p - '0') < '7' - '0' + 1)
				c = c * 8 + x;
			goto ARRAY;
		case 'U':
		case 'u':
		case 'x':
			if (p >= last)
				goto NEXT;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				goto NEXT;
			if (c == 'x')
			{
				c = x;
				while (++p < last)
				{
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					c = c * 0x10 + x;
				}
			}
			else
			{
				unsigned char lpMultiByteStr[2], *s;
				unsigned int  cbMultiByte;
				wchar_t       w;

				w = x;
				while (++p < last)
				{
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
				}
				if (!(cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, lpMultiByteStr, 2, NULL, NULL)))
					break;
				s = lpMultiByteStr;
				if (cbMultiByte != 1 && arrayLength < 4)
				{
					arrayLength++;
					array = (array << 8) + *(s++);
				}
				c = *s;
			}
		ARRAY:
			if (arrayLength < 4)
			{
				arrayLength++;
				array = (array << 8) + c;
			}
			continue;
		}
		break;
	}
	*pfirst = src;
	if (arrayLength)
	{
		length += arrayLength;
		do
		{
			n = (n << 8) + (unsigned char)array;
			array >>= 8;
		} while (--arrayLength);
	}
	return (length != 1 || (char)n >= 0) ? n : (~(__int64)0xFF | n);
}

unsigned long __fastcall UnescapeUnicodeCharA(const char **pfirst, const char *last)
{
	unsigned long       n;
	const unsigned char *p, *src;
	wchar_t             w;

	n = 0;
	for (p = *pfirst; (src = p) < last; n = (n << 16) + w)
	{
		unsigned char c, x;
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
			continue;
		case '0':
			w = L'\0';
			continue;
		case 'a':
			w = L'\a';
			continue;
		case 'b':
			w = L'\b';
			continue;
		case 'f':
			w = L'\f';
			continue;
		case 'n':
			w = L'\n';
			continue;
		case 'r':
			w = L'\r';
			continue;
		case 't':
			w = L'\t';
			continue;
		case 'v':
			w = L'\v';
			continue;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			w = c - '0';
			while (++p < last && (x = *p - '0') < '7' - '0' + 1)
				w = w * 8 + x;
			continue;
		case 'U':
		case 'u':
		case 'x':
			w = c;
			if (p >= last)
				continue;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			w = x;
			while (++p < last)
			{
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
			}
			continue;
		}
		break;
	}
	*pfirst = src;
	return n;
}

unsigned long __fastcall UnescapeUtf8CharA(const char **pfirst, const char *last)
{
	unsigned long       n, u, bits;
	const unsigned char *p, *src;

	n = 0;
	for (p = *pfirst; (src = p) < last; n = (n << bits) + u)
	{
		unsigned char c, x;
		unsigned int  cbMultiByte, cbUtf8;
		wchar_t       w;

		if ((c = *(p++)) != '\\')
			if (c != '\'')
				goto DEFAULT;
			else
				break;
		if (p >= last)
			break;
		bits = 8;
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
			if (bits = cbUtf8 * 8)
				continue;
			break;
		case '0':
			u = '\0';
			continue;
		case 'a':
			u = '\a';
			continue;
		case 'b':
			u = '\b';
			continue;
		case 'f':
			u = '\f';
			continue;
		case 'n':
			u = '\n';
			continue;
		case 'r':
			u = '\r';
			continue;
		case 't':
			u = '\t';
			continue;
		case 'v':
			u = '\v';
			continue;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			u = c - '0';
			while (++p < last && (x = *p - '0') < '7' - '0' + 1)
				u = u * 8 + x;
			continue;
		case 'U':
		case 'u':
		case 'x':
			u = c;
			if (p >= last)
				continue;
			if ((x = *p) == '0')
			{
				p++;
				u = 0;
				do
					if (p >= last)
						goto CONTINUE;
				while ((x = *(p++)) == '0');
				p--;
			}
			if (!ACTOI(&x, 'f', 16))
				continue;
			u = x;
			while (++p < last)
			{
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				u = u * 0x10 + x;
				bits += 8;
			}
			if (bits > 32)
				bits = 32;
			continue;
		}
		break;
	CONTINUE:;
	}
	*pfirst = src;
	return n;
}
