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

      

      This is an example of how a 32 bit runtime stack of a running process in which `main` called a  another function called `func` might look like.

      For our program in your imagination replace it with name `func` with name `vuln`  and also replace the variables according to our program.

      ![stack](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/second.png)

      So here can we can see that if we overflow the buffer at `vuln` function aka `func` in the above image can tamper all the values below in stack till the end of stack and obviously saved ebp on vuln,return address of vuln ,arguments of vuln function, main function local variables and its saved ebp,return address and also arguments to main.

   5. Let's see what will happen before and after we overflow the buffer with A's .

      #### Normal working-

      ![stack](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/third.png)

#### 			After Overflow:-

​			![fourth](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/r0pking/Writeup%20Images/fourth.png)



0x41414141 is not a valid memory region so it obviously crashes but what happens if we point to a valid memory region. We will hijack the flow of execution without crashing.





