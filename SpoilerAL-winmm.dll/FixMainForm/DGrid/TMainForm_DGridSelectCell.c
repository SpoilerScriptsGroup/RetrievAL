#include <windows.h>
#include "TEdit.h"
#include "TMainForm.h"
#include "TSSGSubject.h"

void __fastcall _TMainForm_DGridSelectCell(TMainForm *_this, LPVOID Sender, int ACol, BOOLEAN *CanSelect, int ARow)
{
	*CanSelect = FALSE;
	if ((unsigned int)ARow <= bcb6_std_vector_size(&_this->treeSubjectVec, TSSGSubject *))
		TEdit_SetFocus(_this->HotKeyEdit);
}
