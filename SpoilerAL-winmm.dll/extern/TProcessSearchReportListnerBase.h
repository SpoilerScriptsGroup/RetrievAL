#pragma once

#include <windows.h>

typedef struct _TProcessSearchReportListnerBase
{
	LPVOID        *vftable;
	unsigned long reserveMode;
	void          *thisPointer;
	unsigned long reserveVal;
	unsigned long reportSpan;
	unsigned long start, min, max, step;

} TProcessSearchReportListnerBase;

