
__declspec(naked) unsigned long __fastcall TSSGCtrl_ReadSSRFile_DestReserve(int signedLoop)
{
	__asm
	{
		#define signedLoop ecx
		#define Step       (ebp - 0F8H)

		bt      signedLoop, 0
		jc      SIGNED
		xor     edx, edx
		div     dword ptr [Step]
		ret

		align   16
	SIGNED:
		cdq
		idiv    dword ptr [Step]
		ret

		#undef signedLoop
		#undef Step
	}
}
