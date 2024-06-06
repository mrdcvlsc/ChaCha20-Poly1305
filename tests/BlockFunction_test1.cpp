#include <iostream>
#include <utility>
#include <vector>

#ifndef _MAKE_LIB
    #include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
    #include <ChaCha20-Poly1305.hpp>
#endif

// #define PRINT_FAILED_OUTPUTS
std::vector<bool>        TEST_RESULTS;
const static std::string TEST_NAME = "Block Function Additional Test 1 ";
void ASSERT_UINT512(const epi::uint320_t &A, const epi::uint320_t &B, const std::string &TEST_MESSAGE);

template <typename T>
void ASSERT_ARRAY(T *A, T *B, size_t length, std::string TEST_MESSAGE, std::vector<bool> &RESULTS);

void printBytes(unsigned char *bytearray, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x ", bytearray[i]);
    }
    std::cout << "\n";
}

int main() {
    std::cout << "\n---------------------------------\n";
    std::cout << TEST_NAME << "\n=================================\n";

    // TEST VARIABLES
    unsigned char key[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    unsigned char nonce[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    unsigned int correct_output_state[16] = {0xade0b876, 0x903df1a0, 0xe56a5d40, 0x28bd8653, 0xb819d2bd, 0x1aed8da0,
                                             0xccef36a8, 0xc70d778b, 0x7c5941da, 0x8d485751, 0x3fe02477, 0x374ad8b8,
                                             0xf4b8436a, 0x1ca11815, 0x69b687c3, 0x8665eeb2};

    unsigned char correct_serial[64] = {0x76, 0xb8, 0xe0, 0xad, 0xa0, 0xf1, 0x3d, 0x90, 0x40, 0x5d, 0x6a, 0xe5, 0x53,
                                        0x86, 0xbd, 0x28, 0xbd, 0xd2, 0x19, 0xb8, 0xa0, 0x8d, 0xed, 0x1a, 0xa8, 0x36,
                                        0xef, 0xcc, 0x8b, 0x77, 0x0d, 0xc7, 0xda, 0x41, 0x59, 0x7c, 0x51, 0x57, 0x48,
                                        0x8d, 0x77, 0x24, 0xe0, 0x3f, 0xb8, 0xd8, 0x4a, 0x37, 0x6a, 0x43, 0xb8, 0xf4,
                                        0x15, 0x18, 0xa1, 0x1c, 0xc3, 0x87, 0xb6, 0x69, 0xb2, 0xee, 0x65, 0x86};

    unsigned int counter = 0;

    // ANSWERS
    unsigned int *initial_state = new unsigned int[CHACHA20_STATE_DWORDS];
    unsigned int *output_state = new unsigned int[CHACHA20_BLK_FUNC_OUTPUT_DWORDS];

    chacha20::init_state(initial_state, (unsigned int *) key, counter, (unsigned int *) nonce);
    chacha20::apply_20rounds(output_state, initial_state);

    unsigned char *serialize = (unsigned char *) output_state;

    // TESTING ANSWER

    ASSERT_ARRAY<unsigned int>(output_state, correct_output_state, 16, "output state", TEST_RESULTS);
    ASSERT_ARRAY<unsigned char>(serialize, correct_serial, 64, "serialized ", TEST_RESULTS);

    delete[] initial_state;
    delete[] output_state;

    // SUMMARY OF RESULTS
    size_t failed_cnt = 0;
    for (auto e: TEST_RESULTS) {
        if (!e)
            failed_cnt++;
    }

    std::cout << "---------------------------------\n";
    std::cout << TEST_NAME << " RESULT:"
              << "\n";
    if (!failed_cnt) {
        std::cout << "\tALL test PASSED\n";
        std::cout << "---------------------------------\n";
        return 0;
    } else {
        std::cout << "\tSOME test FAILED\n";
        std::cout << "---------------------------------\n";
        return 1;
    }
}

void ASSERT_UINT512(const epi::uint320_t &A, const epi::uint320_t &B, const std::string &TEST_MESSAGE) {
    std::cout << TEST_NAME << ":" << TEST_MESSAGE << " : ";
    if (A != B) {
        std::cout << "FAILED\n";
        TEST_RESULTS.push_back(false);
    } else {
        std::cout << "PASSED\n";
        TEST_RESULTS.push_back(true);
    }
}

template <typename T>
void ASSERT_ARRAY(T *A, T *B, size_t length, std::string TEST_MESSAGE, std::vector<bool> &RESULTS) {
    std::cout << ":" << TEST_MESSAGE << " : ";
    bool result_passed = true;
    for (size_t i = 0; i < length; ++i) {
        if (A[i] != B[i]) {
            result_passed = false;
            break;
        }
    }

    if (!result_passed) {
        std::cout << "FAILED\n";
    } else {
        std::cout << "PASSED\n";
    }
    RESULTS.push_back(result_passed);
}