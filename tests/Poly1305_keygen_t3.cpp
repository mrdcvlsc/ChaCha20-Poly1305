#include <iostream>
#include <utility>
#include <vector>

#ifndef _MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

// #define PRINT_FAILED_OUTPUTS
std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "Poly1305 keygen using ChaCha20 test 3 ";
void ASSERT_UINT512(const uint320& A, const uint320& B, const std::string& TEST_MESSAGE);

template<typename T>
void ASSERT_ARRAY(T* A, T* B, size_t length, std::string TEST_MESSAGE, std::vector<bool>& RESULTS);

void printBytes(unsigned char* bytearray, size_t len) {
    for(size_t i=0; i<len; ++i) {
        printf("%02x ", bytearray[i]);
    }
    std::cout << "\n";
}

int main() {
    std::cout << "\n---------------------------------\n";
    std::cout << TEST_NAME << "\n=================================\n";

    // TEST VARIABLES
    unsigned char chacha20key[32] = {
        0x1c, 0x92, 0x40, 0xa5, 0xeb, 0x55, 0xd3, 0x8a, 0xf3, 0x33, 0x88, 0x86, 0x04, 0xf6, 0xb5, 0xf0,
        0x47, 0x39, 0x17, 0xc1, 0x40, 0x2b, 0x80, 0x09, 0x9d, 0xca, 0x5c, 0xbc, 0x20, 0x70, 0x75, 0xc0
    };

    unsigned char nonce[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
    };

    unsigned char polykey[] = {
        0x96, 0x5e, 0x3b, 0xc6, 0xf9, 0xec, 0x7e, 0xd9, 0x56, 0x08, 0x08, 0xf4, 0xd2, 0x29, 0xf9, 0x4b,
        0x13, 0x7f, 0xf2, 0x75, 0xca, 0x9b, 0x3f, 0xcb, 0xdd, 0x59, 0xde, 0xaa, 0xd2, 0x33, 0x10, 0xae
    };

    // ANSWERS

    unsigned char *genkey = new unsigned char[32];
    poly1305::key_gen(genkey,chacha20key,(unsigned int*)nonce);

    // TESTING ANSWER 
    ASSERT_ARRAY<unsigned char>(genkey,polykey,sizeof(polykey),"Poly1305 key generation test ",TEST_RESULTS);
    
    delete [] genkey;
    
    // SUMMARY OF RESULTS
    size_t failed_cnt = 0;
    for(auto e : TEST_RESULTS) {     
        if(!e) failed_cnt++;
    }

    std::cout << "---------------------------------\n";
    std::cout << TEST_NAME << " RESULT:" << "\n";
    if(!failed_cnt) {
        std::cout << "\tALL test PASSED\n";
        std::cout << "---------------------------------\n";
        return 0;
    }
    else {
        std::cout << "\tSOME test FAILED\n";
        std::cout << "---------------------------------\n";
        return 1;
    }  
}


void ASSERT_UINT512(const uint320& A, const uint320& B, const std::string& TEST_MESSAGE) {
    std::cout << TEST_NAME << ":" << TEST_MESSAGE << " : ";
    if(A!=B) {
        std::cout << "FAILED\n";
        TEST_RESULTS.push_back(false);
    }
    else {
        std::cout << "PASSED\n";
        TEST_RESULTS.push_back(true);
    }
}

template<typename T>
void ASSERT_ARRAY(T* A, T* B, size_t length, std::string TEST_MESSAGE, std::vector<bool>& RESULTS) {
    std::cout << ":" << TEST_MESSAGE << " : ";
    bool result_passed = true;
    for(size_t i=0; i<length; ++i) {
        if(A[i]!=B[i]) {
            result_passed = false;
            break;
        }
    }

    if(!result_passed) {
        std::cout << "FAILED\n";
    }
    else {
        std::cout << "PASSED\n";
    }
    RESULTS.push_back(result_passed);
}