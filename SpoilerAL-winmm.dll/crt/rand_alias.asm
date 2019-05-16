.386
.model flat, c

extrn internal_srand:proc
extrn internal_rand:proc

alias <srand> = <internal_srand>
alias <rand> = <internal_rand>

end
