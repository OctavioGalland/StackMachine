	# read number
	read
	push 0
	gt
	push 1
	sub
	push less
	jmpz # if number < 0, jump to less
	
	# number >= 0
greater:
	push 1
	write
	push 0
	push end
	jmpz

less:	push -1
	write

end:	off
