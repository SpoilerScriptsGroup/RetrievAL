.486
.xmm
.model flat, c

extrn _except1:proc

public _ftoui3
public _ftoul3
public _ftol3
public _ftol3_NaN
public _ftol3_work
public _ftol3_common
public _ftol3_arg_error
public _ftol3_except
public _dtoui3
public _dtoul3
public _dtol3
public _dtol3_NaN
public _dtol3_work
public _ultod3
public _ltod3

.code

_ftoui3:
	mov         ecx, 1
	movsd       xmm5, qword ptr [MaxInt64]
	jmp         _ftol3_NaN

_ftoul3:
	mov         ecx, 2
	movsd       xmm5, qword ptr [MaxUInt64]
	jmp         _ftol3_NaN

_ftol3:
	mov         ecx, 3
	movsd       xmm5, qword ptr [MaxInt64]

_ftol3_NaN:
	movd        eax, xmm0
	and         eax, 7FFFFFFFh
	cmp         eax, 7F800000h
	jnc         _ftol3_arg_error
	cvtss2sd    xmm0, xmm0

_ftol3_work:
	cmp         ecx, 2
	jnz         ftol3_non_ftoul3
	movsd       xmm2, qword ptr [MinSubInexact]
	comisd      xmm0, xmm2
	jbe         ftol3_non_ftoul3
	mov         edx, 16
	call        _ftol3_except

ftol3_non_ftoul3:
	comisd      xmm0, xmm5
	jnc         _ftol3_arg_error
	movsd       xmm6, qword ptr [MinInt64]
	comisd      xmm0, xmm6
	jc          _ftol3_arg_error
	movdqa      xmm1, xmm0
	psllq       xmm1, 1
	psrlq       xmm1, 1
	cmp         ecx, 1
	jz          _ftol3_common
	movsd       xmm2, qword ptr [MinFP32]
	comisd      xmm1, xmm2
	ja          _ftol3_common
	xorpd       xmm2, xmm2
	comisd      xmm1, xmm2
	jz          _ftol3_common
	push        eax
	fwait
	fnstcw      word ptr [esp]
	mov         ax, word ptr [esp]
	test        ax, 10h
	pop         eax
	jnz         _ftol3_common
	mov         edx, 2
	call        _ftol3_except
	xor         eax, eax
	mov         edx, 80000000h
	ret

_ftol3_common:
	xorpd       xmm6, xmm6
	comisd      xmm1, xmm6
	je          ftol3_exact
	movsd       xmm3, qword ptr [MantissaMask]
	movsd       xmm4, qword ptr [IntegerBit]
	movd        xmm5, dword ptr [i1075]
	movdqa      xmm6, xmm0
	cmpeqsd     xmm6, xmm1
	movdqa      xmm2, xmm1
	pand        xmm2, xmm3
	por         xmm2, xmm4
	psrlq       xmm1, 52
	movdqa      xmm4, xmm1
	psubq       xmm4, xmm5
	movdqa      xmm3, xmm2
	psllq       xmm3, xmm4
	movdqa      xmm4, xmm5
	psubq       xmm4, xmm1
	movdqa      xmm7, xmm2
	psrlq       xmm7, xmm4
	pcmpgtd     xmm1, xmm5
	punpckldq   xmm1, xmm1
	pand        xmm3, xmm1
	pandn       xmm1, xmm7
	por         xmm1, xmm3
	pxor        xmm5, xmm5
	psubq       xmm5, xmm1
	pand        xmm1, xmm6
	pandn       xmm6, xmm5
	por         xmm6, xmm1
	movd        eax, xmm4
	cmp         eax, 0
	jle         ftol3_exact
	psllq       xmm7, xmm4
	pxor        xmm2, xmm7
	movd        eax, xmm2
	psrldq      xmm2, 4
	movd        edx, xmm2
	or          eax, edx
	jz          ftol3_exact
	mov         edx, 16
	call        _ftol3_except

ftol3_zero:
ftol3_exact:
	movd        eax, xmm6
	psrldq      xmm6, 4
	movd        edx, xmm6
	ret
	
_ftol3_arg_error:
	mov         edx, 8
	call        _ftol3_except
	xor         eax, eax
	mov         edx, 80000000h
	ret

_ftol3_except:
	sub         esp, 32
	fwait
	fnstcw      word ptr [esp]
	movzx       eax, word ptr [esp]
	cmp         edx, 8
	jz          ftol3_eh_gen
	test        byte ptr [FpExcptTable - 1h + edx], al
	jnz         ftol3_eh_cont

ftol3_eh_gen:
	fldcw       word ptr [CWMask]
	mov         dword ptr [esp], edx
	mov         dword ptr [esp + 4h], 0
	movsd       qword ptr [esp + 8h], xmm0
	movsd       qword ptr [esp + 10h], xmm0
	mov         dword ptr [esp + 18h], eax
	call        _except1
	fstp        st(0)

ftol3_eh_cont:
	add         esp, 32
	ret

_dtoui3:
	mov         ecx, 1
	movsd       xmm5, qword ptr [MaxInt64]
	jmp         _dtol3_NaN

_dtoul3:
	mov         ecx, 4
	movsd       xmm5, qword ptr [MaxUInt64]
	jmp         _dtol3_NaN

_dtol3:
	mov         ecx, 5
	movsd       xmm5, qword ptr [MaxInt64]

_dtol3_NaN:
	movdqa      xmm1, xmm0
	psrldq      xmm1, 4
	movd        eax, xmm1
	and         eax, 7FFFFFFFh
	cmp         eax, 7FF00000h
	jnc         _ftol3_arg_error

_dtol3_work:
	cmp         ecx, 1
	je          _ftol3_work
	movdqa      xmm1, xmm0
	psllq       xmm1, 1
	psrlq       xmm1, 1
	movsd       xmm2, qword ptr [MaxFP32]
	comisd      xmm1, xmm2
	jbe         dtol3_underflow
	cmp         ecx, 4
	jz          dtoul3_overflow
	mov         edx, 1
	call        _ftol3_except
	mov         edx, 16
	call        _ftol3_except
	jmp         dtol3_underflow

dtoul3_overflow:
	mov         edx, 16
	call        _ftol3_except
	mov         edx, 1
	call        _ftol3_except

dtol3_underflow:
	movsd       xmm2, qword ptr [MinFP32]
	comisd      xmm1, xmm2
	jnc         dtol3_inexact
	xorpd       xmm2, xmm2
	comisd      xmm1, xmm2
	je          _ftol3_common
	mov         edx, 2
	call        _ftol3_except
	mov         edx, 16
	call        _ftol3_except
	jmp         _ftol3_common

dtol3_inexact:
	movdqa      xmm3, xmm0
	cmp         ecx, 4
	jnz         dtol3_inexact_1
	movsd       xmm2, qword ptr [MaxInt64]
	comisd      xmm3, xmm2
	jc          dtol3_inexact_1
	subpd       xmm3, xmm2

dtol3_inexact_1:
	movdqa      xmm4, xmm3
	psllq       xmm4, 35
	psrlq       xmm4, 35
	movd        eax, xmm4
	or          eax, eax
	jz          dtol3_exact
	mov         edx, 16
	call        _ftol3_except

dtol3_exact:
	comisd      xmm0, xmm5
	jnc         _ftol3_arg_error
	movsd       xmm2, qword ptr [MinInt64]
	comisd      xmm0, xmm2
	jc          _ftol3_arg_error
	jmp         _ftol3_common

align 8

_ultod3:
	xorps       xmm0, xmm0
	cvtsi2sd    xmm0, ecx
	shr         ecx, 31
	addsd       xmm0, qword ptr [Int32ToUInt32 + ecx * 8]
	test        edx, edx
	jz          ultod3_uint32
	xorps       xmm1, xmm1
	cvtsi2sd    xmm1, edx
	shr         edx, 31
	addsd       xmm1, qword ptr [Int32ToUInt32 + edx * 8]
	mulsd       xmm1, qword ptr [DP2to32]
	addsd       xmm0, xmm1

ultod3_uint32:
	ret

align 16

_ltod3:
	xorps       xmm1, xmm1
	cvtsi2sd    xmm1, edx
	xorps       xmm0, xmm0
	cvtsi2sd    xmm0, ecx
	shr         ecx, 31
	mulsd       xmm1, qword ptr [DP2to32]
	addsd       xmm0, qword ptr [Int32ToUInt32 + ecx * 8]
	addsd       xmm0, xmm1
	ret

.const

Int32ToUInt32   qword   0
DP2to32         qword   41F0000000000000h
MantissaMask    qword   000FFFFFFFFFFFFFh
IntegerBit      qword   0010000000000000h
SignBit64       qword   8000000000000000h
MinFP64         qword   0010000000000000h
MinInt64        qword   0C3E0000000000000h
MaxInt64        qword   43E0000000000000h
MaxUInt64       qword   43F0000000000000h
MaxFP32         qword   47EFFFFFF0000000h
MinFP32         qword   380FFFFFF0000000h
MinSubInexact   qword   4560000000000000h
FpExcptTable    byte    08h, 10h, 00h, 00h, 00h, 00h, 00h, 01h, 00h, 00h, 00h, 00h, 00h, 00h, 0F0h, 20h
i1075           dword   00000433h
i1087           dword   0000043Fh
x0800           dword   00000800h
x17FF           dword   000017FFh
CWMask          word    133Fh

end
