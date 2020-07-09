//***
//ullrem.c - unsigned long remainder routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       defines the unsigned long remainder routine
//           __aullrem
//
//*******************************************************************************

#define LOWORD(x) [x]
#define HIWORD(x) [(x) + 4]

//***
//ullrem - unsigned long remainder
//
//Purpose:
//       Does a unsigned long remainder of the arguments.  Arguments are
//       not changed.
//
//Entry:
//       Arguments are passed on the stack:
//               1st pushed: divisor (QWORD)
//               2nd pushed: dividend (QWORD)
//
//Exit:
//       EDX:EAX contains the remainder (dividend%divisor)
//       NOTE: this routine removes the parameters from the stack.
//
//Uses:
//       ECX
//
//Exceptions:
//
//*******************************************************************************

#ifdef CFUNCTION
#define __cdecl __fastcall
#define _aullrem _ullrem
#endif

__declspec(naked) unsigned __int64 __cdecl _aullrem(unsigned __int64 dividend, unsigned __int64 divisor)
{
#if 0
	__asm
	{
		push    ebx

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a%b will
		// generate a call to ullrem(a, b)):
		//
		//               -----------------
		//               |               |
		//               |---------------|
		//               |               |
		//               |--divisor (b)--|
		//               |               |
		//               |---------------|
		//               |               |
		//               |--dividend (a)-|
		//               |               |
		//               |---------------|
		//               | return addr** |
		//               |---------------|
		//       ESP---->|      EBX      |
		//               -----------------
		//

		#define DVND (esp + 8)              // stack address of dividend (a)
		#define DVSR (esp + 16)             // stack address of divisor (b)

		// Now do the divide.  First look to see if the divisor is less than 4194304K.
		// If so, then we can use a simple algorithm with word divides, otherwise
		// things get a little more complex.
		//

		mov     eax, HIWORD(DVSR)           // check to see if divisor < 4194304K
		or      eax, eax
		jnz     short L1                    // nope, gotta do this the hard way
		mov     ecx, LOWORD(DVSR)           // load divisor
		mov     eax, HIWORD(DVND)           // load high word of dividend
		xor     edx, edx
		div     ecx                         // edx <- remainder, eax <- quotient
		mov     eax, LOWORD(DVND)           // edx:eax <- remainder:lo word of dividend
		div     ecx                         // edx <- final remainder
		mov     eax, edx                    // edx:eax <- remainder
		xor     edx, edx
		jmp     short L2                    // restore stack and return

		//
		// Here we do it the hard way.  Remember, eax contains DVSRHI
		//

	L1:
		mov     ecx, eax                    // ecx:ebx <- divisor
		mov     ebx, LOWORD(DVSR)
		mov     edx, HIWORD(DVND)           // edx:eax <- dividend
		mov     eax, LOWORD(DVND)
	L3:
		shr     ecx, 1                      // shift divisor right one bit; hi bit <- 0
		rcr     ebx, 1
		shr     edx, 1                      // shift dividend right one bit; hi bit <- 0
		rcr     eax, 1
		or      ecx, ecx
		jnz     short L3                    // loop until divisor < 4194304K
		div     ebx                         // now divide, ignore remainder

		//
		// We may be off by one, so to check, we will multiply the quotient
		// by the divisor and check the result against the orignal dividend
		// Note that we must also check for overflow, which can occur if the
		// dividend is close to 2**64 and the quotient is off by 1.
		//

		mov     ecx, eax                    // save a copy of quotient in ECX
		mul     dword ptr HIWORD(DVSR)
		xchg    ecx, eax                    // put partial product in ECX, get quotient in EAX
		mul     dword ptr LOWORD(DVSR)
		add     edx, ecx                    // EDX:EAX = QUOT * DVSR
		jc      short L4                    // carry means Quotient is off by 1

		//
		// do long compare here between original dividend and the result of the
		// multiply in edx:eax.  If original is larger or equal, we're ok, otherwise
		// subtract the original divisor from the result.
		//

		cmp     edx, HIWORD(DVND)           // compare hi words of result and original
		ja      short L4                    // if result > original, do subtract
		jb      short L5                    // if result < original, we're ok
		cmp     eax, LOWORD(DVND)           // hi words are equal, compare lo words
		jbe     short L5                    // if less or equal we're ok, else subtract
	L4:
		sub     eax, LOWORD(DVSR)           // subtract divisor from result
		sbb     edx, HIWORD(DVSR)
	L5:

		//
		// Calculate remainder by subtracting the result from the original dividend.
		// Since the result is already in a register, we will perform the subtract in
		// the opposite direction and negate the result to make it positive.
		//

		sub     eax, LOWORD(DVND)           // subtract original dividend from result
		sbb     edx, HIWORD(DVND)
		neg     edx                         // and negate it
		neg     eax
		sbb     edx, 0

		//
		// Just the cleanup left to do.  dx:ax contains the remainder.
		// Restore the saved registers and return.
		//

	L2:

		pop     ebx

		ret     16

		#undef DVND
		#undef DVSR
	}
#else
	__asm
	{
		push    ebx
		push    esi

		// Set up the local stack and save the index registers.  When this is done
		// the stack frame will look as follows (assuming that the expression a%b will
		// generate a call to ullrem(a, b)):
		//
		//               -----------------
		//               |               |
		//               |---------------|
		//               |               |
		//               |--divisor (b)--|
		//               |               |
		//               |---------------|
		//               |               |
		//               |--dividend (a)-|
		//               |               |
		//               |---------------|
		//               | return addr** |
		//               |---------------|
		//               |      EBX      |
		//               |---------------|
		//       ESP---->|      ESI      |
		//               -----------------
		//

		#define DVND (esp + 12)             // stack address of dividend (a)
		#define DVSR (esp + 20)             // stack address of divisor (b)

		mov     ebx, LOWORD(DVND)           // load dividend
		mov     eax, HIWORD(DVND)
		mov     esi, LOWORD(DVSR)           // load divisor
		mov     edx, HIWORD(DVSR)

		//
		// Now do the divide.  First look to see if the divisor is less than 4194304K.
		// If so, then we can use a simple algorithm with word divides, otherwise
		// things get a little more complex.
		//

		test    edx, edx                    // check to see if divisor < 4194304K
		jnz     hard                        // nope, gotta do this the hard way
		div     esi                         // EDX <- remainder, EAX <- quotient
		mov     eax, ebx                    // EDX:EAX <- remainder:lo word of dividend
		div     esi                         // EDX <- final remainder
		mov     eax, edx                    // EDX:EAX <- remainder
		xor     edx, edx
		jmp     epilogue                    // restore stack and return

		align   16
	hard:
		//
		// Here we do it the hard way.  Remember, EDX contains DVSRHI
		//

		lea     ecx, [edx + edx]
		jns     shift
		xor     ecx, ecx
		cmp     ebx, esi
		sbb     eax, edx
		mov     ebx, edx
		adc     ecx, -1
		mov     eax, LOWORD(DVND)
		and     ebx, ecx
		and     ecx, esi
		mov     edx, HIWORD(DVND)
		sub     eax, ecx
		sbb     edx, ebx                    // EDX:EAX = remainder
		jmp     epilogue                    // restore stack and return

		align   16
	shift:
		bsr     ecx, ecx
		shrd    esi, edx, cl
		mov     edx, eax                    // EDX:EAX <- dividend
		mov     eax, ebx
		shrd    eax, edx, cl
		shr     edx, cl
		div     esi                         // now divide, ignore remainder

		//
		// We may be off by one, so to check, we will multiply the quotient
		// by the divisor and check the result against the orignal dividend
		// Note that we must also check for overflow, which can occur if the
		// dividend is close to 2**64 and the quotient is off by 1.
		//

		mov     ecx, HIWORD(DVSR)
		mov     esi, eax                    // save a copy of quotient in ESI
		imul    ecx, eax                    // put partial product in ECX, get quotient in EAX
		mul     dword ptr LOWORD(DVSR)

		//
		// do long compare here between original dividend and the result of the
		// multiply in EDX:EAX.  If original is larger or equal, we're ok, otherwise
		// subtract the original divisor from the result.
		//

		add     ecx, edx                    // ECX:EAX = QUOT * DVSR
		mov     edx, HIWORD(DVND)
		sbb     esi, esi
		sub     ebx, eax                    // subtract product from dividend
		sbb     edx, ecx
		mov     eax, ebx                    // EDX:EAX = remainder
		sbb     esi, 0
		jz      epilogue                    // if above or equal we're ok, else add
		add     eax, LOWORD(DVSR)           // add divisor to remainder
		mov     ecx, HIWORD(DVSR)
		adc     edx, ecx

	epilogue:
		//
		// Just the cleanup left to do.  EDX:EAX contains the remainder.
		// Restore the saved registers and return.
		//

		pop     esi
		pop     ebx

		ret     16

		#undef DVND
		#undef DVSR
	}
#endif
}
