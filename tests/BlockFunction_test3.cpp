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
const static std::string TEST_NAME = "Block Function Additional Test 3 ";
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
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
    };

    unsigned char nonce[12] = {
        0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00
    };

    unsigned int correct_output_state[16] = {
        0x2452eb3a, 0x9249f8ec, 0x8d829d9b, 0xddd4ceb1,
        0xe8252083, 0x60818b01, 0xf38422b8, 0x5aaa49c9,
        0xbb00ca8e, 0xda3ba7b4, 0xc4b592d1, 0xfdf2732f,
        0x4436274e, 0x2561b3c8, 0xebdd4aa6, 0xa0136c00
    };

    unsigned char correct_serial[64] = {
        0x3a, 0xeb, 0x52, 0x24, 0xec, 0xf8, 0x49, 0x92, 0x9b, 0x9d, 0x82, 0x8d, 0xb1, 0xce, 0xd4, 0xdd,
        0x83, 0x20, 0x25, 0xe8, 0x01, 0x8b, 0x81, 0x60, 0xb8, 0x22, 0x84, 0xf3, 0xc9, 0x49, 0xaa, 0x5a,
        0x8e, 0xca, 0x00, 0xbb, 0xb4, 0xa7, 0x3b, 0xda, 0xd1, 0x92, 0xb5, 0xc4, 0x2f, 0x73, 0xf2, 0xfd,
        0x4e, 0x27, 0x36, 0x44, 0xc8, 0xb3, 0x61, 0x25, 0xa6, 0x4a, 0xdd, 0xeb, 0x00, 0x6c, 0x13, 0xa0
    };

    unsigned int counter = 1;
   
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