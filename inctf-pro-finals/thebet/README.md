# Writeup for theBet Challenge

## Description of the challenge:-


<b>Welcome to the Bet! You are standing among the world's best lawyers, politicians, and criminals. You approach a young lawyer not more than 24 years of age. As you converse with him, you come to know about his reputation. The next moment you know, you find yourself debating which is better - Capital Punishment or Life Imprisonment. You are of the opinion that Capital Punishment is better as a quick death would mean less pain as compared to the life of solitude in the cell. Your friend believes otherwise. You both decide on writing down your views and handing them over to a third party, (a criminal) so as to decide who's opinion is correct. The criminal however states that the one who loses the debate shall either face Capital Punishment or Life Imprisonment depending on what the winning party chooses.</b>

#### Challenge files provided for the challenge is only the vulnerable binary

#### Checksec:-

| Mitigation | Stance         |
|------------|----------------|
|RELRO       | FULL RELRO     |
|Stack Canary| No Canary Found|
|NX          | NX Disabled    |
|PIE         | No PIE         |

### This is Writeup is gonna be totally in a way on how my thought process was during solving the challenge. 


![let's begin](https://i.imgflip.com/34ifo0.gif)

There are totally 4 user defined functions in this binary:-
`Main` functions which calls the functions `setup seccomp,Life Imprisonment and Capital Punishment` ..

The function `Life Imprisonment` and `Capital Punishment` both almost does the same functionality but with different puts statements..

The function `setup seccomp` sets up a seccomp that allows only a limited set of syscalls but the funny part is that it is placed at the end of the main function..

```
  char local_28 [32];
  
  puts("Describe your argument: ");
  sVar1 = read(0,local_28,100);
  ```
  
  This code reamins same in both the `Life imprisonment` and `Capital Punishment` function..
  
  The part where the problem in the code is you can see that the variable size is 32 but read is reading up to 100 characters..
  
  ## Boom!! Buffer Overflow

![buffer overflow](https://www.radiojitter.com/wp-content/uploads/otwpct/tmb/buffer-OF3_1543828870_940X300_c_c_0_0.png)

From the above image you can know about the buffer overflow attack.

Here see this below image where it get crashed when supplied high number of characters and also it tries to return to our address of choosing which is 8 A's.


![crash](https://github.com/vital-information-resource-under-siege/PWN-Challenges/blob/main/inctf-pro-finals/thebet/Writeup%20Images/Screenshot_2022-01-19_23-52-39.png)

```
  len = (int)sVar1;
  i = 0;
  do {
    if (len <= i) {
      return;
    }
    for (j = 0; j < 0xe; j = j + 1) {
      if (local_28[i] == bad_chars[j]) {
        puts("\nI\'m sorry you lost! You will be imprisoned for life.. :(\n");
                    /* WARNING: Subroutine does not return */
        exit(0);
      }
    }
    i = i + 1;
  } while( true );
}
```
The above code looks like its king something from the input. Every character from the input is checked with a global array of characters called bad chars if any character from that found out to be in input the program exists.. 

So the input has to be without those characters..
`` {0x50, 0x2e, 0xbb, 0xb0, 0x83, 0xf6, 0x10, 0xaa, 0xd2, 0x98, 0x99, 0x30, 0x31} ``

The saddest part is that `pop rdi` gadget which is essential for puts falls under the restricted chars..

As NX is disabled we have to write a shellcode maybe that too as seccomp is placed which we need to do a `OPEN READ WRITE SHELLCODE` that too with some restricted chars and the next part is only 100 characters is read..And a stack leak is not present .So we have to stack pivot in the main function and that is the place where the seccomp filter is added. 

```
 0003: 0x35 0x00 0x01 0x40000000  if (A < 0x40000000) goto 0005
 0004: 0x15 0x00 0x0d 0xffffffff  if (A != 0xffffffff) goto 0018
 0005: 0x15 0x0b 0x00 0x00000003  if (A == close) goto 0017
 0006: 0x15 0x0a 0x00 0x00000008  if (A == lseek) goto 0017
 0007: 0x15 0x09 0x00 0x0000000a  if (A == mprotect) goto 0017
 0008: 0x15 0x08 0x00 0x0000000c  if (A == brk) goto 0017
 0009: 0x15 0x07 0x00 0x00000014  if (A == writev) goto 0017
 0010: 0x15 0x06 0x00 0x0000003b  if (A == execve) goto 0017
 0011: 0x15 0x05 0x00 0x0000003c  if (A == exit) goto 0017
 0012: 0x15 0x04 0x00 0x0000003e  if (A == kill) goto 0017
 0013: 0x15 0x03 0x00 0x00000066  if (A == getuid) goto 0017
 0014: 0x15 0x02 0x00 0x00000068  if (A == getgid) goto 0017
 0015: 0x15 0x01 0x00 0x000000e7  if (A == exit_group) goto 0017
 0016: 0x15 0x00 0x01 0x00000142  if (A != execveat) goto 0018
```
Does this means we only need a shellcode to pwn the binary.. 

![sad life](https://i.imgflip.com/61u0iq.jpg)

I am not good at shellcoding so I was like completely pissed at myself that maybe I can't solve this challenge or have to spend a day to atleast complete this challenge as my shellcoding skill is so bad..

I was scratching my head and searching the net for some shellcodes that could work here..

![saw](https://memegenerator.net/img/instances/53645289/oh-wait-i-think-i-saw-something.jpg)

#### Suddenly!My eye caught onto something interesting..

![breakthrough](https://github.com/vital-information-resource-under-siege/PWN-Challenges/blob/main/inctf-pro-finals/thebet/Writeup%20Images/asd.png)

My input was 99 A's with a newline at the end which accounts to full 100.. 


From the above image you can see rdi is 0 ,rsi is still the same buffer and rdx is the number of characters read(i checked with different inputs it is not the old value but the size return value which gets passed to the int varaible)

The reason why I am saying about the registers is function arguement for the function is passed through these registers in 64 bit arch. 

``read(int fd, void *buf, size_t count);``

And funny part is that read function address does not have any restricted chars..

For read the first,second and third arguement is perfectly at sync to make a another read this time a unrestricted without bad chars but still under 100 length..
And also here we have bypassed the seccomp rules as the function never ran.

```
           main(void)

{
  char local_13;
  char local_12 [10];
  
  initialize();
  puts(
      "Welcome to the Bet! You are standing among the world\'s best lawyers, politicians and crimina ls. \n\nYou approach a young lawyer not more than 24 years of age. As you converse with him, y ou come to know about his reputation. The next moment you know, you find yourself debating whi ch is better - Capital Punishment or Life Imprisonment. \n\nYou are of the opinion that Capita l Punishment is better as a quick death would mean less pain as compared to life of solitude i n the cell. Your friend believes otherwise. \n\nYou both decide on writing down your views and  handing them over to a third party, (a criminal) so as to decide who\'s opinion is correct. \ n\nThe criminal however states that the one who loses the debate shall either face Capital Pun ishment or Life Imprisonment depending on what the winning party chooses. \n\nGive us your nam e: "
      );
  fgets(local_12,0xb,stdin);
  puts("\n1. Capital Punishment\n2. Life Imprisonment\nYour choice: ");
  __isoc99_scanf(&DAT_0040241a,&local_13);
  getchar();
  if (local_13 == '1') {
    Capital_Punishment(bad_chars);
  }
  else {
    if (local_13 != '2') {
      puts("That is not a valid choice\n");
                    /* WARNING: Subroutine does not return */
      exit(0);
    }
    Life_Imprisonment(bad_chars);
  }
  setup_seccomp();
  return 0;
}
```
#### From the above code which is the main function code we can see that only at the end of the main seccomp function is called to setup the seccomp 

### We just found a unintended way to solve this challenge without writing a shellcode..

![meme](https://i.imgflip.com/2l2aus.jpg)

## Exploit Plan:-

1)Select `Capital Punishment` or `Life Prisonment` both does the same work..

2)Overflow the buffer with 40 A's and tamper the saved return to read plt address and then pass another 52 A's

3)Then for the next forged read pass the simple rop payload to leak puts libc address to get the libc version from libc.blukat.me and download the file and also to find the libc range and return back to `Capital Punishment` or `Life Imprisonment` .

4)Repeat the same step but this time use the second read to return to `system('/bin/sh')`.

![meme](https://memegenerator.net/img/instances/64859062/hack-the-planet.jpg)


