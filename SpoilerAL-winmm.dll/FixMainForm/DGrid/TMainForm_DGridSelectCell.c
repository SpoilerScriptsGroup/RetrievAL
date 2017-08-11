#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "TEdit.h"
#include "TMainForm.h"
#include "TSSGSubject.h"

void __fastcall _TMainForm_DGridSelectCell(TMainForm *_this, LPVOID Sender, int ACol, BOOLEAN *CanSelect, int ARow)
{
	*CanSelect = FALSE;
	if ((unsigned int)ARow <= vector_size(&_this->treeSubjectVec))
		TEdit_SetFocus(_this->HotKeyEdit);
}
