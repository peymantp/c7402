#!/bin/python3
import sys
import argparse
import os

ROUNDS = 8
#key = b'qwertyui'
#key = b'\x19\x13\t\x19\x1cY\x1a\n'


def generate_subkey(left, key):
    new_key = []
    for i in range(len(left)):
        #print(left[i], key[i], left[i] ^ key[i])
        new_key.append(left[i] ^ key[i])
    return bytes(new_key)


def padding(x):
    padding = 16 - ((len(x) % 16 != 0) * (len(x) % 16))
    return x + bytes([padding]) * padding

# remove and check the padding


def strip(x):
    print(x)
    for i in range(x[-1]):
        if x[-(i + 1)] != x[-1]:
            raise ValueError('Input is not padded or padding is corrupt')
    return x[:-x[-1]]


def f(x, i, k):
    return pow((k * i * 2), x) % 15


def function(i, subkey, right):
    output = []
    for j in range(len(subkey)):
        #print(f(right[j] , i, subkey[j]))
        output.append(f(right[j], i, subkey[j]))
    # for d in right:
    #    output.append(f(d, i))
    return output


def round(i, k, L: bytes, R: bytes):
    temp = []
    f_right = function(i, k, R)
    for j in range(len(L)):
        temp.append(f_right[j] ^ L[j])
    return R, bytes(temp)
    # return R, [a ^ b for (a,b) in zip(L, function(i, k, R))]


def process_block(B: bytes, rounds, init_key: bytes):
    # Split the block
    L, R = bytes(B[:8]), bytes(B[8:])
    current_key = init_key
    for j in rounds:
        L, R = round(j, current_key, L, R)
        current_key = generate_subkey(L, current_key)

    print("Key: ", current_key)
    return R + L


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='COMP 7402 Assignment 4')
    parser.add_argument("-e", dest='e', action='store_true',
                        help="Encode the file")
    parser.add_argument("-d", dest='d', action='store_true',
                        help="Decode the file")
    parser.add_argument(dest='input_path',
                        help="Path of the file you want to encrypt or decrypt")
    parser.add_argument(dest='output_path',
                        help="Output path of the decrypted or encrypted file")

    args = parser.parse_args()

    if(args.e):
        P = padding(bytearray(open(args.input_path, 'rb').read()))
        if len(P) == 0:
            print("Input has to contain data")
            exit()
        # i is block num
        for i in range(int(len(P) / 16)):
            start_block = i * 16
            end_block = start_block + 16
            # Grab the block
            B = P[start_block: end_block]
            B = process_block(B, range(1, ROUNDS), key)
            # Write the block back
            P[start_block: end_block] = B
        with open(args.output_path, 'wb') as out:
            out.write(P)
    elif (args.d):
        P = bytearray(open(args.input_path, 'rb').read())
        if len(P) % 16 != 0:
            raise ValueError(
                'Ciphertext is not a valid length, it must be corrupted')
        # i is block num
        cipher = [bytes(P[i:i+16]) for i in range(0, len(P), 16)]
        for i in reversed(range(int(len(P)/16))):
            #start_block = i * 16
            #end_block = start_block + 16
            # Grab the block
            #B = P[start_block : end_block]
            print(cipher[i])
            # print(len(cipher[i]))
            B = process_block(cipher[i], reversed(range(1, ROUNDS)), key)
            # Write the block back
            #P[start_block : end_block] = B
            cipher[i] = B

        #P = strip(P)
        output = b''
        for block in cipher:
            output += block

        with open(args.output_path, 'wb') as out:
            out.write(output)
    else:
        print("unknown directive {}".format(sys.argv[1]))
