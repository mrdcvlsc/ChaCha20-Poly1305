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
const static std::string TEST_NAME = "Block Function Additional Test 5 ";
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
        0x00,0x00,0x00,0x00,0x00,0x02
    };

    unsigned int correct_output_state[16] = {
        0x374dc6c2, 0x3736d58c, 0xb904e24a, 0xcd3f93ef,
        0x88228b1a, 0x96a4dfb3, 0x5b76ab72, 0xc727ee54,
        0x0e0e978a, 0xf3145c95, 0x1b748ea8, 0xf786c297,
        0x99c28f5f, 0x628314e8, 0x398a19fa, 0x6ded1b53
    };

    unsigned char correct_serial[64] = {
        0xc2, 0xc6, 0x4d, 0x37, 0x8c, 0xd5, 0x36, 0x37, 0x4a, 0xe2, 0x04, 0xb9, 0xef, 0x93, 0x3f, 0xcd,
        0x1a, 0x8b, 0x22, 0x88, 0xb3, 0xdf, 0xa4, 0x96, 0x72, 0xab, 0x76, 0x5b, 0x54, 0xee, 0x27, 0xc7,
        0x8a, 0x97, 0x0e, 0x0e, 0x95, 0x5c, 0x14, 0xf3, 0xa8, 0x8e, 0x74, 0x1b, 0x97, 0xc2, 0x86, 0xf7,
        0x5f, 0x8f, 0xc2, 0x99, 0xe8, 0x14, 0x83, 0x62, 0xfa, 0x19, 0x8a, 0x39, 0x53, 0x1b, 0xed, 0x6d
    };

    unsigned int counter = 0;
   
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