#pragma once

#include <windows.h>

EXTERN_C int __fastcall TListBox_GetItemIndex(LPVOID ListBox);
EXTERN_C int __fastcall TListBox_GetTopIndex(LPVOID ListBox);
EXTERN_C int __fastcall TListBox_GetItemHeight(LPVOID ListBox);
EXTERN_C int __fastcall TListBox_ItemAtPos(LPVOID ListBox, LPPOINT Pos, BOOLEAN Existing);
