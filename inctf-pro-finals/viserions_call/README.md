# Writeup for the Viserion's Call Writeup 

## Description of the Challenge:-

<b> Give me the right password!</b>

<b>Note: The server is running on 20.04 </b>



#### Files provided for the challenge are the vulnerable binary , libc.so.6 and fake flag.txt file

### Checksec:-



| Mitigation   | Stance       |
| ------------ | ------------ |
| RELRO        | FULL RELRO   |
| Stack Canary | Canary Found |
| NX           | NX Enabled   |
| PIE          | PIE Enabled  |



### This Writeup is gonna be super short because I kinda solved it in a unintended easy way



![start_meme](https://memegenerator.net/img/instances/65717385/okay-then-lets-begin.jpg)

Funny backstory is that this challenge was rated hard 

#### First Let's analyse the behaviour of the Program and Plan our Exploit:-

1) First Let's do a test run.. It asks for a name and password..And then check the password with something and print Invalid password

   ![asd](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/viserions_call/writeup-images/first.png)





2. Let's see it in a ghidra...First of all,The user defined functions present in the binary.

![user defined functions](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/viserions_call/writeup-images/second.png)



3. Most of the functions in the above images are Compiler generated functions.

4. The user defined functions in the binary are `main,initialize,password_check,hidden_func,password_view and  backdoor`

5. After viewing the decompiled code,The initialize function was obvious setting up the alarm for the timeout for the program and setting up stream buffering .

6. Let's look at the main function decompiled code.

   ![third](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/viserions_call/writeup-images/third.png)

7. The first image that takes the name as input is safe 20 bytes of string using read syscall and not even a single byte overflow here.

8. But the problem occurs at the the second input where the size of the variable is 20 and but read accepts 80 characters. Simply by reading the decompiled code gave us the vulnerability.

   ![meme2](https://i.redd.it/agw69j6z2c131.jpg)

9. But here comes the tricky part ..

   #### First of all we have to bypass the stack canary without any format string bugs,then there is PIE enabled and finally the return address of the main function is libc_start_main+243 so we can't partial write to leak puts something like that.. Just after our big breakthrough we just stumbled into something that is super hard to bypass

   

   ![meme three](https://memegenerator.net/img/instances/82098423/what-if-i-told-you-there-is-no-way-out.jpg) 

10. But let me remind you guys..I did solve this challenge super easy right.. Maybe I found something that allow me bypass all the mitigations right. 

11. When I was skimming through the decompiled code,I saw something suspicious on the main function code at line 17 which holds the address of password view function which is present in stack and gets called at line 27.

    ![one more meme](https://i.imgflip.com/61y5bv.jpg)

    12. But function pointer may also be present before the input right which may render our overflow vulnerability useless . Let's check that..

        ![ss4](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/viserions_call/writeup-images/fourth.png)

Here we can see that our our name input is ajay. And our next input which is password is 8 B's above the function password which reads upto 80 characters can easily tamper the function pointer. Still we have a PIE there partial overwrite to what address tho.

14. Interestingly, When viewing through different function .. I remembered a function named backdoor. The naming of the function made me suspicious.

    So I went to see what this function does..After seeing the function.. My face totally went like this gif.

    ![mememe](https://c.tenor.com/8mnrYcikj3sAAAAC/meme-troll.gif)

15. Lemme post the decompiled code of that function here.

    ```c
    void backdoor(void)
    
    {
      FILE *__stream;
      long in_FS_OFFSET;
      char local_48 [56];
      undefined local_10;
      
      local_10 = *(undefined8 *)(in_FS_OFFSET + 0x28);
      __stream = fopen("flag.txt","rb");
      if (__stream != (FILE *)0x0) {
        fgets(local_48,0x3c,__stream);
        fclose(__stream);
      }
      puts(local_48);
                        /* WARNING: Subroutine does not return */
      exit(0);
    }
    ```

    This function holds the flag which is basically the win function for this program. That means we just have to perform a local buffer overflow.

16. But the PIE offset for the backdoor function is 0x1015df and for the password_view function the offset is 0x1014a2 . 

    The input is read using read so no terminators are placed after input. 

    ## As PIE work is similar to ASLR which places the binary base in random regions except the first nibble and the last three nibbles will always be same while in ASLR the first two nibbles and the last three nibbles will be same

    Let me write a test C program to print out the address to show how PIE and ASLR works.

    ```c
    #include<stdio.h>
    #include<stdlib.h>
    int main()
    {
    	printf("The address of main function is %p\n",main);
    	printf("The address of system function is %p\n",system);
    }
    ```

    ![output](https://raw.githubusercontent.com/vital-information-resource-under-siege/PWN-Challenges/main/inctf-pro-finals/viserions_call/writeup-images/fifth.png)

    Here we can see that for every execution the address of main function always changes but the last 3 nibbles and fist nibble . While the system function address changes by first two nibbles and the last three nibbles.

    So our partial two byte write still will not yield us a one shot solution but a 1/16 byte bruteforce will work .. 

17. But there is a problem in password check that if we don't give correct password the program exits which will render our overflow vulnerability useless.

18. Here is the code that exits if the check is failed in password check function.

    ```c
      local_20 = *(long *)(in_FS_OFFSET + 0x28);
      local_38 = 0x705f337275633373;
      local_30 = 0x64723077737334;
      local_28 = 0;
      local_3c = 0;
      while( true ) {
        sVar1 = strlen((char *)&local_38);
        if (sVar1 <= (ulong)(long)local_3c) break;
        if ((&stack0x0000001c)[local_3c] != *(char *)((long)&local_38 + (long)local_3c)) {
          puts("Invalid Password");
                        /* WARNING: Subroutine does not return */
          exit(0);
        }
        local_3c = local_3c + 1;
      }
      puts("Correct Password!");
    ```

    

    19. But it is actually easy, when u convert the hex to string the password is found out to be `s3cur3_p4ssw0rd`.

    20. But think about it,it checks for the string `s3cur3_p4ssw0rd` but our password also needs to have the partial two byte write payload.

    21. But strcmp only checks until null byte tho. While read does not have any null byte terminator like strcpy.

    22. The Exploit source code for the challenge is:-

        ```python
        #!/usr/bin/env python3
        
        
        
        from pwn import *
        from time import sleep
        
        e = ELF("./viserions_call")
        #r = process("./viserions_call")
        r = remote("gc1.eng.run",32680)
        r.sendline(b"A" * 10)
        sleep(0.5)
        r.send(b"s3cur3_p4ssw0rd\x00\x00\x00\x00\x00" + p16(0x15df))
        r.interactive()
        
        ```

        This Exploit works in a possibility  of 1/16.

        ## Me after completing this challenge in a super easy way with all mitigations present with difficulty stated as hard

        ![final meme](https://i.kym-cdn.com/photos/images/newsfeed/002/008/781/65d.png)

### Final note: Author said the intended solution was to use vsyscalls and finally use a partial one byte overwrite to solve this challenge.

