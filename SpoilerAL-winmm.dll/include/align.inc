$align macro number:req, value:=<0CCh>
	local length
	length = ($ - @CurSeg) mod number
	if length ne 0
		length = number - length
		db length dup (value)
	endif
endm
