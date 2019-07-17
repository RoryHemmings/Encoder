'''
ASCII to BIN
BIN to ASCII

Image to BIN (through actually image data)
BIN to Image

BIN to Base64
Base64 to BIN

ASCII to Base64
Base64 to ASCII

Image to Base64
Base64 to Image

Flag for Image: -i
Flag for ASCII: -t
Flag for Binary: -b
Flag for Base64: -b64
Flag for File input: -f
Flag for File output: -o
Flag for Spaces: -s
Flag for Help: -h
'''

import sys
import numpy
import cv2
import base64
import argparse

ASCII = {chr(i): i for i in range(129)}
RASCII = {i: chr(i) for i in range(129)}

b64chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
BASE64 = {b64chars[i]: i for i in range(len(b64chars))}
RBASE64 = {i: b64chars[i] for i in range(len(b64chars))}

help_message = 'Help\n\tBasic Usage: py encoder.py tb "hello"\n\tencodes "hello" into binary string\n\n\tpy encoder.py tb "hello" s - s adds spaces\n\tf <file path> - reads input from file\n\to <file path> - writes output to file\n\th - displays this message\n\n\tOptions:\n\t\ttb - ASCII to binary\n\t\tbt - binary to ASCII\n\t\tbb64 - binary to base64\n\t\tb64b - base64 to binary\n\t\ttb64 - ASCII to base64\n\t\tb64t - base64 to ASCII\n\t\tib - image to raw image binary\n\t\tbi - raw image binary to image (Not functional currently)\n\t\tib64 - image to base64 (intended for images but can be used for any file type), enter path of image as normal arg, dont use f flag for input\n\t\tb64i - base64 to image'

def split_by(s, num):
    ret = []
    j = 0
    for i in range(len(s)):
        if (i + 1) % num == 0:
            ret.append(s[j:i+1])
            j = i+1
    if j != len(s):
        ret.append(s[j:])
    return ret

def remove_spaces(n):
    n = list(n)
    for i in range(len(n)):
        if i >= len(n): break
        if n[i] == ' ':
            n.pop(i)
            if n != 0: i -= 1
    return ''.join(n)

def BIN_literal_to_string(n, length=8):
    b = str(n)[2:]
    while len(b) < length:
        b = '0' + b
    return b

def ASCII_to_BIN(n, space=False):
    ret = ''
    for s in n:
        try:
            i = ASCII[s]
        except KeyError:
            i = 106
        if not space:
            ret += BIN_literal_to_string(bin(i))
        else:
            ret += BIN_literal_to_string(bin(i)) + ' '
    return ret

def BIN_to_ASCII(n):
    ret = ''
    n = split_by(remove_spaces(n), 8)
    for e in n:
        i = int(e, base=2)
        try:
            ret += RASCII[i]
        except KeyError:
            ret += '?'
    return ret

#   Converts RAW data, probably not that usefull, doesn't differ between file types
#   Probably not useful
def Image_to_BIN(path, grayscale=False):
    ret = ''
    img = cv2.imread(path)
    for row in range(img.shape[0]):
        for col in range(img.shape[1]):
            for c in range(img.shape[2])[::-1]:
                ret += BIN_literal_to_string(bin(img[row, col, c]), 8)
    return ret

#   Impossible through raw data as far as i know
def BIN_to_IMAGE(n):
    return "Impossible through raw data as far as i know, I wrote everything in before writing the functions and i didnt want to remove it"

def BIN_to_Base64(n):
    ret = ''
    n = split_by(remove_spaces(n), 6)
    if len(n[len(n) - 1]) < 6:
        for i in range(6 - len(n[len(n) - 1])): n[len(n) - 1] += '0'
    for e in n:
        i = int(e, base=2)
        ret += RBASE64[i]
    while len(ret) % 4 != 0: ret += '='
    return ret

def Base64_to_BIN(n):
    ret = ''
    for e in n:
        if e == '=': break
        try:
            i = BASE64[e]
        except KeyError:
            print('base64 does not support a character used in your input (possibly \n')
            sys.exit(1)
        ret += BIN_literal_to_string(bin(i), 6)
    while len(ret) % 8 != 0: ret = ret[:len(ret) - 1]
    return ret

def ASCII_to_Base64(n):
    return BIN_to_Base64(ASCII_to_BIN(n))

def Base64_to_ASCII(n):
    return BIN_to_ASCII(Base64_to_BIN(n))

#   I could have used this library function for the other operations
#   but its more fun to actually convert it manually, It also allows me to convert to binary
def Image_to_Base64(path):
    with open(path, 'rb') as img:
        ret = str(base64.b64encode(img.read()))[2:-1]
        return ret

def Base64_to_Image(n):
    with open('save.png', 'wb') as out:
        out.write(base64.b64decode(n))
    return "file saved to save.png"

def handle_args():
    finshed = False
    output = ''
    spaces = False
    flag = ''
    in_ = False
    out_ = False
    in_path = 'in.txt'
    out_path = 'save.txt'
    type_index = 0
    if len(sys.argv) == 1: 
        print(help_message) 
        return
    for i in range(len(sys.argv)):
        try:
            f = sys.argv[i]
            if f == 'h':
                print(help_message)
                return
            elif f == 's':
                spaces = True
            elif f == 'f':
                in_path = sys.argv[i + 1]
                in_ = True
                i += 1
            elif f == 'o':
                out_path = sys.argv[i + 1]
                out_ = True
                i += 1
            elif f == 'tb' or f == 'bt' or f == 'bb64' or f == 'b64b' or f == 'tb64' or f == 'b64t' or f == 'ib' or f == 'bi' or f == 'ib64' or f == 'b64i':
                flag = f
                type_index = i
        except IndexError:
            print('Incorrect args')

    i = type_index
    arg = ''
    if not in_:
        arg = sys.argv[i + 1]
    else:
        f = open(in_path, 'r')
        for l in f.readlines():
            arg += l
    if flag == 'tb':
        output = ASCII_to_BIN(arg, spaces)
    elif flag == 'bt':
        output = BIN_to_ASCII(arg)
    elif flag == 'bb64':
        output = BIN_to_Base64(arg)
    elif flag == 'b64b':
        output = Base64_to_BIN(arg)
    elif flag == 'tb64':
        output = ASCII_to_Base64(arg)
    elif flag == 'b64t':
        output = Base64_to_ASCII(arg)
    elif flag == 'ib':
        output = Image_to_BIN(arg)
    elif flag == 'bi':
        output = BIN_to_IMAGE(arg)
    elif flag == 'ib64':
        output = Image_to_Base64(arg)
    elif flag == 'b64i':
        output = Base64_to_Image(arg)
    else:
        print('no type present')
    
    if not out_:
        print(output)
    else:
        out = open(out_path, 'w')
        out.write(output)

def main():
    handle_args()
    # 01101000 01100101 01101100 01101100 01101111 00100000 01101001 00100000 01100001 01101101 00100000 01100011 01101111 01101111 01101100
    # print(ASCII_to_BIN(str(sys.argv[1]), space=True))
    # print(split_by(remove_spaces(ASCII_to_BIN(str(sys.argv[1]), space=True)), 8))
    # print(BIN_to_ASCII(remove_spaces(ASCII_to_BIN(str(sys.argv[1]), space=True))))
    # print(BIN_to_ASCII(str(sys.argv[1])))
    # print(BIN_to_Base64(str(sys.argv[1])))
    # print(ASCII_to_Base64(str(sys.argv[1])))
    # print(ASCII_to_Base64(str(sys.argv[1])))
    # print(Base64_to_Image(str(sys.argv[1])))

if __name__ == '__main__':
    main()
