#ifdef GetFileTitlePointerT
#ifdef __BORLANDC__
#pragma warn -8058
#endif
TCHAR * __fastcall GetFileTitlePointerT(const TCHAR *lpFileNeme)
{
	TCHAR *lpTitle = (TCHAR *)lpFileNeme;
	if (*lpFileNeme)
	{
		do
		{
			if (*lpFileNeme == TEXT('\\') || *lpFileNeme == TEXT(':'))
				lpTitle = (TCHAR *)lpFileNeme + 1;
#ifndef _UNICODE
			else if (IsDBCSLeadByteEx(CP_THREAD_ACP, *lpFileNeme) && !*(++lpFileNeme))
				break;
#endif
		} while (*(++lpFileNeme));
	}
	return lpTitle;
}
#endif
