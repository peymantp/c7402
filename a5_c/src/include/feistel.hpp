#ifndef FEISTEL_H
#define FEISTEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char byte;
typedef byte section[8];

typedef struct block_t {
    section left;
    section right;
} block;

void swap_sections(block *b);
block *create_block_uchar_uchar(byte *left, byte *right);
block *create_block_uchar(byte *data);
byte *get_left_section(block *b);
byte *get_right_section(block *b);
void print_block(block *b);
void print_block_chars(block* b);
void print_section(section s);
void print_section_chars(section s);
void debug_block(block *b);
void xor_sections(section lhs, section rhs, section dest);
byte function(const byte key_part, byte section_part, int i);
block* feistel_round(const byte* key, block *b, int i);
long mpow(long base, long exp, long modulus);
void pad_message(byte* message, long size, long add);

#endif // FEISTEL_H
