#include "SubjectStringTable.h"
#include "bcb6_std_vector_string.h"
#include "TSSGSubject.h"

bcb6_std_vector SubjectStringTable_array = { 0 };
bcb6_std_vector SubjectStringTable_indices = { 0 };

#define array   SubjectStringTable_array
#define indices SubjectStringTable_indices

void __cdecl SubjectStringTable_dtor()
{
	bcb6_std_vector_dtor(&indices);
	bcb6_std_vector_string_dtor(&array);
}

void __cdecl SubjectStringTable_clear()
{
	bcb6_std_vector_dtor(&indices);
	bcb6_std_vector_ctor(&indices);
	bcb6_std_vector_string_dtor(&array);
	bcb6_std_vector_ctor(&array);
	bcb6_std_vector_string_resize(&array, 1);
	bcb6_std_vector_resize(&indices, sizeof(size_t));
	*(size_t *)indices._M_start = 0;
}

static size_t __fastcall internal_SubjectStringTable_insert(const char *s, size_t length);

__declspec(naked) size_t __fastcall SubjectStringTable_insert(bcb6_std_string *s)
{
	__asm
	{
		mov     edx, dword ptr [ecx + 4]
		mov     ecx, dword ptr [ecx]
		sub     edx, ecx
		jmp     internal_SubjectStringTable_insert
	}
}

__declspec(naked) size_t __fastcall SubjectStringTable_insert_cstr(const char *s)
{
	__asm
	{
		mov     edx, SIZE_MAX
		jmp     internal_SubjectStringTable_insert
	}
}

static size_t __fastcall internal_SubjectStringTable_insert(const char *s, size_t length)
{
	if (!s || !length || !*s)
		return 0;
	size_t index;
	size_t middle = 1;
	size_t low = 1;
	size_t high = bcb6_std_vector_size(&indices, size_t) - 1;
	while (low <= high)
	{
		middle = (low + high) / 2;
		index = ((size_t *)indices._M_start)[middle];
		int cmp = strcmp(((bcb6_std_string *)array._M_start)[index]._M_start, s);
		if (!cmp)
			return index;
		if (cmp < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}
	size_t size = bcb6_std_vector_size(&array, BYTE);
	index = size / sizeof(bcb6_std_string);
	bcb6_std_vector_resize(&array, size + sizeof(bcb6_std_string));
	if (length == SIZE_MAX)
		length = strlen(s);
	bcb6_std_string_ctor_assign_cstr_with_length((bcb6_std_string *)((LPBYTE)array._M_start + size), s, length);
	size = bcb6_std_vector_size(&indices, BYTE);
	bcb6_std_vector_resize(&indices, size + sizeof(size_t));
	size -= middle * sizeof(size_t);
	size_t *src = (size_t *)indices._M_start + middle;
	memmove(src + 1, src, size);
	((size_t *)indices._M_start)[middle] = index;
	return index;
}
