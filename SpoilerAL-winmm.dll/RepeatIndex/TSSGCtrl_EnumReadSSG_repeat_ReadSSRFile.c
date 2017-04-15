#include <windows.h>
#include "TSSGCtrl.h"
#include "SSGSubjectProperty.h"
#include "bcb6_std_vector_string.h"

EXTERN_C DWORD RepeatDepth;

static void __stdcall repeat_ReadSSRFile(
	TSSGCtrl        *_this,
	LPVOID          ParentStack,
	LPVOID          ADJElem,
	bcb6_std_string *LineS,
	DWORD           RepeatIndex,
	DWORD           ParentRepeat);

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile()
{
	__asm
	{
		#define ReturnAddress   004EB508H
		#define _this           (ebp +   8H)
		#define ParentStack     (ebp +  10H)
		#define ADJElem         (ebp +  14H)
		#define RepeatIndex     (ebp +  18H)
		#define ParentRepeat    (ebp +  1CH)
		#define LineS           (ebp -  38H)

		mov     edx, dword ptr [ParentRepeat]
		lea     eax, [LineS]
		mov     ecx, dword ptr [RepeatIndex]
		push    edx
		push    ecx
		push    eax
		mov     edx, dword ptr [ADJElem]
		mov     ecx, dword ptr [ParentStack]
		mov     eax, dword ptr [_this]
		push    edx
		push    ecx
		push    eax
		push    ReturnAddress
		jmp     repeat_ReadSSRFile

		#undef ReturnAddress
		#undef _this
		#undef ParentStack
		#undef ADJElem
		#undef RepeatIndex
		#undef ParentRepeat
		#undef LineS
	}
}

static void __stdcall repeat_ReadSSRFile(
	TSSGCtrl        *_this,
	LPVOID          ParentStack,
	LPVOID          ADJElem,
	bcb6_std_string *LineS,
	DWORD           RepeatIndex,
	DWORD           ParentRepeat)
{
	bcb6_std_vector tmpV;
	bcb6_std_vector indices;

	bcb6_std_vector_ctor(&indices);
	TSSGCtrl_ReadSSRFile(&tmpV, _this, LineS, &indices);
	if (!bcb6_std_vector_empty(&tmpV))
	{
		if (!bcb6_std_vector_empty(&indices))
		{
			DWORD               parent;
			TSSGSubjectProperty *prop;
			LPVOID              it;
			LPDWORD             repeat;
			size_t              elementSize;
			bcb6_std_vector     element;

			prop = GrowSubjectProperty(&parent);
			if (prop)
			{
				prop->RepeatDepth  = RepeatDepth;
				prop->RepeatIndex  = RepeatIndex;
				prop->ParentRepeat = ParentRepeat;
			}
			RepeatDepth++;
			it = tmpV._M_start;
			repeat = (LPDWORD)indices._M_start;
			elementSize = bcb6_std_vector_size(&tmpV, BYTE) / bcb6_std_vector_size(&indices, DWORD);
			do
			{
				element._M_start = it;
				element._M_end_of_storage = element._M_finish = (LPBYTE)it += elementSize;
				TSSGCtrl_EnumReadSSG(_this, &element, ParentStack, ADJElem, *(repeat++), parent);
			} while (it != tmpV._M_finish);
			RepeatDepth--;
		}
		else
		{
			TSSGCtrl_EnumReadSSG(_this, &tmpV, ParentStack, ADJElem, RepeatIndex, ParentRepeat);
		}
	}
	bcb6_std_vector_dtor(&indices);
	bcb6_std_vector_dtor(&tmpV);
}

