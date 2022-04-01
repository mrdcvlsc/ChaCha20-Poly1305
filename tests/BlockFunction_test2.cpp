#include <iostream>
#include <utility>
#include <vector>

#ifndef MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

// #define PRINT_FAILED_OUTPUTS
std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "Block Function Additional Test 2 ";
void ASSERT_UINT512(const uint512& A, const uint512& B, const std::string& TEST_MESSAGE);

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
    unsigned char key[32] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };

    unsigned char nonce[12] = {
        0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00
    };

    unsigned int correct_output_state[16] = {
        0xbee7079f, 0x7a385155, 0x7c97ba98, 0x0d082d73,
        0xa0290fcb, 0x6965e348, 0x3e53c612, 0xed7aee32,
        0x7621b729, 0x434ee69c, 0xb03371d5, 0xd539d874,
        0x281fed31, 0x45fb0a51, 0x1f0ae1ac, 0x6f4d794b
    };

    unsigned char correct_serial[64] = {
        0x9f, 0x07, 0xe7, 0xbe, 0x55, 0x51, 0x38, 0x7a, 0x98, 0xba, 0x97, 0x7c, 0x73, 0x2d, 0x08, 0x0d,
        0xcb, 0x0f, 0x29, 0xa0, 0x48, 0xe3, 0x65, 0x69, 0x12, 0xc6, 0x53, 0x3e, 0x32, 0xee, 0x7a, 0xed,
        0x29, 0xb7, 0x21, 0x76, 0x9c, 0xe6, 0x4e, 0x43, 0xd5, 0x71, 0x33, 0xb0, 0x74, 0xd8, 0x39, 0xd5,
        0x31, 0xed, 0x1f, 0x28, 0x51, 0x0a, 0xfb, 0x45, 0xac, 0xe1, 0x0a, 0x1f, 0x4b, 0x79, 0x4d, 0x6f
    };

    unsigned int counter = 1;
   
    // ANSWERS
    unsigned int *initial_state = new unsigned int[__CHAx220_STATE_DWORDS__];
    unsigned int* output_state = new unsigned int[__CHAx220_BLK_FUNC_OUTPUT_DWORDS__];

    __internal_chacha20::init_state(initial_state,(unsigned int*)key,counter,(unsigned int*)nonce);
    __internal_chacha20::apply_20rounds(output_state,initial_state);
    
    unsigned char* serialize = (unsigned char*) output_state;

 
    // TESTING ANSWER 

    ASSERT_ARRAY<unsigned int>(output_state,correct_output_state,16,"output state",TEST_RESULTS);
    ASSERT_ARRAY<unsigned char>(serialize,correct_serial,64,"serialized ",TEST_RESULTS);
    
    delete [] initial_state;
    delete [] output_state;
    
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


void ASSERT_UINT512(const uint512& A, const uint512& B, const std::string& TEST_MESSAGE) {
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
    std::cout << TEST_NAME << ":" << TEST_MESSAGE << " : ";
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