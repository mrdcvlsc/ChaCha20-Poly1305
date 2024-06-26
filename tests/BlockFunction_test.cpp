#include <iostream>

#ifndef _MAKE_LIB
    #include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
    #include <ChaCha20-Poly1305.hpp>
#endif

// ChaCha State Comparison
bool CompareState(unsigned int *A, unsigned int *B) {
    for (size_t i = 0; i < 16; ++i)
        if (A[i] != B[i])
            return false;
    return true;
}

int main() {
    unsigned char key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                             0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                             0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    unsigned char nonce[12] = {0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00};

    unsigned int correct_output_state[16] = {0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3, 0xc7f4d1c7, 0x0368c033,
                                             0x9aaa2204, 0x4e6cd4c3, 0x466482d2, 0x09aa9f07, 0x05d7c214, 0xa2028bd9,
                                             0xd19c12b5, 0xb94e16de, 0xe883d0cb, 0x4e3c50a2};

    unsigned char correct_serial[64] = {0x10, 0xf1, 0xe7, 0xe4, 0xd1, 0x3b, 0x59, 0x15, 0x50, 0x0f, 0xdd, 0x1f, 0xa3,
                                        0x20, 0x71, 0xc4, 0xc7, 0xd1, 0xf4, 0xc7, 0x33, 0xc0, 0x68, 0x03, 0x04, 0x22,
                                        0xaa, 0x9a, 0xc3, 0xd4, 0x6c, 0x4e, 0xd2, 0x82, 0x64, 0x46, 0x07, 0x9f, 0xaa,
                                        0x09, 0x14, 0xc2, 0xd7, 0x05, 0xd9, 0x8b, 0x02, 0xa2, 0xb5, 0x12, 0x9c, 0xd1,
                                        0xde, 0x16, 0x4e, 0xb9, 0xcb, 0xd0, 0x83, 0xe8, 0xa2, 0x50, 0x3c, 0x4e};

    unsigned int *initial_state = new unsigned int[CHACHA20_STATE_DWORDS];
    unsigned int *output_state = new unsigned int[CHACHA20_BLK_FUNC_OUTPUT_DWORDS];

    chacha20::init_state(initial_state, (unsigned int *) key, 1, (unsigned int *) nonce);
    chacha20::apply_20rounds(output_state, initial_state);

    unsigned char *serialize = (unsigned char *) output_state;

    bool serial_passed = true;
    for (size_t i = 0; i < CHACHA20_BLK_FUNC_OUTPUT_BYTES; ++i) {
        if (serialize[i] != correct_serial[i]) {
            serial_passed = false;
            break;
        }
    }

    bool passed = CompareState(correct_output_state, output_state);

    delete[] initial_state;
    delete[] output_state;

    if (passed && serial_passed) {
        std::cout << "ChaChaBlockFunctionTest : PASSED\n";
        return 0;
    } else {
        std::cout << "ChaChaBlockFunctionTest : FAILED\n";
        return 1;
    }
}