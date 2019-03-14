#include "cbc.h"

void encrypt_cbc(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]) {
    uint32_t left, right;
    size_t ret;
    uint64_t sblock, sblock_prev = 0xFEEDFACE;
    while(!feof(infile)) {
        memset(&sblock,0,sizeof(sblock));
        ret = fread(&sblock,1,sizeof(sblock),infile);
        if(!ret) break;
        /* CBC */
        sblock ^= sblock_prev;
        left = (sblock>>32) & 0xFFFFFFFF;
        right = sblock & 0xFFFFFFFF;
        sblock = encrypt(left,right,ROUNDS,keys);
        sblock_prev = sblock;
        fwrite(&sblock,1,sizeof(sblock),outfile);
    }
}

void decrypt_cbc(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]) {
    int first = 1;
    uint32_t left, right;
    size_t ret;
    uint64_t sblock, sblock_prev, saved;
    while(!feof(infile)) {
        memset(&sblock,0,sizeof(sblock));
        ret = fread(&sblock,1,sizeof(sblock),infile);
        if(!ret) break;
        saved = sblock;
        left = (sblock>>32) & 0xFFFFFFFF;
        right = sblock & 0xFFFFFFFF;
        sblock = decrypt(left,right,ROUNDS,keys);
        if(first) {
            sblock ^= 0xFEEDFACE;
            first = 0;
        } else {
            sblock ^= sblock_prev;
        }
        /* CBC */
        sblock_prev = saved;
        fwrite(&sblock,1,sizeof(sblock),outfile);
    }
}
