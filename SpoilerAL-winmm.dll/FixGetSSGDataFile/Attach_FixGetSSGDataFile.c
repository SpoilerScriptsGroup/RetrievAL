#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#pragma intrinsic(memcmp)
#include "TSSGCtrl.h"

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

map_iterator (__cdecl * const map_string_lower_bound)(map*, string* key) = (LPVOID)0x004F20E4;
map_iterator*(__cdecl * const map_string_vector_insert)(map_iterator*, map*, map_iterator pos, void* pair) = (LPVOID)0x004F2424;

static void __fastcall TSSGCtrl_SetSSGDataFile_IsSSL(
	TSSGCtrl*  const SSGC,
	map*       const tmpM,
	register string* VIt,
	string*    const VEnd)
{
	extern BOOL FixTheProcedure;
	static char HEAD[] = "[group", TAIL[] = "[/group]";
	static string const GROUP_HEAD = { HEAD, HEAD + sizeof(HEAD) - 1, NULL, NULL, HEAD + sizeof(HEAD), 0 };
	static string const GROUP_TAIL = { TAIL, TAIL + sizeof(TAIL) - 1, NULL, NULL, HEAD + sizeof(TAIL), 0 };

	vector_string names = { NULL };
	for (string tag; VIt < VEnd; VIt++) {
		if (string_empty(VIt)) continue;
		TStringDivision_Half_WithoutTokenDtor(&tag, &SSGC->strD, VIt, "]", 1, 0, FALSE);
		if (string_equals(&tag, &GROUP_HEAD)) {
			string* const begin = VIt + 1;
			if (FixTheProcedure) {
				string Token;
				string_ctor_assign_char(&Token, ',');
 				TStringDivision_List(&SSGC->strD, VIt, Token, &names, etTRIM);
			}
			else
			{
				vector_begin(&names) = VIt;
				vector_end(&names) = begin;
			}
			while (++VIt < VEnd && !string_equals(VIt, &GROUP_TAIL));
			for (string* name = vector_begin(&names); name < vector_end(&names); name++) {
				vector_string* Data;
				map_iterator it = map_string_lower_bound(tmpM, name);
				if (it == map_end(tmpM) || !string_equals(pair_first(it), name)) {
					struct {
						string        GroupTag;
						vector_string GroupV;
					} tmpMpair = { NULL };
					string_ctor_assign(&tmpMpair.GroupTag, name);
					map_string_vector_insert(&it, tmpM, it, &tmpMpair);
					string_dtor(&tmpMpair.GroupTag);
				}
				Data = (vector_string*)pair_second_aligned(it, string);
				if (!FixTheProcedure) vector_string_clear(Data);
				vector_string_reserve(Data, vector_size(Data) + (VIt - begin));
				for (const string* LineS = begin; LineS < VIt; LineS++)
					vector_string_push_back(Data, LineS);
			}
		}
		string_dtor(&tag);
	}
	if (FixTheProcedure)
		vector_string_dtor(&names);
}

static void __cdecl TSSGCtrl_SetSSGDataFile_insert(
	map        * const second,
	map_iterator const begin,
	map_iterator const end)
{
	for (map_iterator it = begin; it != end; it = map_iterator_increment(it)) {
		map_iterator lb = map_string_lower_bound(second, pair_first(it));
		if (lb != map_end(second) && string_equals(pair_first(lb), pair_first(it))) {
			vector_string* src = pair_second_aligned(it, string);
			vector_string* dst = pair_second_aligned(lb, string);
			vector_string_clear(dst);
			vector_string_reserve(dst, vector_size(src));
			for (string* s = string_begin(src); s < string_end(src); s++)
				vector_string_push_back(dst, s);
		} else map_string_vector_insert(&lb, second, lb, pair_first(it));
	}
}

#define CALL_REL                   (BYTE)0xE8
#define JMP_REL32                  (BYTE)0xE9

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
