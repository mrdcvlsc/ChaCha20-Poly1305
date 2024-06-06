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
const static std::string TEST_NAME = "Block Function Additional Test 4 ";
void ASSERT_UINT512(const epi::uint320_t& A, const epi::uint320_t& B, const std::string& TEST_MESSAGE);

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
        0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };

    unsigned char nonce[12] = {
        0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00
    };

    unsigned int correct_output_state[16] = {
        0xfb4dd572, 0x4bc42ef1, 0xdf922636, 0x327f1394,
        0xa78dea8f, 0x5e269039, 0xa1bebbc1, 0xcaf09aae,
        0xa25ab213, 0x48a6b46c, 0x1b9d9bcb, 0x092c5be6,
        0x546ca624, 0x1bec45d5, 0x87f47473, 0x96f0992e
    };

    unsigned char correct_serial[64] = {
        0x72, 0xd5, 0x4d, 0xfb, 0xf1, 0x2e, 0xc4, 0x4b, 0x36, 0x26, 0x92, 0xdf, 0x94, 0x13, 0x7f, 0x32,
        0x8f, 0xea, 0x8d, 0xa7, 0x39, 0x90, 0x26, 0x5e, 0xc1, 0xbb, 0xbe, 0xa1, 0xae, 0x9a, 0xf0, 0xca,
        0x13, 0xb2, 0x5a, 0xa2, 0x6c, 0xb4, 0xa6, 0x48, 0xcb, 0x9b, 0x9d, 0x1b, 0xe6, 0x5b, 0x2c, 0x09,
        0x24, 0xa6, 0x6c, 0x54, 0xd5, 0x45, 0xec, 0x1b, 0x73, 0x74, 0xf4, 0x87, 0x2e, 0x99, 0xf0, 0x96
    };

    unsigned int counter = 2;
   
    // ANSWERS
    unsigned int *initial_state = new unsigned int[CHACHA20_STATE_DWORDS];
    unsigned int* output_state = new unsigned int[CHACHA20_BLK_FUNC_OUTPUT_DWORDS];

    chacha20::init_state(initial_state,(unsigned int*)key,counter,(unsigned int*)nonce);
    chacha20::apply_20rounds(output_state,initial_state);
    
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


void ASSERT_UINT512(const epi::uint320_t& A, const epi::uint320_t& B, const std::string& TEST_MESSAGE) {
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