#ifndef _VERSION_H_
#define _VERSION_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "datetime.h"

#ifndef TOSTRING
#define STRINGIFY(a) #a
#define TOSTRING(a) STRINGIFY(a)
#endif

#define VERSION_MAJOR    6
#define VERSION_MINOR    4
#define VERSION_BUILD    __DATE_YEAR__
#define VERSION_REVISION __DATE_MDD__

#define VERSION_STRING \
	TOSTRING(VERSION_MAJOR) "." \
	TOSTRING(VERSION_MINOR) "." \
	TOSTRING(__DATE_YYYY__) "." \
	TOSTRING(__DATE_MM__) TOSTRING(__DATE_DD__)

#endif	// _VERSION_H_
