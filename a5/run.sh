#!/bin/sh


if sh compile.sh; then
    echo ""
    echo [*] compilation successful
    echo [*] running cbc
    
    rm outputs/*

    ./bin/feistel -e -m cbc -i src/feistel.c -o outputs/cipher_cbc -k abcd
    echo ""
    echo ""
    ./bin/feistel -d -m cbc -i outputs/cipher_cbc -o outputs/plain_cbc -k abcd
    
    echo ""
    echo [*] checking for differences
    if cmp outputs/plain_cbc src/feistel.c ; then
        echo ""
    else
        echo ""
    fi
    echo ""
    echo ""
    
    
    echo [*] running ecb
    ./bin/feistel -e -m ecb -i src/feistel.c -o outputs/cipher_ecb -k abcd
    echo ""
    echo ""
    ./bin/feistel -d -m ecb -i outputs/cipher_ecb -o outputs/plain_ecb -k abcd

    echo ""
    echo [*] checking for differences
    if cmp outputs/plain_ecb src/feistel.c ; then
        echo ""
    else
        echo ""
    fi
    echo ""
else
    echo [*] Compilation failed!
fi