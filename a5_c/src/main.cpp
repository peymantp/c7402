#include "include/stdinc.hpp"

const byte key[8] = {97, 98, 99, 100, 101, 102, 103, 104}; // aaaaaaaa
const byte msg[16] = {72,  97,  115, 116, 97, 76, 97, 86,
                      105, 115, 116, 97,  66, 97, 98, 121}; // HastaLaVista

int main() {
    printf("Key: ");
    print_section((byte *)key);
    printf("\n\n");

    debug_block((block *)msg);

    block *b = create_block_uchar((byte *)msg);
    // debug_block(b);

    for (int i = 1; i < 9; i++) {
        block* old = b;
        b = feistel_round(key, b, i);
        debug_block(b);
        free(old);
    }
    swap_sections(b);

    for (int i = 8; i > 0; i--) {
        block* old = b;
        b = feistel_round(key, b, i);
        debug_block(b);
        free(old);
    }
    swap_sections(b);
    debug_block(b);

    //printf("%lu\n", sizeof(msg2));

    // byte *msg2 = (byte *)malloc(73);
    // strncpy((char*)msg2, "HastaLaVistaBabyHowAreYaHastaLaVistaBabyHowAreYaHastaLaVistaBabyHowAreYa", 73);
    // int rem = sizeof(msg2) % sizeof(block);
    // float times = strlen((char*)msg2) / sizeof(block);
    // long block_count = 0;
    // printf("%lu\n", strlen((char*)msg2));
    // if (rem != 0) {
    //     if (times > 1) {
    //         long newsize = strlen((char*)msg2) + (times * sizeof(block)) + rem;
    //         block_count = newsize / sizeof(block);
    //         msg2 = (byte *)realloc(msg2, newsize);
    //         printf("%lu / %lu = %lu\n", newsize, sizeof(block), block_count);
    //     } else {
    //         //printf("%d\n", rem);
    //         //msg2 = (byte *)realloc(msg2, sizeof(msg2) + rem);
    //     }
    // }

    return 1;
}

byte* read_file(char* fname) {
    FILE* file = fopen(fname, "rb");
    
    return NULL;
}