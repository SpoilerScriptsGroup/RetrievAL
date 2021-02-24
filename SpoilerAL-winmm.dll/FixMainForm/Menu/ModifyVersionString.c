#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMenuItem.h"
#include "version.h"

void __stdcall ModifyVersionString(TMainForm *MainForm)
{
	extern const char lpVersion[];

	TMenuItem_SetCaption(MainForm->M_Version, lpVersion);
}
