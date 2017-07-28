#include "SubjectStringTable.h"
#include "bcb6_std_vector_string.h"
#include "TSSGSubject.h"

extern HANDLE hHeap;

size_t SubjectStringTable_count    = 0;
size_t *SubjectStringTable_indices = NULL;
STRING *SubjectStringTable_array   = NULL;

static char empty[] = { '\0' };

#define count   SubjectStringTable_count
#define indices SubjectStringTable_indices
#define array   SubjectStringTable_array

void __cdecl SubjectStringTable_dtor()
{
	if (count)
	{
		HeapFree(hHeap, 0, indices);
		for (STRING *p = array + 1, *end = array + count; p < end; p++)
			HeapFree(hHeap, 0, p->begin);
		HeapFree(hHeap, 0, array);
		indices = NULL;
		array = NULL;
		count = 0;
	}
}

void __cdecl SubjectStringTable_clear()
{
	if (count)
	{
		HeapFree(hHeap, 0, indices);
		for (STRING *p = array + 1, *end = array + count; p < end; p++)
			HeapFree(hHeap, 0, p->begin);
		HeapFree(hHeap, 0, array);
	}
	array = (STRING *)HeapAlloc(hHeap, 0, sizeof(STRING) * 0x10);
	array->end = array->begin = empty;
	indices = (size_t *)HeapAlloc(hHeap, 0, sizeof(size_t) * 0x10);
	*indices = 0;
	count = 1;
}

size_t __fastcall SubjectStringTable_insert(bcb6_std_string *s)
{
	if (bcb6_std_string_empty(s))
		return 0;
	size_t middle = 1;
	size_t low = 1;
	size_t high = count - 1;
	while (low <= high)
	{
		middle = (low + high) / 2;
		size_t index = indices[middle];
		int cmp = strcmp(array[index].begin, s->_M_start);
		if (!cmp)
			return index;
		if (cmp < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}
	if (!(count & 0x0F))
	{
		void *memblock = HeapReAlloc(hHeap, 0, array, sizeof(STRING) * (count + 0x10));
		if (!memblock)
			return 0;
		array = (STRING *)memblock;
		memblock = HeapReAlloc(hHeap, 0, indices, sizeof(size_t) * (count + 0x10));
		if (!memblock)
			return 0;
		indices = (size_t *)memblock;
	}
	size_t length = bcb6_std_string_length(s);
	if (!(array[count].begin = (char *)HeapAlloc(hHeap, 0, length + 1)))
		return 0;
	*(array[count].end = array[count].begin + length) = '\0';
	memcpy(array[count].begin, s->_M_start, length);
	size_t *src = indices + middle;
	memmove(src + 1, src, (count - middle) * sizeof(size_t));
	return *src = count++;
}
