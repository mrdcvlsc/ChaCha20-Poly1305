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
const static std::string TEST_NAME = "Poly1305 mac test 8 ";
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
    unsigned char polykey[32] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    unsigned char text[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                            0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
                            0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x01, 0x01, 0x01, 0x01,
                            0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};

    unsigned char tag[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // ANSWERS

    unsigned char *out_tag = new unsigned char[16];
    poly1305::mac(out_tag, polykey, text, sizeof(text));

    // TESTING ANSWER
    ASSERT_ARRAY<unsigned char>(out_tag, tag, 16, "Poly1305 tag/mac test ", TEST_RESULTS);

    delete[] out_tag;

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

    if (result_passed && poly1305::verify(A, B)) {
        std::cout << "PASSED\n";
    } else {
        std::cout << "FAILED\n";
    }
    RESULTS.push_back(result_passed && poly1305::verify(A, B));
}