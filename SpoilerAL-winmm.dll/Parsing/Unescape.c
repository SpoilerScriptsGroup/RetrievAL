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
					if (p < last && !IsDBCSLeadByteEx(CP_THREAD_ACP, c) || ++p < last)
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
				break;
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
				break;
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
