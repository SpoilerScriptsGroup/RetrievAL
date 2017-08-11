#include "SubjectStringTable.h"
#include "TSSGSubject.h"

vector_string SubjectStringTable_array = { 0 };
vector_size_t SubjectStringTable_indices = { 0 };

#define array   SubjectStringTable_array
#define indices SubjectStringTable_indices

void __cdecl SubjectStringTable_dtor()
{
	vector_dtor(&indices);
	vector_string_dtor(&array);
}

void __cdecl SubjectStringTable_clear()
{
	vector_dtor(&indices);
	vector_ctor(&indices);
	vector_string_dtor(&array);
	vector_ctor(&array);
	vector_string_resize(&array, 1);
	vector_BYTE_resize(&indices, sizeof(size_t));
	*vector_begin(&indices) = 0;
}

size_t __fastcall SubjectStringTable_insert(string *s)
{
	if (string_empty(s))
		return 0;
	size_t size = vector_bytes(&indices);
	size_t count = size / sizeof(size_t);
	size_t middle = 1;
	size_t low = 1;
	size_t high = count - 1;
	while (low <= high)
	{
		middle = (low + high) / 2;
		size_t index = vector_at(&indices, middle);
		int cmp = strcmp(string_begin(&vector_at(&array, index)), string_begin(s));
		if (!cmp)
			return index;
		if (cmp < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}
	vector_BYTE_resize(&array, vector_bytes(&array) + sizeof(string));
	string_ctor_assign(bcb6_std_vector_end(&array) - 1, s);
	vector_BYTE_resize(&indices, size + sizeof(size_t));
	size_t *src = &vector_at(&indices, middle);
	memmove(src + 1, src, size - middle * sizeof(size_t));
	return *src = count;
}
