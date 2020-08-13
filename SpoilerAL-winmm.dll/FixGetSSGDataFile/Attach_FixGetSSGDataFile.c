#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#pragma intrinsic(memcmp)
#include "TSSGCtrl.h"
#include <mbstring.h>

EXTERN_C void __cdecl FixGetSSGDataFile();
EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);

static __declspec(naked) void TSSGCtrl_GetSSGDataFile_Half(
	bcb6_std_string *FileName,
	TStringDivision *strD,
	bcb6_std_string *FName,
	bcb6_std_string  Token,
	unsigned long    Index,
	unsigned long    Option) {
	extern BOOL EnableParserFix;
	__asm {// eax is FName already
		cmp  EnableParserFix, 0
		je   SKIP
		mov  edx, [ebp + 0x0C]// SSGS
		test edx, edx
		jz   SKIP
		push eax
		push edx
		call ReplaceDefineDynamic
	SKIP:
		jmp  TStringDivision_Half
	}
}

static BOOL __fastcall FunctionableGroup(string *name, vector_string *func)
{
	#define BOM BSWAP32(0xEFBBBF00)

	static size_t const header[2] = { BOM, -1 };
	unsigned char *p, *first, *last, *src, *dest, *end;

	if (!(p = _mbschr(string_begin(name), '(')))
		return FALSE;
	end = string_end(name);
	*(string_end(name) = TrimRightBlank(string_begin(name), p)) = '\0';
	vector_string_push_back_range(func, (char *)header, (char *)header + sizeof(header));
	for (first = p + 1; ; )
	{
		switch (*(++p))
		{
		default:
			continue;
		case ')':
			last = TrimBlank(&first, p);
			if (last != first) vector_string_push_back_range(func, first, last);
			break;
		case ',':
			last = TrimBlank(&first, p);
			vector_string_push_back_range(func, first, last);
			first = p + 1;
			continue;
		case '/':
			switch (p[1])
			{
			default:
				continue;
			case '*':
				for (src = p + 1; ; )
				{
					switch (*(++src))
					{
					default:
						continue;
					case '*':
						if (src[1] != '/')
							continue;
						src += 2;
						dest = p--;
						memmove(dest, src, (end -= src - dest) - dest + 1);
						break;
					case_unsigned_leadbyte_cp932:
						if (*(++src))
							continue;
					case '\0':
						goto TERMINATE;
					}
					break;
				}
				continue;
			case '/':
			TERMINATE:
				break;
			}
			break;
		case_unsigned_leadbyte_cp932:
			if (*(++p))
				continue;
		case '\0':
			break;
		}
		break;
	}
	return TRUE;

	#undef BOM
}

map_iterator  (__cdecl *const map_string_lower_bound)(map *, const string *key) = (LPVOID)0x004F20E4;
map_iterator *(__cdecl *const map_string_vector_insert)(map_iterator *, map *, map_iterator pos, const void *pair) = (LPVOID)0x004F2424;

static void __fastcall TSSGCtrl_SetSSGDataFile_IsSSL(
	TSSGCtrl*  const SSGC,
	map*       const tmpM,
	register string* VIt,
	string*    const VEnd)
{
	extern BOOL FixTheProcedure;

	vector_string names;

	vector_ctor(&names);
	for (; VIt < VEnd; VIt++)
	{
		string *first;
		size_t length = 0;
		vector_string func = { NULL };

		if (string_length(VIt) < 7 || ((LPDWORD)string_begin(VIt))[0] != BSWAP32('[gro') || (((LPDWORD)string_begin(VIt))[1] & 0x00FFFFFF) != BSWAP32('up]\0'))
			continue;
		memmove(string_begin(VIt), string_begin(VIt) + 7, (string_end(VIt) -= 7) - string_begin(VIt) + 1);
		vector_string_clear(&names);
		if (FunctionableGroup(VIt, &func) || !FixTheProcedure)
			vector_string_push_back(&names, VIt);
		else
		{
			string Token;

			string_ctor_assign_char(&Token, ',');
			TStringDivision_List(&SSGC->strD, VIt, Token, &names, etTRIM);
		}
		first = VIt + 1;
		while (++VIt < VEnd && (string_length(VIt) != 8 || ((LPDWORD)string_begin(VIt))[0] != BSWAP32('[/gr') || ((LPDWORD)string_begin(VIt))[1] != BSWAP32('oup]')))
			length += string_length(VIt) + 1;
		for (string *name = vector_begin(&names); name < vector_end(&names); name++)
		{
			vector_string *Data;

			map_iterator it = map_string_lower_bound(tmpM, name);
			if (it == map_end(tmpM) || !string_equals(pair_first(it), name))
			{
				struct {
					string        GroupTag;
					vector_string GroupV;
				} tmpMpair = { *name };

				map_string_vector_insert(&it, tmpM, it, &tmpMpair);
			}
			Data = (vector_string *)pair_second_aligned(it, string);
			if (vector_empty(&func))
			{
				if (!FixTheProcedure)
					vector_string_clear(Data);
				vector_string_reserve(Data, vector_size(Data) + (VIt - first));
				for (const string *LineS = first; LineS < VIt; LineS++)
					vector_string_push_back(Data, LineS);
			}
			else
			{
				string_reserve(vector_begin(&func), sizeof(size_t) * 2 + length);
				for (const string *LineS = first; LineS < VIt; LineS++)
				{
					string_append(vector_begin(&func), LineS);
					string_push_back(vector_begin(&func), '\n');
				}
				vector_string_dtor(Data);
				*Data = func;
			}
		}
	}
	vector_string_dtor(&names);
}

static void __cdecl TSSGCtrl_SetSSGDataFile_insert(
	map        * const second,
	map_iterator const begin,
	map_iterator const end)
{
	for (map_iterator it = begin; it != end; map_iterator_increment(it)) {
		map_iterator lb = map_string_lower_bound(second, pair_first(it));
		if (lb != map_end(second) && string_equals(pair_first(lb), pair_first(it))) {
			vector_string* src = pair_second_aligned(it, string);
			vector_string* dst = pair_second_aligned(lb, string);
			vector_string_dtor(dst);
			*dst = *src;
			vector_ctor(src);
		} else map_string_vector_insert(&lb, second, lb, pair_first(it));
	}
}

#define CALL_REL  (BYTE)0xE8
#define JMP_REL32 (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixGetSSGDataFile()
{
	// TSSGCtrl::GetSSGDataFile
	*(LPDWORD)(0x004EF8E9 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_Half - (0x004EF8E9 + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004EF9C2 + 1) = (DWORD)FixGetSSGDataFile - (0x004EF9C2 + 1 + sizeof(DWORD));

	// TSSGCtrl::SetSSGDataFile
	*(LPDWORD)0x004F0B08 = BSWAP32(0x52568D55);
	*(LPDWORD)0x004F0B0C = BSWAP32(0xE08B4D08);
	*(LPBYTE )0x004F0B10 = CALL_REL;
	*(LPDWORD)0x004F0B11 = (DWORD)TSSGCtrl_SetSSGDataFile_IsSSL - (0x004F0B11 + sizeof(DWORD));
	*(LPBYTE )0x004F0B15 = JMP_REL32;

	*(LPDWORD)(0x004F1A74 + 1) = (DWORD)TSSGCtrl_SetSSGDataFile_insert - (0x004F1A74 + 1 + sizeof(DWORD));
}
