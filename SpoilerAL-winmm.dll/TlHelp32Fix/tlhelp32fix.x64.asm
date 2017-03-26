includelib kernel32.lib

extern Process32First:proc
extern Process32Next:proc
extern Module32First:proc
extern Module32Next:proc

alias <Process32FirstA> = <Process32First>
alias <Process32NextA> = <Process32Next>
alias <Module32FirstA> = <Module32First>
alias <Module32NextA> = <Module32Next>

end
