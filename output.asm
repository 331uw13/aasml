.global _start
.text
_start:
	call entry
	movq $60, %rax
	movq %r8, %rdi
	syscall


entry:
	pushq %rbp
	movq %rsp, %rbp
	movl $30, -4(%rbp)
	popq %rbp
	ret
