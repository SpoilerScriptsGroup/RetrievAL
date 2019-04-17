#ifdef GetFileTitlePointerT
#include <windows.h>
#ifdef __BORLANDC__
#pragma warn -8058
#endif
TCHAR * __fastcall GetFileTitlePointerT(const TCHAR *lpFileNeme)
{
	TCHAR *lpTitle, c;

	lpTitle = (TCHAR *)lpFileNeme;
	while (c = *(lpFileNeme++))
		if (c == TEXT('\\') || c == TEXT('/') || c == TEXT(':'))
			lpTitle = (TCHAR *)lpFileNeme;
#ifndef _UNICODE
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, c) && !*(lpFileNeme++))
			break;
#endif
	return lpTitle;
}
#endif
