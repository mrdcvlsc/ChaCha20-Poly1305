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
const static std::string TEST_NAME = "ChaCha20 Encryption Test 3 ";
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
    unsigned char key[32] = {0x1c, 0x92, 0x40, 0xa5, 0xeb, 0x55, 0xd3, 0x8a, 0xf3, 0x33, 0x88,
                             0x86, 0x04, 0xf6, 0xb5, 0xf0, 0x47, 0x39, 0x17, 0xc1, 0x40, 0x2b,
                             0x80, 0x09, 0x9d, 0xca, 0x5c, 0xbc, 0x20, 0x70, 0x75, 0xc0};

    unsigned char nonce[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};

    unsigned int counter = 42;

    unsigned char plain_text[] = {
      0x27, 0x54, 0x77, 0x61, 0x73, 0x20, 0x62, 0x72, 0x69, 0x6c, 0x6c, 0x69, 0x67, 0x2c, 0x20, 0x61, 0x6e, 0x64, 0x20,
      0x74, 0x68, 0x65, 0x20, 0x73, 0x6c, 0x69, 0x74, 0x68, 0x79, 0x20, 0x74, 0x6f, 0x76, 0x65, 0x73, 0x0a, 0x44, 0x69,
      0x64, 0x20, 0x67, 0x79, 0x72, 0x65, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x67, 0x69, 0x6d, 0x62, 0x6c, 0x65, 0x20, 0x69,
      0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x77, 0x61, 0x62, 0x65, 0x3a, 0x0a, 0x41, 0x6c, 0x6c, 0x20, 0x6d, 0x69, 0x6d,
      0x73, 0x79, 0x20, 0x77, 0x65, 0x72, 0x65, 0x20, 0x74, 0x68, 0x65, 0x20, 0x62, 0x6f, 0x72, 0x6f, 0x67, 0x6f, 0x76,
      0x65, 0x73, 0x2c, 0x0a, 0x41, 0x6e, 0x64, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6d, 0x6f, 0x6d, 0x65, 0x20, 0x72, 0x61,
      0x74, 0x68, 0x73, 0x20, 0x6f, 0x75, 0x74, 0x67, 0x72, 0x61, 0x62, 0x65, 0x2e};

    unsigned char cipher_text[] = {
      0x62, 0xe6, 0x34, 0x7f, 0x95, 0xed, 0x87, 0xa4, 0x5f, 0xfa, 0xe7, 0x42, 0x6f, 0x27, 0xa1, 0xdf, 0x5f, 0xb6, 0x91,
      0x10, 0x04, 0x4c, 0x0d, 0x73, 0x11, 0x8e, 0xff, 0xa9, 0x5b, 0x01, 0xe5, 0xcf, 0x16, 0x6d, 0x3d, 0xf2, 0xd7, 0x21,
      0xca, 0xf9, 0xb2, 0x1e, 0x5f, 0xb1, 0x4c, 0x61, 0x68, 0x71, 0xfd, 0x84, 0xc5, 0x4f, 0x9d, 0x65, 0xb2, 0x83, 0x19,
      0x6c, 0x7f, 0xe4, 0xf6, 0x05, 0x53, 0xeb, 0xf3, 0x9c, 0x64, 0x02, 0xc4, 0x22, 0x34, 0xe3, 0x2a, 0x35, 0x6b, 0x3e,
      0x76, 0x43, 0x12, 0xa6, 0x1a, 0x55, 0x32, 0x05, 0x57, 0x16, 0xea, 0xd6, 0x96, 0x25, 0x68, 0xf8, 0x7d, 0x3f, 0x3f,
      0x77, 0x04, 0xc6, 0xa8, 0xd1, 0xbc, 0xd1, 0xbf, 0x4d, 0x50, 0xd6, 0x15, 0x4b, 0x6d, 0xa7, 0x31, 0xb1, 0x87, 0xb5,
      0x8d, 0xfd, 0x72, 0x8a, 0xfa, 0x36, 0x75, 0x7a, 0x79, 0x7a, 0xc1, 0x88, 0xd1};

    // ANSWERS

    unsigned char *cipher_out = chacha20::encrypt(key, counter, nonce, plain_text, sizeof(plain_text));

    // TESTING ANSWER
    ASSERT_ARRAY<unsigned char>(cipher_out, cipher_text, sizeof(cipher_text), "ChaCha20 encryption ", TEST_RESULTS);

    delete[] cipher_out;

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