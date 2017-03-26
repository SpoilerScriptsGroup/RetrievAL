.486
.model flat

public @TDrawGrid_SetDefaultColWidth@8

.code

align 16

;EXTERN_C void __msfastcall TDrawGrid_SetDefaultColWidth(TDrawGrid *DrawGrid, const int ColWidth)
;{
;	DrawGrid->DefaultColWidth = ColWidth;
;}
@TDrawGrid_SetDefaultColWidth@8 proc near

	mov     eax, ecx
	mov     ecx, 0055902CH
	jmp     ecx

@TDrawGrid_SetDefaultColWidth@8 endp

end
