.486
.model flat

public @TDrawGrid_SetDefaultRowHeight@8

.code

align 16

;EXTERN_C void __msfastcall TDrawGrid_SetDefaultRowHeight(TDrawGrid *DrawGrid, const int RowHeight)
;{
;	DrawGrid->DefaultRowHeight = RowHeight;
;}
@TDrawGrid_SetDefaultRowHeight@8 proc near

	mov     eax, ecx
	mov     ecx, 00559068H
	jmp     ecx

@TDrawGrid_SetDefaultRowHeight@8 endp

end
