#include "SubjectStringTable.h"
#include "TSSGSubject.h"

#pragma function(memcmp)

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
	vector_BYTE_reserve(&array, sizeof(string));
	string_ctor(vector_end(&array)++);
	vector_BYTE_reserve(&indices, sizeof(size_t));
	*(vector_end(&indices)++) = 0;
}

size_t __fastcall SubjectStringTable_insert(const string *s)
{
	const char *right = string_begin(s);
	size_t rightLength = string_end(s) - right;
	if (!rightLength)
		return 0;
	size_t size = vector_size(&indices);
	size_t low = 1;
	size_t high = size - 1;
	while (low <= high)
	{
		size_t middle = (low + high) / 2;
		size_t index = vector_at(&indices, middle);
		const string *elem = &vector_at(&array, index);
		const char *left = string_begin(elem);
		size_t length = string_end(elem) - left;
		if (length > rightLength)
			length = rightLength;
		int ret = memcmp(left, right, length + 1);
		if (!ret)
			return index;
		if (ret < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}
	vector_string_push_back(&array, s);
	vector_insert(&indices, vector_begin(&indices) + low, size);
	return size;
}
