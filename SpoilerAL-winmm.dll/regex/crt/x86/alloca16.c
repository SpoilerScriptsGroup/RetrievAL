//***
//chkstk.c - aligned C stack checking routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       Provides 16 and 8 bit aligned alloca routines.
//
//*******************************************************************************

extern void __cdecl _chkstk();

//***
// _alloca_probe_16, _alloca_probe_8 - align allocation to 16/8 byte boundary
//
//Purpose:
//       Adjust allocation size so the ESP returned from chkstk will be aligned
//       to 16/8 bit boundary. Call chkstk to do the real allocation.
//
//Entry:
//       EAX = size of local frame
//
//Exit:
//       Adjusted EAX.
//
//Uses:
//       EAX
//
//*******************************************************************************

__declspec(naked) void __cdecl _alloca_probe_16()
{
	__asm
	{
		                                // 16 byte aligned alloca
		push    ecx
		lea     ecx, [esp + 8]          // TOS before entering this function
		sub     ecx, eax                // New TOS
		and     ecx, (16 - 1)           // Distance from 16 bit align (align down)
		add     eax, ecx                // Increase allocation size
		sbb     ecx, ecx                // ecx = 0xFFFFFFFF if size wrapped around
		or      eax, ecx                // cap allocation size on wraparound
		pop     ecx                     // Restore ecx
		jmp     _chkstk
	}
}

__declspec(naked) void __cdecl _alloca_probe_8()
{
	__asm
	{
		                                // 8 byte aligned alloca
		push    ecx
		lea     ecx, [esp + 8]          // TOS before entering this function
		sub     ecx, eax                // New TOS
		and     ecx, (8 - 1)            // Distance from 8 bit align (align down)
		add     eax, ecx                // Increase allocation Size
		sbb     ecx, ecx                // ecx = 0xFFFFFFFF if size wrapped around
		or      eax, ecx                // cap allocation size on wraparound
		pop     ecx                     // Restore ecx
		jmp     _chkstk
	}
}
