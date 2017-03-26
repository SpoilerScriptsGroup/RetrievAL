#pragma once

#include "bcb6_std_vector.h"

#define TProcessAccessElementData_GetData(AElem) \
	(bcb6_std_vector *)((LPBYTE)(AElem) + 16)

