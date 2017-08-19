#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "bcb6_std_allocator.h"

void(__cdecl *vector_string_reserve)(vector_string *s, size_t n) = (LPVOID)0x00473564;

void __fastcall vector_string_deallocate(string *first, string *last)
{
	while (first != last)
		string_dtor(first++);
}

void __fastcall vector_string_dtor(vector_string *v)
{
	if (!vector_empty(v))
	{
		vector_string_deallocate(vector_begin(v), vector_end(v));
		allocator_deallocate(vector_begin(v), vector_BYTE_capacity(v));
	}
}

void __fastcall vector_string_clear(vector_string *v)
{
	vector_string_deallocate(vector_begin(v), vector_end(v));
	vector_clear(v);
}

void __fastcall vector_string_erase(vector_string *v, string *first, string *last)
{
	size_t size;
	if (size = (char *)last - (char *)first)
	{
		(char *)vector_end(v) -= size;
		vector_string_deallocate(first, last);
		if (size = (char *)vector_end(v) - (char *)first)
			memcpy(first, last, size);
	}
}

void __fastcall vector_string_resize(vector_string *v, size_t n)
{
	if (n)
	{
		n *= sizeof(string);
		size_t size = vector_BYTE_size(v);
		if (n > size)
		{
			size_t capacity = vector_BYTE_capacity(v);
			if (n > capacity)
			{
				capacity = size + max(size, n - size);
				char *p = (char *)allocator_reallocate(vector_begin(v), vector_BYTE_capacity(v), capacity);
				vector_begin(v) = (string *)p;
				v->_M_end_of_storage = (string *)(p + capacity);
			}
			string *end = (string *)((char *)vector_begin(v) + n);
			for (string *it = vector_end(v); it != end; it++)
				string_ctor(it);
			vector_end(v) = end;
		}
		else if (n < size)
		{
			string *last = vector_end(v);
			vector_end(v) = (string *)((char *)vector_begin(v) + n);
			vector_string_deallocate(vector_end(v), last);
		}
	}
	else
	{
		vector_string_clear(v);
	}
}
