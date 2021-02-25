#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMenuItem.h"
#include "version.h"

const char lpVersion[] = "version : " VERSION_STRING;

void __stdcall ModifyVersionString(TMainForm *MainForm)
{
	TMenuItem_SetCaption(MainForm->M_Version, lpVersion);
}
