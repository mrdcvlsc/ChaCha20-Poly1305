#include <iostream>
#include "../ChaCha20.hpp"

// ChaCha State Comparison
bool CompareState(unsigned int *A, unsigned int *B){
    for(size_t i=0; i<16; ++i)
        if(A[i] != B[i])
            return false;
    return true;
}

int main()
{
    unsigned char key[32] = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
        0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f
    };

    unsigned char nonce[12] = {
        0x00,0x00,0x00,0x09,0x00,0x00,
        0x00,0x4a,0x00,0x00,0x00,0x00
    };

    unsigned int correct_output_state[32] = {
        0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3,
        0xc7f4d1c7, 0x0368c033, 0x9aaa2204, 0x4e6cd4c3,
        0x466482d2, 0x09aa9f07, 0x05d7c214, 0xa2028bd9,
        0xd19c12b5, 0xb94e16de, 0xe883d0cb, 0x4e3c50a2
    };
    

    unsigned int *initial_state = __internal_chacha20::initialize_chacha_state((unsigned int*)key,1,(unsigned int*)nonce);
    unsigned int* output_state = __internal_chacha20::chacha20_block(initial_state,(unsigned int*)key,1,(unsigned int*)nonce);

    bool passed = CompareState(correct_output_state,output_state);

    delete [] initial_state;
    delete [] output_state;

    if(passed) {
        std::cout << "BlockFunctionTest : PASSED\n";
        return 0;
    }
    else {
        std::cout << "BlockFunctionTest : FAILED\n";
        return 1;
    }
}