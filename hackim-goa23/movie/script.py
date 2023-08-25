#!/usr/bin/env python3


from z3 import *
import string

a1 = [BitVec(f"flag_{i}",32)for i in range(24)]
s = Solver()
s.add(a1[0] == ord("E"))
s.add(a1[1] == ord("N"))
s.add(a1[2] == ord("O"))
s.add(a1[3] == ord("{"))
s.add(a1[23] == ord("}"))
s.add(a1[12] == ord("A"))
s.add(a1[13] == ord("N"))
s.add(a1[14] == ord("D"))
s.add(a1[15] == ord("_"))
s.add(a1[16] == ord("M"))
s.add(a1[17] == ord("r"))
s.add(a1[18] == ord("R"))
s.add(a1[19] == ord("o"))
s.add(a1[20] == ord("b"))
s.add(a1[21] == ord("0"))
s.add(a1[22] == ord("t"))
s.add(a1[23] == ord("}"))
s.add(a1[11] == ord("_"))
s.add(a1[4] == ord("H"))
sum_val = 0
mul_val = 1
ccheck = "ENO{*******_AND_MrRob0t}"
for i in range(4,12):
    sum_val+=a1[i] ^ ord(ccheck[11+i])
    mul_val*=a1[i] ^ ord(ccheck[11+i])
s.add(sum_val == 500)
s.add(mul_val == 0x3a49d503c4c0)
val = string.digits + string.ascii_uppercase
for i in range(5,11):
    for j in range(0,127):
        if(chr(j)  not in val):
            s.add(a1[i] != j)
for i in range(5,11):
    s.add(a1[i] >= 0x30)
    s.add(a1[i] <= 0x5a)
for i in range(1,10):
    print(s.check())
    model = s.model() 
    flag = ''.join([chr(int(str(model[a1[i]]))) for i in range(len(model))])
    print(flag)
