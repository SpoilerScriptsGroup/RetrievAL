.386
.model flat, c

includelib kernel32.lib

extern Process32First@8:proc
extern Process32Next@8:proc
extern Module32First@8:proc
extern Module32Next@8:proc

alias <Process32FirstA@8> = <Process32First@8>
alias <Process32NextA@8> = <Process32Next@8>
alias <Module32FirstA@8> = <Module32First@8>
alias <Module32NextA@8> = <Module32Next@8>

end
