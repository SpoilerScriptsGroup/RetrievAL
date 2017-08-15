#include <windows.h>
#include <intrin.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_allocator.h"

void __fastcall bcb6_std_vector_BYTE_resize(bcb6_std_vector *v, size_t n)
{
	void *p = (char *)v->_M_start + n;
	if (p <= v->_M_end_of_storage)
	{
		if (p > v->_M_finish)
			memset(v->_M_finish, 0, (size_t)p - (size_t)v->_M_finish);
		v->_M_finish = p;
	}
	else
	{
		p = bcb6_std_node_alloc_allocate(n * 2);
		size_t size = (size_t)v->_M_finish - (size_t)v->_M_start;
		memcpy(p, v->_M_start, size);
		memset((char *)p + size, 0, n - size);
		bcb6_std_node_alloc_deallocate(v->_M_start, (size_t)v->_M_end_of_storage - (size_t)v->_M_start);
		v->_M_start = p;
		v->_M_finish = (char *)p + n;
		v->_M_end_of_storage = (char *)p + n * 2;
	}
}
