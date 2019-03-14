#ifndef FEISTEL_H
#define FEISTEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>

typedef unsigned char byte_t;
typedef byte_t section[8];

typedef struct block_t {
    section left;
    section right;
} block;

void swap_sections(block *b);
block *create_block_uchar_uchar(byte_t *left, byte_t *right);
block *create_block_uchar(byte_t *data);
byte_t *get_left_section(block *b);
byte_t *get_right_section(block *b);
void print_block(block *b);
void print_block_chars(block* b);
void print_section(section s);
void print_section_chars(section s);
void debug_block(block *b);
void xor_sections(section lhs, section rhs, section dest);
byte_t function(const byte_t key_part, byte_t section_part, int i);
block* feistel_round(const byte_t* key, block *b, int i);
long mpow(long base, long exp, long modulus);
void pad_message(byte_t* message, long size, long add);

#endif // FEISTEL_H
