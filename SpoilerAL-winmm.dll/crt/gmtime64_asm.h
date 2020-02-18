static void __cdecl _gmtime64_s_jan();
static void __cdecl _gmtime64_s_feb();
static void __cdecl _gmtime64_s_mar();
static void __cdecl _gmtime64_s_apr();
static void __cdecl _gmtime64_s_may();
static void __cdecl _gmtime64_s_jun();
static void __cdecl _gmtime64_s_jul();
static void __cdecl _gmtime64_s_aug();
static void __cdecl _gmtime64_s_sep();
static void __cdecl _gmtime64_s_oct();
static void __cdecl _gmtime64_s_nov();
static void __cdecl _gmtime64_s_dec();
static void __cdecl _gmtime64_s_next_month();

#define LABEL(name)                          \
    }                                        \
}                                            \
__declspec(naked) static void __cdecl name() \
{                                            \
    __asm                                    \
    {

__declspec(naked) errno_t __cdecl _gmtime64_s(struct tm *dest, const __time64_t *source)
{
	static const void(__cdecl *JumpTable[])() = {
		_gmtime64_s_jan,
		_gmtime64_s_feb,
		_gmtime64_s_mar,
		_gmtime64_s_apr,
		_gmtime64_s_may,
		_gmtime64_s_jun,
		_gmtime64_s_jul,
		_gmtime64_s_aug,
		_gmtime64_s_sep,
		_gmtime64_s_oct,
		_gmtime64_s_nov,
		_gmtime64_s_dec,
	};

	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define JAN_FEB     (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		mov     edx, dword ptr [dest]
		mov     ecx, dword ptr [source]
		test    edx, edx
		jz      L8
		test    ecx, ecx
		jz      L7
		mov     eax, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
		test    ecx, ecx
		jg      L1
		jl      _gmtime32_s
		test    eax, eax
		js      L2
		jmp     _gmtime32_s

	L1:
		cmp     ecx, 7
		jb      L2
		ja      L7
		cmp     eax, 9358E1CFH
		ja      L7
	L2:
		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebp, 72894AB7H
		mov     ebx, eax
		mul     ebp
		mov     esi, edx
		mov     eax, 0C22E4506H
		mul     ebx
		push    ebx
		mov     ebx, eax
		mov     eax, ecx
		mov     edi, edx
		mul     ebp
		mov     ebp, eax
		mov     eax, ecx
		mov     ecx, edx
		mov     edx, 0C22E4506H
		mul     edx
		add     ebp, esi
		adc     ecx, 0
		add     ebx, ebp
		adc     edi, 0
		mov     ebx, eax
		xor     eax, eax
		add     ecx, edi
		adc     eax, eax
		add     ebx, ecx
		adc     edx, eax
		pop     ecx
		shrd    ebx, edx, 16
		shr     edx, 16
		imul    eax, ebx, DAY_SEC
		sub     ecx, eax
		add     ebx, SINCE(1970) - SINCE(1600) - JAN_FEB
		adc     edx, 0
		mov     eax, 04444445H
		mov     edi, edx
		push    ebx
		mul     ecx
		mov     eax, 04444445H
		mov     esi, edx
		mul     edx
		mov     ebp, esi
		shl     ebp, 4
		mov     eax, edx
		shl     eax, 4
		sub     ebp, esi
		shl     ebp, 2
		sub     eax, edx
		shl     eax, 2
		sub     ecx, ebp
		mov     ebp, dword ptr [dest + 20]
		sub     esi, eax
		mov     dword ptr [ebp], ecx
		mov     dword ptr [ebp + 4], esi
		mov     dword ptr [ebp + 8], edx
		mov     eax, ebx
		mov     ebp, 0C8F862EDH
		mov     ecx, 396B06BCH
		mul     ebp
		mov     esi, edx
		mov     eax, ebx
		mul     ecx
		mov     ebx, eax
		mov     eax, edi
		push    edx
		mul     ebp
		mov     ebp, eax
		mov     ecx, edx
		mov     eax, edi
		mov     edx, 396B06BCH
		mul     edx
		add     ebp, esi
		mov     edi, eax
		adc     ecx, 0
		add     ebx, ebp
		pop     ebp
		pop     esi
		adc     ebp, 0
		xor     eax, eax
		add     ecx, ebp
		adc     eax, eax
		lea     ebp, [edi + ecx]
		adc     edx, eax
		mov     ecx, dword ptr [dest + 16]
		shrd    ebp, edx, 15
		imul    edi, ebp, 400
		imul    eax, ebp, YEAR400
		sub     esi, eax
		mov     eax, 24924925H
		lea     ebp, [esi + 2]
		mul     ebp
		add     ebp, edx
		lea     eax, [edx * 8]
		xor     ebx, ebx
		sub     ebp, eax
		cmp     esi, YEAR - JAN_FEB + 1
		mov     dword ptr [ecx + 24], ebp
		adc     ebx, 0
		jnz     L4
		mov     eax, 002CDB61H
		add     edi, 300
		sub     esi, YEAR100 * 3
		jae     L3
		sub     edi, 100
		add     esi, YEAR100
		jc      L3
		sub     edi, 100
		add     esi, YEAR100
		jc      L3
		sub     edi, 100
		add     esi, YEAR100
	L3:
		cmp     esi, YEAR - JAN_FEB + 1
		jb      L4
		mul     esi
		lea     edi, [edi + edx * 4]
		imul    eax, edx, YEAR4
		sub     esi, eax
		cmp     esi, YEAR - JAN_FEB + 1
		adc     ebx, 0
		jz      L5
	L4:
		lea     esi, [esi + ebx + JAN_FEB - 1]
		jmp     L6

	L5:
		add     esi, JAN_FEB - 1
		add     edi, 4
		sub     esi, YEAR * 4
		jae     L6
		dec     edi
		add     esi, YEAR
		jc      L6
		dec     edi
		add     esi, YEAR
		jc      L6
		dec     edi
		add     esi, YEAR
		jc      L6
		dec     edi
		add     esi, YEAR
	L6:
		sub     edi, 1900 - 1600
		mov     edx, esi
		shr     esi, 5
		xor     eax, eax
		mov     dword ptr [ecx + 16], esi
		mov     dword ptr [ecx + 20], edi
		mov     dword ptr [ecx + 28], edx
		mov     dword ptr [ecx + 32], eax
		jmp     dword ptr [JumpTable + esi * 4]

	L7:
		mov     dword ptr [edx     ], -1
		mov     dword ptr [edx +  4], -1
		mov     dword ptr [edx +  8], -1
		mov     dword ptr [edx + 12], -1
		mov     dword ptr [edx + 16], -1
		mov     dword ptr [edx + 20], -1
		mov     dword ptr [edx + 24], -1
		mov     dword ptr [edx + 28], -1
		mov     dword ptr [edx + 32], -1
	L8:
		mov     eax, EINVAL
		ret

	LABEL(_gmtime64_s_jan)
		sub     edx, 30
		ja      _gmtime64_s_next_month
		add     edx, 31
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_feb)
		sub     edx, 30
		add     ebx, 28
		sub     edx, ebx
		ja      _gmtime64_s_next_month
		add     edx, ebx
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_mar)
		sub     edx, ebx
		sub     edx, 89
		ja      _gmtime64_s_next_month
		add     edx, 31
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_apr)
		sub     edx, ebx
		sub     edx, 119
		ja      _gmtime64_s_next_month
		add     edx, 30
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_may)
		sub     edx, ebx
		sub     edx, 150
		ja      _gmtime64_s_next_month
		add     edx, 31
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_jun)
		sub     edx, ebx
		sub     edx, 180
		ja      _gmtime64_s_next_month
		add     edx, 30
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_jul)
		sub     edx, ebx
		sub     edx, 211
		ja      _gmtime64_s_next_month
		add     edx, 31
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_aug)
		sub     edx, ebx
		sub     edx, 242
		ja      _gmtime64_s_next_month
		add     edx, 31
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_sep)
		sub     edx, ebx
		sub     edx, 272
		ja      _gmtime64_s_next_month
		add     edx, 30
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_oct)
		sub     edx, ebx
		sub     edx, 303
		ja      _gmtime64_s_next_month
		add     edx, 31
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_nov)
		sub     edx, ebx
		sub     edx, 333
		ja      _gmtime64_s_next_month
		add     edx, 30
		mov     dword ptr [ecx + 12], edx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_dec)
		sub     edx, ebx
		pop     edi
		sub     edx, 333
		pop     esi
		mov     dword ptr [ecx + 12], edx
		pop     ebp
		pop     ebx
		ret

	LABEL(_gmtime64_s_next_month)
		inc     esi
		mov     dword ptr [ecx + 12], edx
		mov     dword ptr [ecx + 16], esi
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef dest
		#undef source
	}

	#undef DAY_SEC
	#undef SINCE
	#undef JAN_FEB
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

#undef LABEL
