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



### This Writeup is gonna be super short bcoz I kinda solved it in a unintended easy way



![start_meme](https://memegenerator.net/img/instances/65717385/okay-then-lets-begin.jpg)

Funny backstory is that this challenge was rated hard 

#### First Let's analyze the behaviour of the Program:-

1) It asks for a name and password..And then check the password with something and print Invalid password













