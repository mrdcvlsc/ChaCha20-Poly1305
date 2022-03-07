#include <iostream>
#include <cstring>
#include "../ChaCha20.hpp"

using namespace std;

// ChaCha State Comparison
bool CompareState(unsigned int *A, unsigned int *B){
    for(size_t i=0; i<16; ++i)
        if(A[i] != B[i])
            return false;
    return true;
}

int main(){

    unsigned int stack_state[4*4] = {
        0x879531e0, 0xc5ecf37d, 0x516461b1, 0xc9a62f8a,
        0x44c20ef3, 0x3390af7f, 0xd9fc690b, 0x2a5f714c,
        0x53372767, 0xb00a5631, 0x974c541a, 0x359e9963,
        0x5c971061, 0x3d631689, 0x2098d9d6, 0x91dbd320,
    };

    unsigned int *heap_state = new unsigned int[16];

    for(size_t i=0; i<(4*4); ++i)
        heap_state[i] = stack_state[i];

    unsigned int correct_state[4*4] = {
        0x879531e0, 0xc5ecf37d, 0xbdb886dc, 0xc9a62f8a,
        0x44c20ef3, 0x3390af7f, 0xd9fc690b, 0xcfacafd2,
        0xe46bea80, 0xb00a5631, 0x974c541a, 0x359e9963,
        0x5c971061, 0xccc07c79, 0x2098d9d6, 0x91dbd320
    };

    __internal_chacha20::QUARTERROUND(stack_state, 2, 7, 8, 13);
    __internal_chacha20::QUARTERROUND(heap_state, 2, 7, 8, 13);

    bool passed = CompareState(stack_state,correct_state) && CompareState(heap_state,correct_state);

    delete [] heap_state;

    if(passed) {
        std::cout << "ChaChaQuarterRoundTest : PASSED\n";
        return 0;
    }
    else {
        std::cout << "ChaChaQuarterRoundTest : FAILED\n";
        return 1;
    }
}