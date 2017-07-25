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

size_t __fastcall SubjectStringTable_insert(bcb6_std_string *s)
{
	if (bcb6_std_string_empty(s))
		return 0;
	size_t size = bcb6_std_vector_size(&indices, BYTE);
	size_t count = size / sizeof(size_t);
	size_t middle = 1;
	size_t low = 1;
	size_t high = count - 1;
	while (low <= high)
	{
		middle = (low + high) / 2;
		size_t index = ((size_t *)indices._M_start)[middle];
		int cmp = strcmp(((bcb6_std_string *)array._M_start)[index]._M_start, s->_M_start);
		if (!cmp)
			return index;
		if (cmp < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}
	bcb6_std_vector_resize(&array, bcb6_std_vector_size(&array, BYTE) + sizeof(bcb6_std_string));
	bcb6_std_string_ctor_assign((bcb6_std_string *)array._M_finish - 1, s);
	bcb6_std_vector_resize(&indices, size + sizeof(size_t));
	size_t *src = (size_t *)indices._M_start + middle;
	memmove(src + 1, src, size - middle * sizeof(size_t));
	return *src = count;
}
