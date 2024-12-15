#!/usr/bin/env python3

import string
def ror(n, c, bits=32):
    mask = (1 << bits) - 1
    return ((n >> c) | (n << (bits - c))) & mask

def rol(n, c, bits=32):
    return ror(n, bits - c, bits)


values = string.printable
print(values)
probs = []
for i in values:
    for j in values:
        for k in values:
            for l in values:
                probs.append(i + j + k + l)
count = 0
for i in probs:
    key =  0x67739ff5 
    val = int("".join(f"{ord(c):02x}" for c in i[:4]),16) ^ key
    try:
        val = rol(val,4)
    except Exception:
        print(val)
        exit()
    key_arr = ['\xf6\xdf\x87\xc7','\x49\x84\x99\x86','\x1f\xca\x78\x08','\x69\x65\x6b\x2c']
    key_1 = int("".join(f"{ord(c):02x}" for c in key_arr[-1]),16)
    xor_edx_rsp_plus_rcx =   val ^ key_1
    temp1 = rol(key,0xd)
    temp2 = key & 0xf
    a_hex = int("".join(f"{ord(c):02x}" for c in i[:4]),16)
    b_hex = int("".join(f"{ord(c):02x}" for c in key_arr[-2]),16)
    interim_2 = (a_hex + b_hex)&(2**32 - 1)
    ror_eax_7 = ror(int("".join(f"{ord(c):02x}" for c in i[:4]),16),7)
    ror_edi_cl = ror(interim_2,temp2)
    key_3 = int("".join(f"{ord(c):02x}" for c in key_arr[-3]),16)
    xor_eax_ebx = ror_eax_7 ^ temp1
    imul_edx_edi = (xor_edx_rsp_plus_rcx * ror_edi_cl)&0xffffffff
    add_edx_rsp_plus_rcx = (imul_edx_edi + key_3)&0xffffffff
    result = xor_eax_ebx ^ add_edx_rsp_plus_rcx
    check = 0x8c9a5b38
    if(result == check):
        print(i)
        exit()
    count+=1
print("Exhausted")
