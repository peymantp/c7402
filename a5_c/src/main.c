#include "include/stdinc.h"

const byte key[8] = {97, 97, 97, 97, 97, 97, 97, 97}; // aaaaaaaa
const byte msg[16] = {72, 97, 115, 116, 97, 76, 97, 86, 105, 115, 116, 97, 66, 97, 98, 121}; // HastaLaVista



int main() {
    printf("Key: ");
    print_section((byte*)key);
    printf("\n\n");

    printf("Msg-> ");
    print_block((block*)msg);
    printf("Text-> ");
    print_block_chars((block*)msg);
    printf("\n");

    block *b = create_block_uchar((byte*) msg);
    //debug_block(b);

    for (int i = 1; i < 9; i++) {
        block* old = b;
        b = feistel_round(key, b, i);
        debug_block(b);
        free(old);
    }

    return 1;
}
