# Source Generated with Decompyle++
# File: binary (Python 3.7)

import sys

def substitution(char):
    switch = {
        'a': 1,
        'b': 2,
        'c': 3,
        'd': 4,
        'e': 5,
        'f': 6,
        'g': 7,
        'h': 8,
        'i': 9,
        'j': 10,
        'k': 11,
        'l': 12,
        'm': 13,
        'n': 14,
        'o': 15,
        'p': 16,
        'q': 17,
        'r': 18,
        's': 19,
        't': 20,
        'u': 21,
        'v': 22,
        'w': 23,
        'x': 24,
        'y': 25,
        'z': 26,
        '{': 27,
        '}': 28,
        '_': 29,
        '!': 30 }
    return switch.get(char, 0)


def hash_(passw):
    hash = 1
    for i in range(len(passw)):
        hash += hash * 9 + 2 ** substitution(passw[i]) % 31
    print(hash)
    return hash

print('Enter the passcode :')
password = input()
passcode = 'alohamora!'
if password == 'alohamora!':
    print('Spells are not allowed.')
elif hash_(password) == hash_(passcode):
    print('Welcome to Hogwards, the flag is esCTF{dummy_flag}')
else:
    print('UnAuthorized')
