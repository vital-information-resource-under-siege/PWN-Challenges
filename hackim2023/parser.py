#!/usr/bin/env python3



from pwn import *

p = process("./value")
vals = []
count = 256
for i in range(0,count*2):
    vals.append(int(p.recvline().strip().decode()))
low = []
for i in range(0,14,2):
    adder = vals[i] + vals[i+1]
    if(adder < 280):
        low.append(i)
print(low)
low = [0,4,6,8,12]
r = remote("52.59.124.14",10013) 
r.sendlineafter(b"Easy or Hard?",b"hard")
r.send(b"\n")
for i in range(0,8):
    if((i*2) in low):
        r.send(f"{vals[i*2]}\n")
        r.send(f"{vals[(i*2)+1]}\n")
    else:
        r.send(b"0\n")
        r.send(b"0\n")
r.interactive()
