.486
.model flat

public @TDrawGrid_SetTopRow@8

.code

align 16

;EXTERN_C void __msfastcall TDrawGrid_SetTopRow(TDrawGrid *DrawGrid, const int TopRow)
;{
;	DrawGrid->TopRow = TopRow;
;}
@TDrawGrid_SetTopRow@8 proc near

	mov     eax, ecx
	mov     ecx, 005593B8H
	jmp     ecx

@TDrawGrid_SetTopRow@8 endp

end
