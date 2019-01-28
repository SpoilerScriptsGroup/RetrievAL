#include <windows.h>
#include "SSGSubjectProperty.h"
#include "TMainForm.h"

extern HANDLE hHeap;

TSSGSubjectProperty *SubjectProperty = NULL;
DWORD               SubjectPropertyCount = 0;
DWORD               TitleWidth = 0;

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
		if (SubjectPropertyCount != 0)
		{
			if (TSSGCtrl_GetSSGActionListner(&MainForm->ssgCtrl))
				for (DWORD i = 0; i < SubjectPropertyCount; i++)
					if (SubjectProperty[i].Width == MAXDWORD)
					{
						 prop = &SubjectProperty[index = i];
						*prop = (const TSSGSubjectProperty) { 0 };
						goto RECYCLE;
					}
			if ((SubjectPropertyCount & 0x0F) != 0)
			{
				prop = SubjectProperty;
			}
			else
			{
				prop = (TSSGSubjectProperty *)HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, SubjectProperty, sizeof(TSSGSubjectProperty) * (SubjectPropertyCount + 0x10));
				if (prop == NULL)
					break;
				SubjectProperty = prop;
			}
		}
		else
		{
			prop = (TSSGSubjectProperty *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(TSSGSubjectProperty) * 0x10);
			if (prop == NULL)
				break;
			SubjectProperty = prop;
		}
		prop += index = SubjectPropertyCount++;
	RECYCLE:
		prop->ParentRepeat = MAXDWORD;
	} while (0);
	*lpdwIndex = prop == NULL ? MAXDWORD : index;
	return prop;
}
