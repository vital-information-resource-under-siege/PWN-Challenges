#!/usr/bin/env python3

import codecs
def ror(n, c, bits=32):
    mask = (1 << bits) - 1
    return ((n >> c) | (n << (bits - c))) & mask

def rol(n, c, bits=32):
    return ror(n, bits - c, bits)


a = "2024"
key = 0x89f8db58
val = int("".join(f"{ord(c):02x}" for c in a[:4]),16) ^ key
val = rol(val,12)
key_arr = ['\x69\xc6\xcd\xb0','\x48\xec\x24\x25','\x53\x27\xf2\x67']
key_1 = int("".join(f"{ord(c):02x}" for c in key_arr[-1]),16)
xor_edx_rsp_plus_rcx =   val ^ key_1
temp1 = rol(key,0xd)
temp2 = key & 0xf
a_hex = int("".join(f"{ord(c):02x}" for c in a[:4]),16)
b_hex = int("".join(f"{ord(c):02x}" for c in key_arr[-2]),16)
interim_2 = (a_hex + b_hex)&(2**32 - 1)
ror_eax_7 = ror(int("".join(f"{ord(c):02x}" for c in a[:4]),16),7)
ror_edi_cl = ror(interim_2,temp2)
key_3 = int("".join(f"{ord(c):02x}" for c in key_arr[-3]),16)
xor_eax_ebx = ror_eax_7 ^ temp1
imul_edx_edi = (xor_edx_rsp_plus_rcx * ror_edi_cl)&0xffffffff
add_edx_rsp_plus_rcx = (imul_edx_edi + key_3)&0xffffffff
print(hex(xor_eax_ebx ^ add_edx_rsp_plus_rcx))
