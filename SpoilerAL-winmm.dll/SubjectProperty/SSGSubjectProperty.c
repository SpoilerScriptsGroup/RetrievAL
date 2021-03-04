#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_allocator.h"
#include "SSGSubjectProperty.h"
#include "TMainForm.h"

extern HANDLE hHeap;

TSSGSubjectProperty *SubjectProperty = NULL;
DWORD               SubjectPropertyCount = 0;
DWORD static        Allocation;
DWORD               TitleWidth = 0;
void(__cdecl *const tree_string_double_M_erase)(struct _Rb_tree *this, struct _Rb_tree_node *__x) = (void *)0x0051C168;

void __fastcall TSSGSubjectProperty_dtor(TSSGSubjectProperty *const prop, BOOL const dealloc)
{
	if (prop->_M_node_count)
	{
		tree_string_double_M_erase(prop, map_end(prop)->_M_parent);
		if (!dealloc)
		{
			map_end(prop)->_M_left   = map_end(prop);
			map_end(prop)->_M_parent = NULL;
			map_end(prop)->_M_right  = map_end(prop);
		}
		prop->_M_node_count = 0;
	}
	if (dealloc)
	{
		node_alloc_deallocate(
			map_end(prop)
#if !OPTIMIZE_ALLOCATOR
			, sizeof(struct _Rb_tree_node)
#endif
		);
		map_end(prop) = NULL;
	}
}

void __cdecl ClearSubjectProperty()
{
	SubjectPropertyCount = 0;
	if (SubjectProperty != NULL)
	{
		HeapFree(hHeap, 0, SubjectProperty);
		SubjectProperty = NULL;
	}
	TitleWidth = 0;
}

TSSGSubjectProperty * __fastcall GrowSubjectProperty(DWORD *lpdwIndex)
{
	TSSGSubjectProperty *prop;
	DWORD index;

	do	/* do { ... } while (0); */
	{
		if (SubjectPropertyCount)
		{
			if (MainForm->ssgCtrl.script.sPos)
			{
				DWORD i = SubjectPropertyCount;
				do if (SubjectProperty[--i].RepeatDepth == MAXDWORD)
				{
					 prop = &SubjectProperty[index = i];
					*prop = (const TSSGSubjectProperty) { .OuterRepeat = MAXDWORD };
					goto RESOLVED;
				}
				while (i);
			}
			if (SubjectPropertyCount < Allocation)
				prop = SubjectProperty;
			else
			{
				prop = (TSSGSubjectProperty *)HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, SubjectProperty, sizeof(TSSGSubjectProperty) * (Allocation << 1));
				if (prop == NULL)
					break;
				SubjectProperty = prop;
				Allocation <<= 1;
			}
		}
		else
		{
			prop = (TSSGSubjectProperty *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(TSSGSubjectProperty) * (Allocation = 0x10));
			if (prop == NULL)
				break;
			SubjectProperty = prop;
		}
		prop += index = SubjectPropertyCount++;
		prop->OuterRepeat = MAXDWORD;
	} while (0);
RESOLVED:
	*lpdwIndex = prop == NULL ? MAXDWORD : index;
	return prop;
}
