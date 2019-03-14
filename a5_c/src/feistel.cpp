#include "include/feistel.hpp"

void swap_sections(block* b) {
    section tmp;
    strncpy((char*) tmp, (char*)b->left, 8);
    //printf("Saving left in tmp: %s\n", (char*)tmp);
    strncpy((char*) b->left, (char*)b->right, 8);
    //printf("Saving right in left: %s\n", (char*)b->right);
    strncpy((char*) b->right, (char*)tmp, 8);
    //printf("Saving tmp in right: %s\n", (char*)b->right);
}

block* create_block_uchar_uchar(byte* left, byte* right) {
    block *out = (block*)malloc(sizeof(block));
    memset(out, '\0', 16);
    strncpy((char*)out->left, (char*)left, 8);
    strncpy((char*)out->right, (char*)right, 8);
    return out;
}

block* create_block_uchar(byte* data) {
    block *out = (block*)malloc(sizeof(block));
    memset(out, '\0', 16);
    strncpy((char*)out, (char*)data, 16);
    return out;
}

byte* get_left_section(block* b) {
    byte* section = (byte*)malloc(8);
    strncpy((char*)section, (char*)b->left, 8);
    return section;
}

byte* get_right_section(block* b) {
    byte* section = (byte*)malloc(8);
    strncpy((char*)section, (char*)b->right, 8);
    return section;
}

void print_block(block* b) {
    printf("Left: ");
    print_section(b->left);
    printf("\n       Right:");
    print_section(b->right);
    printf("\n");
}

void print_section(section s) {
    printf("%d %d %d %d %d %d %d %d", s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
}

void print_block_chars(block* b) {
    printf("Left: ");
    print_section_chars(b->left);
    printf("\n       Right:");
    print_section_chars(b->right);
    printf("\n");
}

void print_section_chars(section s) {
    printf("%c %c %c %c %c %c %c %c", s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
}

void debug_block(block *b) {
    printf("Vals-> ");
    print_block(b);
    printf("Text-> ");
    print_block_chars(b);
    printf("\n");
}

void xor_sections(section lhs, section rhs, section dest) {
    for (char i = 0; i < 8; i++) {
        dest[i] = lhs[i] ^ rhs[i];
    }
}

byte function(const byte key_part, byte section_part, int i) {
    //byte res = (byte) pow(key_part * i * 2, section_part) % 15;
    return (byte) mpow(key_part * 1 * 2, section_part, 49);
}

block* feistel_round(const byte* key, block *b, int i) {
    byte* left = get_left_section(b);
    byte* right = get_right_section(b);
    for(byte j = 0; j < 8; j++) {
        byte  resf = function(key[j], right[j], i);
        left[j] = left[j] ^ resf;
    }
    block* output = create_block_uchar_uchar(right, left);
    free(left);
    free(right);
    return output;
}

long mpow(long base, long exp, long modulus) {
  base %= modulus;
  long result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return result;
}


void permute(const byte* data, byte* output) {
    
}

void generate_subkey(const byte* key) {

}