#include "include/stdinc.hpp"
using namespace std; 
std::vector<char> read_file(char *fname);
void per_my_mute(byte_t* key);
block* feistel_forward(block* b, byte_t* key);
block* feistel_backward(block* b, byte_t* key);
block** pad_data(char* ipath, long *nblocks);
void encrpt(long nblocks, block** input, block** output, section* keys);
void decrypt(long nblocks, block** input, block** output, section* keys);
void write_file(block** output, long nblocks);
//const byte_t key[8] = {97, 98, 99, 100, 101, 102, 103, 104}; // aaaaaaaa
const byte_t msg[16] = {72,  97,  115, 116, 97, 76, 97, 86,
                      105, 115, 116, 97,  66, 97, 98, 121}; // HastaLaVista

int main(int argc, char** argv) {

    long nblocks = 0;
    
    byte_t key[8];
    char *ipath, *opath;
    int c ;
    block** blocks;

    bool do_encrypt = false;
    bool do_decrypt = false;
    block** modified_blocks;
    section* keys;

    while( ( c = getopt (argc, argv, "k:i:o:ed") ) != -1 )  {
        switch(c) {
            case 'k':
                if(optarg) {
                     strncpy((char*)key, optarg, 8);
                }else {
                    exit(0);
                }
                break;
            case 'i':
                if (optarg) {
                    ipath = optarg;
                }
            break;
            case 'o':
                if (optarg) {
                    opath = optarg;
                }
            break;
            case 'e':
                do_encrypt = true;
            break;
            case 'd':
                do_decrypt = true;
            break;
        }
    }

    if (do_encrypt && do_decrypt) {
        perror("You can't encrypt and decrypt the data at the same time!");
        exit(EXIT_FAILURE);
    } else if (do_encrypt == false && do_decrypt == false) {
        perror("You have to either choose to encryt or decrypt!");
        exit(EXIT_FAILURE);
    }

    blocks = pad_data(ipath, &nblocks);
    modified_blocks = (block**) malloc(sizeof(block) * nblocks);
    memset(modified_blocks, '\0', sizeof(block) * nblocks);
    
    //subkey generation
    keys = (section*) malloc(sizeof(section) * nblocks);
    memset(keys, '\0', sizeof(section) * nblocks);
    for (int i = 0; i < nblocks; i++){
        strncpy((char*)keys[i],(char*)key,8);
        cout << do_encrypt << " " << do_decrypt << endl;
        per_my_mute(key);
    }

    if (do_encrypt) {
        cout << "encrpting\n";
        encrpt(nblocks, blocks, modified_blocks, keys);
    }

    if (do_decrypt) {
        cout << "decrypting\n";
        decrypt(nblocks, blocks, modified_blocks, keys);
    }

    
    // encrpt(nblocks, blocks, modified_blocks, keys);
    // decrypt(nblocks, modified_blocks, blocks, keys);
    // for (int i = 0; i < nblocks; i++)
    //    debug_block(blocks[i]);
    write_file(modified_blocks,nblocks);
    return 1;
}

void encrpt(long nblocks, block** input, block** output, section* keys){
    for (int i = 0; i < nblocks; i++){
        output[i] = feistel_forward(input[i],keys[i]);
    }
    // for (int i = 0; i < nblocks; i++)
    //     debug_block(output[i]);
}

void decrypt(long nblocks, block** input, block** output, section* keys){
    for (int i = nblocks-1; i > -1; i--){
        output[i] = feistel_backward(input[i],keys[i]);
    }
    // for (int i = 0; i < nblocks; i++)
    //     debug_block(output[i]);  
}

block** pad_data(char* ipath, long *nblocks) {
    vector<char> contents = read_file(ipath);
    block** blocks;

    float n = (float)contents.size() / sizeof(block);

    *nblocks = ceil(n);
    
    long change = ((*nblocks * sizeof(block)) - contents.size());
    long newsize = contents.size() + change;
    
    for (int i = 0; i < change; i++) {
        contents.push_back('\0');
    }
    
    blocks = (block**) malloc(sizeof(block) * (*nblocks));
    memset(blocks, '\0', sizeof(block) * (*nblocks));

    for (int i = 0; i < contents.size(); i+=16) {
        //cout << i << " " << i+16 <<endl;
        auto begin = contents.begin() + i;
        auto end = contents.begin() + i + 16;
        vector<char> newVec(begin, end);
        //cout << newVec.data() << endl;
        //char bl[8];
        //strncpy(bl, newVec.data(), 8);
        char* data = newVec.data();
        blocks[i/16] = create_block_uchar((byte_t*)data);
        //strncpy((char*)(&blocks[i/16]), newVec.data(), 16);
    }
    return blocks;
}

vector<char> read_file(char *fname) {
    using std::vector;
    byte_t *output;
    std::ifstream file(fname, std::ios::binary);
    std::vector<char> fileContents((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
    return fileContents;
}

void per_my_mute(byte_t* key){
    int arr[8];
    for (int i = 0; i < 8; i++) {
        arr[i] = key[i];
    }
    next_permutation(arr, arr + 7);
    for (int i = 0; i < 8; i++) {
        key[i] = (byte_t)arr[i];
       
    }
    // printf("Key: ");
    // print_section((byte_t *)key);
    // printf("\n\n");
    return;
}

block* feistel_forward(block* b, byte_t* key) {
    for (int i = 1; i < 9; i++) {
        block* old = b;
        b = feistel_round(key, old, i);
        free(old);
    }
    swap_sections(b);
    block* ret = create_block_uchar_uchar(b->left, b->right);
    return ret;
}

block* feistel_backward(block* b, byte_t* key) {
    for (int i = 8; i > 0; i--) {
        block* old = b;
        b = feistel_round(key, old, i);
        free(old);
    }
    swap_sections(b);
    block* ret = create_block_uchar_uchar(b->left, b->right);
    return ret;
}

void write_file(block** output, long nblocks){
    ofstream myfile;
    myfile.open("output", std::ios::binary);
    for (long i = 0; i < nblocks; i++){
        cout << (char*)output[i];
        myfile << (char*)output[i];
    }
    myfile.close();
    return;
}