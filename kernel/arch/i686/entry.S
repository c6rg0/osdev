// Defining the value of the stack pointer
.section .bss
.align 16
stack_bottom:
.skip 16384 // 16 KiB; ((2^10)x16)
stack_top:

// Entry point
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp

	// Call global constructors
	call _init

	// Transfer control to the main kernel
	call kernel_main

	// Hang if kernel_main unexpectedly returns
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
