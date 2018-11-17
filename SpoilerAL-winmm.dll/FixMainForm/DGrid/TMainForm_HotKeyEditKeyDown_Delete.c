#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "TWinControl.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_Delete(TMainForm *this)
{
	HWND          edit;
	unsigned long start, end;

	edit = TWinControl_GetHandle(vector_at(&this->calcImage->valBox, 1).edit);
	SendMessageA(edit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
	if (start == end)
		SendMessageA(edit, EM_SETSEL, start, start + 1);
	SendMessageA(edit, EM_REPLACESEL, FALSE, (WPARAM)"");
}
