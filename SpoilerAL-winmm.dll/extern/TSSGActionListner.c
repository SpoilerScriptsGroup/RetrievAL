#include "TSSGActionListner.h"

void(__cdecl *TSSGActionListner_OnProcessOpen)(LPVOID _this, TSSGSubject *SSGS, DWORD Mode) = (LPVOID)0x0052EB20;
void(__cdecl *TSSGActionListner_OnSubjectDisabled)(LPVOID _this, bcb6_std_string Code) = (LPVOID)0x0052EBC4;
void(__cdecl *TSSGActionListner_OnProcessOpenError)(LPVOID _this, TSSGSubject *SSGS) = (LPVOID)0x0052EE78;
void(__cdecl *TSSGActionListner_OnSubjectReadError)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052EF8C;
void(__cdecl *TSSGActionListner_OnSubjectWriteError)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F13C;
void(__cdecl *TSSGActionListner_OnSubjectReadSuccess)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F31C;
void(__cdecl *TSSGActionListner_OnSubjectWriteSuccess)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address) = (LPVOID)0x0052F6D0;
