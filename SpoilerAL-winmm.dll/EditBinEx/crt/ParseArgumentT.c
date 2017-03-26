#ifdef ParseArgumentT

#if _MSC_VER >= 1400
#include <intrin.h>
#include <tchar.h>

#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(_tcslen)
#else
__inline void __movsb(void *Destination, const void *Source, size_t Count)
{
	__asm
	{
		mov     esi, Destination
		mov     edi, Source
		mov     ecx, Count
		rep movsb
	}
}
__inline void __movsw(void *Destination, const void *Source, size_t Count)
{
	__asm
	{
		mov     esi, Destination
		mov     edi, Source
		mov     ecx, Count
		rep movsw
	}
}
__inline size_t wcslen(const wchar_t *string)
{
	const wchar_t *p = string;
	while (*p)
		p++;
	return p - string;
}
#endif

#define memcpy __movsb
#define wmemcpy __movsw

#ifdef _UNICODE
#define _tmemcpy wmemcpy
#else
#define _tmemcpy memcpy
#endif

TCHAR ** __stdcall ParseArgumentT(HANDLE hHeap, const TCHAR *lpParameters, int *argc)
{
	const TCHAR *begin, *end;

	begin = lpParameters;
	end = begin + _tcslen(begin);
	while (*begin == TEXT(' '))
		begin++;
	if (end > begin)
	{
		size_t length, count, capacity;
		TCHAR  **argv;

		length = end - begin;
		count = (length + 1) / 2;
		capacity = count * sizeof(TCHAR *) + (length + 1) * sizeof(TCHAR);
		argv = (TCHAR **)HeapAlloc(hHeap, 0, capacity);
		if (argv)
		{
			TCHAR       *p;
			BOOLEAN     inDoubleQuote;
			size_t      index;
			const TCHAR *src;
#if defined(SHRINK_BUFFER) && SHRINK_BUFFER
			TCHAR       *dest;
			size_t      diff;
			LPVOID      memBlock;
#endif

			p = (TCHAR *)(argv + count);
			*(TCHAR *)(end = p + length) = TEXT('\0');
			_tmemcpy(p, begin, length);
			inDoubleQuote = FALSE;
			argv[index = 0] = p;
			while (*p)
			{
#ifndef _UNICODE
				if (!IsDBCSLeadByte(*p))
				{
#endif
					switch (*p)
					{
					case TEXT(' '):
						if (inDoubleQuote)
							break;
						*p = TEXT('\0');
#if !defined(SHRINK_BUFFER) || !SHRINK_BUFFER
						while (*(++p) == TEXT(' '));
#else
						if (*(++p) == TEXT(' '))
						{
							src = p;
							while (*(++src) == TEXT(' '));
							count = end - src + 1;
							end -= src - p;
							_tmemcpy(p, src, count);
						}
#endif
						if (*p)
							argv[++index] = p;
						continue;
					case TEXT('"'):
						inDoubleQuote = !inDoubleQuote;
						_tmemcpy(p, p + 1, (end--) - p);
						continue;
					case TEXT('\\'):
						length = 0;
						while (p[++length] == TEXT('\\'));
						if (p[length] == TEXT('"'))
						{
							length = (length + 1) / 2;
							src = p + length;
							_tmemcpy(p, src, end - src + 1);
							end -= length;
						}
						p += length;
						continue;
					}
#ifndef _UNICODE
				}
				else if (!*(++p))
					break;
#endif
				p++;
			}
#if !defined(SHRINK_BUFFER) || !SHRINK_BUFFER
			*argc = (int)index + 1;
			return argv;
#else
			src = argv[0];
			length = end - src;
			count = index + 1;
			dest = (TCHAR *)(argv + count);
			if (diff = (size_t)dest - (size_t)src)
				_tmemcpy(dest, src, length + 1);
			capacity = count * sizeof(TCHAR *) + (length + 1) * sizeof(TCHAR);
			memBlock = HeapReAlloc(hHeap, 0, argv, capacity);
			if (memBlock)
			{
				diff += (size_t)memBlock - (size_t)argv;
				argv = (TCHAR **)memBlock;
				if (diff)
					for (index = 0; index < count; index++)
						*(size_t *)(argv + index) += diff;
				*argc = (int)count;
				return argv;
			}
			else
			{
				HeapFree(hHeap, 0, argv);
			}
#endif
		}
	}
	*argc = 0;
	return NULL;
}
#endif
