#include <windows.h>
#include "atoitbl.h"

char * __fastcall UnescapeA(char *first, char **plast, BOOL breakSingleQuate)
{
	char *p, *last;

	if ((p = first) < (last = *plast))
	{
		for (; ; )
		{
			char   c, *src, x;
			size_t size;

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
			{
				last = p;
				break;
			}
			switch (c = *(src++))
			{
			case '0':
				*(p++) = '\0';
				break;
			case 'a':
				*(p++) = '\a';
				break;
			case 'b':
				*(p++) = '\b';
				break;
			case 'f':
				*(p++) = '\f';
				break;
			case 'n':
				*(p++) = '\n';
				break;
			case 'r':
				*(p++) = '\r';
				break;
			case 't':
				*(p++) = '\t';
				break;
			case 'v':
				*(p++) = '\v';
				break;
			case 'U':
			case 'u':
				if (src < last)
				{
					x = *src;
					if (ACTOI(&x, 'f', 16))
					{
						wchar_t w;

						w = x;
						do	/* do { ... } while (0); */
						{
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
							if ((src++)[-4] == 'u' || src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							w = w * 0x10 + x;
							src++;
						} while (0);
						p += (unsigned int)WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, p, 2, NULL, NULL);
						break;
					}
				}
				*(p++) = c;
				break;
			case 'x':
				if (src < last)
				{
					x = *src;
					if (ACTOI(&x, 'f', 16))
					{
						c = x;
						if (++src < last)
						{
							x = *src;
							if (ACTOI(&x, 'f', 16))
							{
								c = c * 0x10 + x;
								src++;
							}
						}
					}
				}
				*(p++) = c;
				break;
			default:
				*(p++) = c;
				if (src < last && IsDBCSLeadByteEx(CP_THREAD_ACP, c))
					*(p++) = *(src++);
				break;
			}
			if (!(size = (last -= src - p) - p))
				break;
			memcpy(p, src, size);
		}
		*plast = last;
	}
	return p;
}

wchar_t * __fastcall UnescapeW(wchar_t *first, wchar_t **plast, BOOL breakSingleQuate)
{
	wchar_t *p, *last;

	if ((p = first) < (last = *plast))
	{
		for (; ; )
		{
			wchar_t c, *src;
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
			{
				last = p;
				break;
			}
			switch (c = *(src++))
			{
			case L'0':
				*(p++) = L'\0';
				break;
			case L'a':
				*(p++) = L'\a';
				break;
			case L'b':
				*(p++) = L'\b';
				break;
			case L'f':
				*(p++) = L'\f';
				break;
			case L'n':
				*(p++) = L'\n';
				break;
			case L'r':
				*(p++) = L'\r';
				break;
			case L't':
				*(p++) = L'\t';
				break;
			case L'v':
				*(p++) = L'\v';
				break;
			case L'U':
			case L'u':
			case L'x':
				do	/* do { ... } while (0); */
				{
					wchar_t x;

					if (src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = x;
					if (++src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = c * 0x10 + x;
					if (++src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = c * 0x10 + x;
					if (++src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = c * 0x10 + x;
					if ((src++)[-4] != L'U' || src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = c * 0x10 + x;
					if (++src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = c * 0x10 + x;
					if (++src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = c * 0x10 + x;
					if (++src >= last)
						break;
					x = *src;
					if (!WCTOI(&x, L'f', 16))
						break;
					c = c * 0x10 + x;
					src++;
				} while (0);
			default:
				*(p++) = c;
				break;
			}
			if (!(size = ((char *)last -= (char *)src - (char *)p) - (char *)p))
				break;
			memcpy(p, src, size);
		}
		*plast = last;
	}
	return p;
}

unsigned char * __fastcall UnescapeU(unsigned char *first, unsigned char **plast, BOOL breakSingleQuate)
{
	unsigned char *p, *last;

	if ((p = first) < (last = *plast))
	{
		for (; ; )
		{
			unsigned char c, *src;
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
			{
				last = p;
				break;
			}
			switch (c = *(src++))
			{
			case '0':
				*(p++) = '\0';
				break;
			case 'a':
				*(p++) = '\a';
				break;
			case 'b':
				*(p++) = '\b';
				break;
			case 'f':
				*(p++) = '\f';
				break;
			case 'n':
				*(p++) = '\n';
				break;
			case 'r':
				*(p++) = '\r';
				break;
			case 't':
				*(p++) = '\t';
				break;
			case 'v':
				*(p++) = '\v';
				break;
			case 'U':
			case 'u':
			case 'x':
				if (src < last)
				{
					unsigned char x;

					x = *src;
					if (ACTOI(&x, 'f', 16))
					{
						unsigned long u;

						u = x;
						do	/* do { ... } while (0); */
						{
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
							if ((src++)[-4] == 'u' || src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
							if (++src >= last)
								break;
							x = *src;
							if (!ACTOI(&x, 'f', 16))
								break;
							u = u * 0x10 + x;
							src++;
						} while (0);
						do
							*(p++) = (unsigned char)u;
						while (u >>= 8);
						break;
					}
				}
			default:
				*(p++) = c;
				break;
			}
			if (!(size = (last -= src - p) - p))
				break;
			memcpy(p, src, size);
		}
		*plast = last;
	}
	return p;
}

unsigned long __fastcall UnescapeAnsiCharA(const char **pfirst, const char *last)
{
	unsigned long n;
	size_t        length;
	const char    *p;
	char          c;

	n = 0;
	length = 0;
	for (p = *pfirst; p < last && (c = *(p++)) != '\''; n = n * 0x100 + c, length++)
	{
		char          x, lpMultiByteStr[2], *src;
		wchar_t       w;
		unsigned int  cbMultiByte;

		if (c != '\\')
		{
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				continue;
			n = n * 0x100 + c;
			length++;
			if (p >= last)
				break;
			p++;
			continue;
		}
		if (p >= last)
		{
			p--;
			break;
		}
		switch (c = *(p++))
		{
		case '0':
			c = '\0';
			continue;
		case 'a':
			c = '\a';
			continue;
		case 'b':
			c = '\b';
			continue;
		case 'f':
			c = '\f';
			continue;
		case 'n':
			c = '\n';
			continue;
		case 'r':
			c = '\r';
			continue;
		case 't':
			c = '\t';
			continue;
		case 'U':
		case 'u':
			if (p >= last)
				break;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			w = x;
			do	/* do { ... } while (0); */
			{
				if (++p >= last)
					break;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
				if (++p >= last)
					break;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
				if (++p >= last)
					break;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
				if ((p++)[-4] == 'u' || p >= last)
					break;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
				if (++p >= last)
					break;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
				if (++p >= last)
					break;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
				if (++p >= last)
					break;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					break;
				w = w * 0x10 + x;
				p++;
			} while (0);
			cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, lpMultiByteStr, 2, NULL, NULL);
			if (!cbMultiByte)
				break;
			src = lpMultiByteStr;
			if (--cbMultiByte)
			{
				n = n * 0x100 + *(src++);
				length++;
			}
			c = *src;
			continue;
		case 'v':
			c = '\v';
			continue;
		case 'x':
			if (p >= last)
				break;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			c = x;
			if (++p >= last)
				continue;
			x = *p;
			if (!ACTOI(&x, 'f', 16))
				continue;
			c = c * 0x10 + x;
			p++;
			continue;
		default:
			continue;
		}
		break;
	}
	if (length == 1)
		n = (char)n;
	*pfirst = p;
	return n;
}

unsigned long __fastcall UnescapeUnicodeCharA(const char **pfirst, const char *last)
{
	unsigned long n;
	const char    *p;
	char          c;
	wchar_t       w;

	n = 0;
	for (p = *pfirst; p < last && (c = *(p++)) != '\''; n = n * 0x10000 + w)
	{
		char         x;
		unsigned int cchWideChar;

		if (c != '\\')
		{
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p - 1, 1, &w, 1);
			else if (p < last)
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p++ - 1, 2, &w, 1);
			else
				break;
			if (cchWideChar)
				continue;
			else
				break;
		}
		if (p >= last)
		{
			p--;
			break;
		}
		switch (c = *(p++))
		{
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
		case 'U':
		case 'u':
		case 'x':
			if (p >= last)
				break;
			x = *p;
			if (ACTOI(&x, 'f', 16))
			{
				w = x;
				if (++p >= last)
					continue;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					continue;
				w = w * 0x10 + x;
				if (++p >= last)
					continue;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					continue;
				w = w * 0x10 + x;
				if (++p >= last)
					continue;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					continue;
				w = w * 0x10 + x;
				if ((p++)[-4] != 'U' || p >= last)
					continue;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					continue;
				w = w * 0x10 + x;
				if (++p >= last)
					continue;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					continue;
				w = w * 0x10 + x;
				if (++p >= last)
					continue;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					continue;
				w = w * 0x10 + x;
				if (++p >= last)
					continue;
				x = *p;
				if (!ACTOI(&x, 'f', 16))
					continue;
				w = w * 0x10 + x;
				p++;
				continue;
			}
		default:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p - 1, 1, &w, 1);
			else if (p < last)
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p++ - 1, 2, &w, 1);
			else
				break;
			if (cchWideChar)
				continue;
			else
				break;
		}
		break;
	}
	*pfirst = p;
	return n;
}

unsigned long __fastcall UnescapeUtf8CharA(const char **pfirst, const char *last)
{
	unsigned long n, u, shift;
	const char    *p;
	char          c;

	n = 0;
	for (p = *pfirst; p < last && (c = *(p++)) != '\''; n = (n << shift) + u)
	{
		char         x;
		unsigned int cchWideChar, cbUtf8;
		wchar_t      w;

		if (c != '\\')
		{
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p - 1, 1, &w, 1);
			else if (p < last)
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p++ - 1, 2, &w, 1);
			else
				break;
			if (!cchWideChar)
				break;
			u = 0;
			cbUtf8 = WideCharToMultiByte(CP_UTF8, 0, &w, 1, (LPSTR)&u, sizeof(u), NULL, NULL);
			if (shift = cbUtf8 * 8)
				continue;
			else
				break;
		}
		if (p >= last)
		{
			p--;
			break;
		}
		switch (c = *(p++))
		{
		case '0':
			u = '\0';
			shift = 8;
			continue;
		case 'a':
			u = '\a';
			shift = 8;
			continue;
		case 'b':
			u = '\b';
			shift = 8;
			continue;
		case 'f':
			u = '\f';
			shift = 8;
			continue;
		case 'n':
			u = '\n';
			shift = 8;
			continue;
		case 'r':
			u = '\r';
			shift = 8;
			continue;
		case 't':
			u = '\t';
			shift = 8;
			continue;
		case 'v':
			u = '\v';
			shift = 8;
			continue;
		case 'U':
		case 'u':
		case 'x':
			if (p >= last)
				break;
			x = *p;
			if (ACTOI(&x, 'f', 16))
			{
				u = x;
				do
				{
					if (++p >= last)
						break;
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					u = u * 0x10 + x;
					if (++p >= last)
						break;
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					u = u * 0x10 + x;
					if (++p >= last)
						break;
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					u = u * 0x10 + x;
					if ((p++)[-4] == 'u' || p >= last)
						break;
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					u = u * 0x10 + x;
					if (++p >= last)
						break;
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					u = u * 0x10 + x;
					if (++p >= last)
						break;
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					u = u * 0x10 + x;
					if (++p >= last)
						break;
					x = *p;
					if (!ACTOI(&x, 'f', 16))
						break;
					u = u * 0x10 + x;
					p++;
				} while (0);
				shift = u > 0xFFFF ? u > 0x00FFFFFF ? 32 : 24 : u > 0xFF ? 16 : 8;
				continue;
			}
		default:
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p - 1, 1, &w, 1);
			else if (p < last)
				cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, p++ - 1, 2, &w, 1);
			else
				break;
			if (!cchWideChar)
				break;
			u = 0;
			cbUtf8 = WideCharToMultiByte(CP_UTF8, 0, &w, 1, (char *)&u, sizeof(u), NULL, NULL);
			if (shift = cbUtf8 * 8)
				continue;
			else
				break;
		}
		break;
	}
	*pfirst = p;
	return n;
}
