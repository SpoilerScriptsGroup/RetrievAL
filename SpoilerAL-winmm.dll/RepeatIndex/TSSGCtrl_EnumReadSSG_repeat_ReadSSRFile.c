#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SSGSubjectProperty.h"

extern DWORD RepeatDepth;

static void __stdcall repeat_ReadSSRFile(
	TSSGCtrl *_this,
	LPVOID   ParentStack,
	LPVOID   ADJElem,
	string   *LineS,
	DWORD    RepeatIndex,
	DWORD    ParentRepeat);

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
	TSSGCtrl *_this,
	LPVOID   ParentStack,
	LPVOID   ADJElem,
	string   *LineS,
	DWORD    RepeatIndex,
	DWORD    ParentRepeat)
{
	vector_string tmpV;
	vector_dword  indices;

	vector_ctor(&indices);
	TSSGCtrl_ReadSSRFile(&tmpV, _this, LineS, &indices);
	if (!vector_empty(&tmpV))
	{
		if (!vector_empty(&indices))
		{
			DWORD               parent;
			TSSGSubjectProperty *prop;
			string              *it;
			LPDWORD             repeat;
			size_t              elementSize;

			prop = GrowSubjectProperty(&parent);
			if (prop)
			{
				prop->RepeatDepth  = RepeatDepth;
				prop->RepeatIndex  = RepeatIndex;
				prop->ParentRepeat = ParentRepeat;
			}
			RepeatDepth++;
			it = tmpV._M_start;
			repeat = indices._M_start;
			elementSize = vector_byte_size(&tmpV) / vector_size(&indices);
			do
			{
				vector_string constElem;

				constElem._M_start = it;
				(LPBYTE)it += elementSize;
				constElem._M_end_of_storage = constElem._M_finish = (string *)it;
				TSSGCtrl_EnumReadSSG(_this, &constElem, ParentStack, ADJElem, *(repeat++), parent);
			} while (it != tmpV._M_finish);
			RepeatDepth--;
		}
		else
		{
			TSSGCtrl_EnumReadSSG(_this, &tmpV, ParentStack, ADJElem, RepeatIndex, ParentRepeat);
		}
	}
	vector_dtor(&indices);
	vector_string_dtor(&tmpV);
}

