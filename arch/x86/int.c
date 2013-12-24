global	enable_int
global	disable_int

disable_int:
	cli
	ret
	
enable_int:
	sti
	ret
	