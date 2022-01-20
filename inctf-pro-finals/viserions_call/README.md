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

#### First Let's analyse the behaviour of the Program:-

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

8. But the problem occurs at the the second input where the size of the variable is 20 and but read accepts 80 characters. Simply reading the decompiled code gave us the vulnerability.

   ![meme2](https://i.redd.it/agw69j6z2c131.jpg)

9. But here comes the tricky part ..

   #### First of all we have to bypass the stack canary without any format string bugs,then there is PIE enabled and finally the return address of the main function is libc_start_main+243 so we can't partial write to leak puts something like that.. Just after our big breakthrough we just stumbled into something that is super hard to bypass

   

   ![meme three](https://memegenerator.net/img/instances/82098423/what-if-i-told-you-there-is-no-way-out.jpg) 

10. But let me remind you guys..I did solve this challenge super easy right.. Maybe I found something that allow me bypass all the mitigations right. 

11. When I was skimming through the decompiled code,I saw something suspicious on the main function code at line 17 which holds the address of password view function which is present in stack.

    ![one more meme](https://i.imgflip.com/61y5bv.jpg)

    12. But function pointer may also be present before the input right which may render our overflow vulnerability useless . Let's check that..

        