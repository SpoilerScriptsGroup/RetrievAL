.386
.model flat, c

extrn IsBadReadPtrCompatible@8:proc
extrn IsBadWritePtrCompatible@8:proc

public _imp__IsBadReadPtr@8
public _imp__IsBadWritePtr@8

alias <IsBadReadPtr@8> = <IsBadReadPtrCompatible@8>
alias <IsBadWritePtr@8> = <IsBadWritePtrCompatible@8>

.const

_imp__IsBadReadPtr@8 dword IsBadReadPtrCompatible@8
_imp__IsBadWritePtr@8 dword IsBadWritePtrCompatible@8

end
