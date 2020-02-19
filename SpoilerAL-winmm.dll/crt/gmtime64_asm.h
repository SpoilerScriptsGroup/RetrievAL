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
	#define LEAP_DAY    (31 + 28)
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
		jz      L9
		test    ecx, ecx
		jz      L8
		mov     eax, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
		test    ecx, ecx
		jg      L2
		jl      L1
		test    eax, eax
		js      L3
		jmp     _gmtime32_s

	L1:
		inc     ecx
		jnz     L8
		cmp     edx, MIN_LOCAL_TIME
		jl      L8
		jmp     _gmtime32_s

	L2:
		cmp     ecx, 7
		jb      L3
		ja      L8
		cmp     eax, 0x9358E1CF
		ja      L8
	L3:
		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebp, 0x72894AB7
		mov     ebx, eax
		mul     ebp
		mov     esi, edx
		mov     eax, 0xC22E4506
		mul     ebx
		push    ebx
		mov     ebx, eax
		mov     eax, ecx
		mov     edi, edx
		mul     ebp
		mov     ebp, eax
		mov     eax, ecx
		mov     ecx, edx
		mov     edx, 0xC22E4506
		mul     edx
		add     ebx, ebp
		adc     ecx, 0
		mov     ebp, eax
		xor     eax, eax
		add     esi, ebx
		adc     edi, eax
		add     ecx, edi
		adc     eax, eax
		add     ebp, ecx
		adc     edx, eax
		pop     ebx
		shrd    ebp, edx, 16
		imul    eax, ebp, DAY_SEC
		sub     ebx, eax
		mov     eax, 0x04444445
		mul     ebx
		mov     eax, 0x04444445
		mov     ecx, edx
		mul     edx
		mov     esi, ecx
		add     ebp, SINCE(1970) - (SINCE(1600) + LEAP_DAY)
		shl     ecx, 4
		mov     eax, edx
		shl     eax, 4
		sub     ecx, esi
		shl     ecx, 2
		sub     eax, edx
		shl     eax, 2
		sub     ebx, ecx
		mov     ecx, dword ptr [dest + 16]
		sub     esi, eax
		mov     dword ptr [ecx], ebx
		mov     dword ptr [ecx + 4], esi
		mov     dword ptr [ecx + 8], edx
		mov     eax, 0x396B06BD
		mul     ebp
		shr     edx, 15
		mov     esi, ebp
		imul    eax, edx, YEAR400
		imul    edi, edx, 400
		sub     esi, eax
		mov     eax, 0x24924925
		lea     ebp, [esi + 2]
		mul     ebp
		add     ebp, edx
		lea     eax, [edx * 8]
		xor     ebx, ebx
		sub     ebp, eax
		cmp     esi, YEAR - LEAP_DAY + 1
		mov     dword ptr [ecx + 24], ebp
		adc     ebx, 0
		jnz     L5
		mov     eax, 0x002CDB61
		add     edi, 300
		sub     esi, YEAR100 * 3
		jae     L4
		sub     edi, 100
		add     esi, YEAR100
		jc      L4
		sub     edi, 100
		add     esi, YEAR100
		jc      L4
		sub     edi, 100
		add     esi, YEAR100
	L4:
		cmp     esi, YEAR - LEAP_DAY + 1
		jb      L5
		mul     esi
		lea     edi, [edi + edx * 4]
		imul    eax, edx, YEAR4
		sub     esi, eax
		cmp     esi, YEAR - LEAP_DAY + 1
		adc     ebx, 0
		jz      L6
	L5:
		lea     esi, [esi + ebx + LEAP_DAY - 1]
		jmp     L7

	L6:
		add     esi, LEAP_DAY - 1
		add     edi, 4
		sub     esi, YEAR * 4
		jae     L7
		dec     edi
		add     esi, YEAR
		jc      L7
		dec     edi
		add     esi, YEAR
		jc      L7
		dec     edi
		add     esi, YEAR
		jc      L7
		dec     edi
		add     esi, YEAR
	L7:
		sub     edi, 1900 - 1600
		mov     edx, esi
		shr     esi, 5
		xor     eax, eax
		mov     dword ptr [ecx + 16], esi
		mov     dword ptr [ecx + 20], edi
		mov     dword ptr [ecx + 28], edx
		mov     dword ptr [ecx + 32], eax
		jmp     dword ptr [JumpTable + esi * 4]

	L8:
		mov     dword ptr [edx     ], -1
		mov     dword ptr [edx +  4], -1
		mov     dword ptr [edx +  8], -1
		mov     dword ptr [edx + 12], -1
		mov     dword ptr [edx + 16], -1
		mov     dword ptr [edx + 20], -1
		mov     dword ptr [edx + 24], -1
		mov     dword ptr [edx + 28], -1
		mov     dword ptr [edx + 32], -1
	L9:
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
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

#undef LABEL
