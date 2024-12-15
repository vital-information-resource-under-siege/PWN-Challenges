#!/usr/bin/env python3

import codecs
def ror(n, c, bits=32):
    mask = (1 << bits) - 1
    return ((n >> c) | (n << (bits - c))) & mask

def rol(n, c, bits=32):
    return ror(n, bits - c, bits)


a = "2024"
key = 0x7d7a7a0f
val = int("".join(f"{ord(c):02x}" for c in a[:4]),16) ^ key
val = rol(val,12)
key_arr = ['\x6b\x12\x30\x7a','\x7e\xa4\xa9\x0c','\xbc\x12\x7a\x24','\xa4\x44\xa0\x43','\x47\x4e\x7b\x0d','\x9b\x2f\x20\x10','\x20\xa3\x0b\x61','\x17\xd2\x8e\x18','\x31\x3c\xbe\xab','\x55\xe1\x78\xac','\xb9\x7a\xc0\x6d','\x84\xe9\xb7\x7d','\x83\x5b\xd9\x8a','\x63\xad\x94\x43','\x5f\xb3\x5d\xfd','\x98\x30\x84\xd1']
key_1 = int("".join(f"{ord(c):02x}" for c in key_arr[-13]),16)
xor_edx_rsp_plus_rcx =   val ^ key_1
temp1 = rol(key,0xd)
temp2 = key & 0xf
a_hex = int("".join(f"{ord(c):02x}" for c in a[:4]),16)
b_hex = int("".join(f"{ord(c):02x}" for c in key_arr[-14]),16)
interim_2 = (a_hex + b_hex)&(2**32 - 1)
ror_eax_7 = ror(int("".join(f"{ord(c):02x}" for c in a[:4]),16),7)
ror_edi_cl = ror(interim_2,temp2)
key_3 = int("".join(f"{ord(c):02x}" for c in key_arr[-15]),16)
xor_eax_ebx = ror_eax_7 ^ temp1
imul_edx_edi = (xor_edx_rsp_plus_rcx * ror_edi_cl)&0xffffffff
add_edx_rsp_plus_rcx = (imul_edx_edi + key_3)&0xffffffff
print(hex(xor_eax_ebx ^ add_edx_rsp_plus_rcx))
