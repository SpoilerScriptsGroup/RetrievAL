$align macro number:req, value:=<0CCh>
	db (number - ($ - @CurSeg) mod number) mod number dup (value)
endm
