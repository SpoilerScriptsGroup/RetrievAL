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
#ifndef _UNICODE
			if (!IsDBCSLeadByte(*lpFileNeme))
			{
#endif
				switch (*lpFileNeme)
				{
				case TEXT('\\'):
				case TEXT(':'):
					lpTitle = (TCHAR *)lpFileNeme + 1;
					break;
				}
#ifndef _UNICODE
			}
			else
			{
				if (!*(++lpFileNeme))
					break;
			}
#endif
		} while (*(++lpFileNeme));
	}
	return lpTitle;
}
#endif
