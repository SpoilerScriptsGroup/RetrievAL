#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "TEdit.h"
#include "TMainForm.h"
#include "TSSGSubject.h"

void __fastcall _TMainForm_DGridSelectCell(TMainForm *this, LPVOID Sender, int ACol, BOOLEAN *CanSelect, int ARow)
{
	*CanSelect = FALSE;
	if ((unsigned int)ARow <= vector_size(&this->treeSubjectVec))
		TEdit_SetFocus(this->HotKeyEdit);
}
