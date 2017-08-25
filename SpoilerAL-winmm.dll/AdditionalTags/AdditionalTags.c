#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

void __stdcall Attribute_expr(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);
void __stdcall Attribute_undef(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);
void __stdcall Attribute_define(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);
void __stdcall Attribute_allcate(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);
void __stdcall Attribute_variable_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);
void __stdcall Attribute_variable_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);
void __stdcall Attribute_error_skip_open(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);
void __stdcall Attribute_error_skip_close(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);

#define _BSWAP16(value) (           \
    (((value) >> 8) & 0x000000FF) | \
    (((value) << 8) & 0x0000FF00))

#define _BSWAP32(value) (            \
    (((value) >> 24) & 0x000000FF) | \
    (((value) >>  8) & 0x0000FF00) | \
    (((value) <<  8) & 0x00FF0000) | \
    (((value) << 24) & 0xFF000000))

__declspec(naked) void __cdecl AdditionalTags()
{
	static void __stdcall InternalAdditionalTags(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code);

	__asm
	{
		#define ReturnAddress 004EB508H
		#define SSGCtrl       (ebp +  8H)
		#define ParentStack   (ebp + 10H)
		#define tmpS          (ebp - 50H)
		#define LineS         (ebp - 38H)

		lea     eax, [LineS]
		push    eax
		lea     ecx, [tmpS]
		push    ecx
		mov     eax, dword ptr [ParentStack]
		mov     ecx, dword ptr [eax + 16]
		mov     edx, dword ptr [eax + 20]
		cmp     ecx, edx
		jne     L1
		mov     ecx, dword ptr [eax + 28]
		mov     ecx, dword ptr [ecx - 4]
		add     ecx, 4
	L1:
		mov     eax, dword ptr [ecx - 4]
		mov     ecx, dword ptr [SSGCtrl]
		push    eax
		push    ecx
		push    ReturnAddress
		jmp     InternalAdditionalTags

		#undef ReturnAddress
		#undef SSGCtrl
		#undef ParentStack
		#undef tmpS
		#undef LineS
	}
}

__declspec(naked) static void __stdcall InternalAdditionalTags(TSSGCtrl *SSGCtrl, TSSGSubject *parent, string *prefix, string *code)
{
	#define DEFINE_LABEL(label) static void __cdecl label()

	typedef void(__cdecl *lable_t)();

	#define LABEL(label)                          \
	    }                                         \
	}                                             \
	__declspec(naked) static void __cdecl label() \
	{                                             \
	    __asm                                     \
	    {

	DEFINE_LABEL(L10);
	DEFINE_LABEL(L14);
	DEFINE_LABEL(L16);
	DEFINE_LABEL(L18);

	static const lable_t JumpTable[] = {
		L10,
		L14,
		L16,
		L18,
	};

	__asm
	{
		#define SSGCtrl (esp +  4)
		#define parent  (esp +  8)
		#define prefix  (esp + 12)
		#define code    (esp + 16)

		mov     ecx, dword ptr [prefix]
		mov     eax, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
		sub     ecx, eax
		jbe     L1
		cmp     ecx, 5
		jb      L1
		mov     edx, dword ptr [eax]
		je      L2
		cmp     edx, _BSWAP32('[und')
		je      L4
		sub     ecx, 9
		jb      L1
		cmp     edx, _BSWAP32('[def')
		je      L7
		cmp     ecx, 12 - 9
		ja      L1
		jmp     dword ptr [JumpTable + ecx * 4]
	L1:
		ret     16

		align   16
	L2:
		mov     cl, byte ptr [eax + 4]
		xor     edx, _BSWAP32('[exp')
		xor     ecx, 'r'
		or      ecx, edx
		jnz     L3
		jmp     Attribute_expr
	L3:
		ret     16

		align   16
	L4:
		cmp     ecx, 8
		jb      L6
		mov     cx, word ptr [eax + 4]
		mov     dl, byte ptr [eax + 6]
		cmp     cx, _BSWAP16('ef')
		jne     L6
		cmp     dl, ' '
		je      L5
		cmp     dl, 09H
		jb      L6
		cmp     dl, 0DH
		ja      L6
	L5:
		jmp     Attribute_undef
	L6:
		ret     16

		align   16
	L7:
		mov     eax, dword ptr [eax + 4]
		cmp     eax, _BSWAP32('ine ')
		je      L8
		rol     eax, 8
		cmp     eax, _BSWAP32('\tine')
		jb      L9
		cmp     eax, _BSWAP32('\rine')
		ja      L9
	L8:
		jmp     Attribute_define
	L9:
		ret     16

	LABEL(L10)
		cmp     edx, _BSWAP32('[all')
		jne     L12
		mov     edx, dword ptr [eax + 4]
		mov     cl, byte ptr [eax + 8]
		xor     edx, _BSWAP32('ocat')
		xor     ecx, 'e'
		or      ecx, edx
		jnz     L11
		jmp     Attribute_allcate
	L11:
		ret     16

		align   16
	L12:
		mov     cl, byte ptr [eax + 8]
		mov     eax, dword ptr [eax + 4]
		xor     edx, _BSWAP32('[var')
		xor     eax, _BSWAP32('iabl')
		xor     ecx, 'e'
		or      eax, edx
		or      eax, ecx
		jnz     L13
		jmp     Attribute_variable_open
	L13:
		ret     16

	LABEL(L14)
		mov     cx, word ptr [eax + 8]
		mov     eax, dword ptr [eax + 4]
		xor     edx, _BSWAP32('[/va')
		xor     eax, _BSWAP32('riab')
		xor     ecx, _BSWAP16('le')
		or      eax, edx
		or      eax, ecx
		jnz     L15
		jmp     Attribute_variable_close
	L15:
		ret     16

	LABEL(L16)
		mov     ecx, dword ptr [eax + 4]
		mov     eax, dword ptr [eax + 8]
		xor     edx, _BSWAP32('[err')
		xor     ecx, _BSWAP32('or_s')
		xor     eax, _BSWAP32('kip\0')
		or      ecx, edx
		or      ecx, eax
		jnz     L17
		jmp     Attribute_error_skip_open
	L17:
		ret     16

	LABEL(L18)
		mov     ecx, dword ptr [eax + 4]
		mov     eax, dword ptr [eax + 8]
		xor     edx, _BSWAP32('[/er')
		xor     ecx, _BSWAP32('ror_')
		xor     eax, _BSWAP32('skip')
		or      ecx, edx
		or      ecx, eax
		jnz     L19
		jmp     Attribute_error_skip_close
	L19:
		ret     16

		#undef SSGCtrl
		#undef parent
		#undef prefix
		#undef code
	}
}
