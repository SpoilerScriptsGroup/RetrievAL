#include <vcl.h>
#pragma hdrstop

USEFORM("gui\MainUnit.cpp", MainForm);

#define USE_HEAP_MEMORY 1

#if defined(USE_HEAP_MEMORY) && USE_HEAP_MEMORY
extern HANDLE hHeap;
#endif

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
#if defined(USE_HEAP_MEMORY) && USE_HEAP_MEMORY
		hHeap = GetProcessHeap();
		if (hHeap == NULL)
			throw;
#endif
		Application->Initialize();
		Application->Title = "SpoilerAL";
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
