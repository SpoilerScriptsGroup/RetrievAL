static errno_t __cdecl internal_gmtime32();
static errno_t __cdecl internal_gmtime64();
static errno_t __cdecl internal_gmtime_less_than_400_years_left();

__declspec(naked) errno_t __cdecl _gmtime32_s(struct tm *dest, const __time32_t *source)
{
	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		mov     ecx, dword ptr [dest]
		mov     edx, dword ptr [source]
		test    ecx, ecx
		jz      L2
		test    edx, edx
		jz      L1
		mov     edx, dword ptr [edx]
		mov     eax, 0x88888889
		cmp     edx, MIN_LOCAL_TIME
		jge     internal_gmtime32
	L1:
		or      edx, -1
		mov     dword ptr [ecx     ], edx
		mov     dword ptr [ecx +  4], edx
		mov     dword ptr [ecx +  8], edx
		mov     dword ptr [ecx + 12], edx
		mov     dword ptr [ecx + 16], edx
		mov     dword ptr [ecx + 20], edx
		mov     dword ptr [ecx + 24], edx
		mov     dword ptr [ecx + 28], edx
		mov     dword ptr [ecx + 32], edx
	L2:
		mov     eax, EINVAL
		ret

		#undef dest
		#undef source
	}
}

__declspec(naked) errno_t __cdecl _gmtime64_s(struct tm *dest, const __time64_t *source)
{
	__asm
	{
		#define dest   (esp + 4)
		#define source (esp + 8)

		mov     edx, dword ptr [dest]
		mov     ecx, dword ptr [source]
		test    edx, edx
		jz      L5
		test    ecx, ecx
		jz      L4
		mov     eax, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
		test    ecx, ecx
		jg      L3
		jl      L1
		test    eax, eax
		jns     L2
		jmp     internal_gmtime64

	L1:
		inc     ecx
		jnz     L4
		cmp     eax, MIN_LOCAL_TIME
		jb      L4
	L2:
		mov     ecx, edx
		mov     edx, eax
		mov     eax, 0x88888889
		jmp     internal_gmtime32

	L3:
		cmp     ecx, 7
		jb      internal_gmtime64
		ja      L4
		cmp     eax, 0x9358E1CF
		jbe     internal_gmtime64
	L4:
		or      ecx, -1
		mov     dword ptr [edx     ], ecx
		mov     dword ptr [edx +  4], ecx
		mov     dword ptr [edx +  8], ecx
		mov     dword ptr [edx + 12], ecx
		mov     dword ptr [edx + 16], ecx
		mov     dword ptr [edx + 20], ecx
		mov     dword ptr [edx + 24], ecx
		mov     dword ptr [edx + 28], ecx
		mov     dword ptr [edx + 32], ecx
	L5:
		mov     eax, EINVAL
		ret

		#undef dest
		#undef source
	}
}

__declspec(naked) static errno_t __cdecl internal_gmtime32()
{
	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define LEAP_DAY    (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	__asm
	{
		#define dest         ecx
		#define time32       edx
		#define multiplicand eax

		push    ebx
		push    esi
		push    edi
		lea     edi, [edx + DAY_SEC]
		mul     edi
		shr     edx, 5
		mov     eax, 0x04444445
		mov     ebx, edx
		mov     esi, edx
		mul     edx
		shl     ebx, 4
		mov     eax, edx
		shl     eax, 4
		sub     ebx, esi
		shl     ebx, 2
		sub     eax, edx
		shl     eax, 2
		sub     edi, ebx
		sub     esi, eax
		mov     eax, 0x0AAAAAAB
		mov     ebx, edx
		mov     dword ptr [ecx], edi
		mul     edx
		mov     dword ptr [ecx + 4], esi
		lea     eax, [edx + edx * 2]
		shl     eax, 3
		lea     esi, [edx + SINCE(1970) - (SINCE(1600) + LEAP_DAY) - 1]
		sub     ebx, eax
		sub     esi, YEAR400
		mov     dword ptr [ecx + 8], ebx
		jb      L1
		mov     edi, 400
		jmp     internal_gmtime_less_than_400_years_left

	L1:
		add     esi, YEAR400
		xor     edi, edi
		jmp     internal_gmtime_less_than_400_years_left

		#undef dest
		#undef time32
		#undef multiplicand
	}

	#undef DAY_SEC
	#undef SINCE
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

__declspec(naked) static errno_t __cdecl internal_gmtime64()
{
	#define DAY_SEC     (60 * 60 * 24)
	#define SINCE(year) (((year) - 1) * 365 + ((year) - 1) / 4 - ((year) - 1) / 100 + ((year) - 1) / 400)
	#define LEAP_DAY    (31 + 28)
	#define YEAR        365
	#define YEAR4       (YEAR * 4 + 1)
	#define YEAR100     (YEAR4 * 25 - 1)
	#define YEAR400     (YEAR100 * 4 + 1)

	__asm
	{
		#define dest     edx
		#define time64lo eax
		#define time64hi ecx

		push    ebx
		push    esi
		push    edi
		push    edx
		mov     esi, 0x4506728A
		mov     ebx, eax
		mul     esi
		mov     eax, ecx
		mov     edi, edx
		mul     esi
		xor     esi, esi
		add     edi, eax
		adc     esi, edx
		mov     eax, 0x0000C22E
		mul     ebx
		add     edi, eax
		adc     esi, edx
		imul    eax, ecx, 0x0000C22E
		add     esi, eax
		imul    eax, esi, DAY_SEC
		sub     ebx, eax
		mov     eax, 0x04444445
		mul     ebx
		mov     eax, 0x04444445
		mov     ecx, edx
		mul     edx
		mov     edi, ecx
		add     esi, SINCE(1970) - (SINCE(1600) + LEAP_DAY)
		shl     ecx, 4
		mov     eax, edx
		shl     eax, 4
		sub     ecx, edi
		shl     ecx, 2
		sub     eax, edx
		shl     eax, 2
		sub     ebx, ecx
		pop     ecx
		sub     edi, eax
		mov     dword ptr [ecx], ebx
		mov     dword ptr [ecx + 4], edi
		mov     dword ptr [ecx + 8], edx
		xor     edi, edi
		sub     esi, YEAR400 * 2
		jae     L2
		add     esi, YEAR400
		jc      L1
		add     esi, YEAR400
		jmp     internal_gmtime_less_than_400_years_left

	L1:
		mov     edi, 400
		jmp     internal_gmtime_less_than_400_years_left

	L2:
		cmp     esi, YEAR400
		jae     L3
		mov     edi, 400 * 2
		jmp     internal_gmtime_less_than_400_years_left

	L3:
		sub     esi, YEAR400
		mov     edi, 400 * 3
		jmp     internal_gmtime_less_than_400_years_left

		#undef dest
		#undef time64lo
		#undef time64hi
	}

	#undef DAY_SEC
	#undef SINCE
	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
	#undef YEAR400
}

__declspec(naked) static errno_t __cdecl internal_gmtime_less_than_400_years_left()
{
	#define LEAP_DAY (31 + 28)
	#define YEAR     365
	#define YEAR4    (YEAR * 4 + 1)
	#define YEAR100  (YEAR4 * 25 - 1)

	static const uint32_t mon_yday[] = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };

	__asm
	{
		#define dest ecx
		#define year edi
		#define days esi

		mov     eax, 0x24924925
		lea     ebx, [esi + 2]
		mul     ebx
		mov     eax, edx
		add     edx, ebx
		shl     eax, 3
		xor     ebx, ebx
		sub     edx, eax
		mov     eax, 0x002CDB61
		mov     dword ptr [ecx + 24], edx
		cmp     esi, YEAR - LEAP_DAY + 1
		adc     ebx, ebx
		jnz     L5
		cmp     esi, YEAR100 * 2
		jae     L1
		sub     esi, YEAR100
		jb      L3
		add     edi, 100
		jmp     L4

	L1:
		sub     esi, YEAR100 * 3
		jb      L2
		add     edi, 300
		jmp     L4

	L2:
		add     edi, 200
	L3:
		add     esi, YEAR100
	L4:
		cmp     esi, YEAR - LEAP_DAY + 1
		jb      L5
		mul     esi
		lea     edi, [edi + edx * 4]
		imul    eax, edx, YEAR4
		sub     esi, eax
		cmp     esi, YEAR - LEAP_DAY + 1
		adc     ebx, ebx
		jz      L6
	L5:
		lea     esi, [esi + ebx + LEAP_DAY - 1]
		jmp     L11

	L6:
		cmp     esi, YEAR * 2 - LEAP_DAY + 1
		jae     L7
		sub     esi, YEAR - LEAP_DAY + 1
		jb      L10
		inc     edi
		jmp     L11

	L7:
		sub     esi, YEAR * 3 - LEAP_DAY + 1
		jae     L8
		add     edi, 2
		jmp     L10

	L8:
		sub     esi, YEAR
		jb      L9
		add     edi, 4
		jmp     L11

	L9:
		add     edi, 3
	L10:
		add     esi, YEAR
	L11:
		sub     edi, 1900 - 1600
		mov     edx, esi
		shr     esi, 5
		mov     dword ptr [ecx + 20], edi
		mov     dword ptr [ecx + 28], edx
		mov     dword ptr [ecx + 32], 0
		cmp     esi, 1
		je      L12
		sub     edx, ebx
		mov     ebx, dword ptr [mon_yday + esi * 4 + 4]
		cmp     edx, ebx
		ja      L13
		sub     edx, dword ptr [mon_yday + esi * 4]
		jmp     L14

	L12:
		sub     edx, 30
		add     ebx, 28
		cmp     edx, ebx
		jbe     L14
	L13:
		sub     edx, ebx
		inc     esi
	L14:
		mov     dword ptr [ecx + 12], edx
		mov     dword ptr [ecx + 16], esi
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef dest
		#undef year
		#undef days
	}

	#undef LEAP_DAY
	#undef YEAR
	#undef YEAR4
	#undef YEAR100
}
