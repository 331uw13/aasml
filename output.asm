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
# 'aaa'
	movl $5, -4(%rbp)
# 'num'
	movl $30, -8(%rbp)
	addl $1, -8(%rbp)
	movl -4(%rbp), %eax
	addl %eax, -8(%rbp)
# <return>
	movl -8(%rbp), %r8d
	popq %rbp
	ret
