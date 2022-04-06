#include <iostream>
#include <utility>
#include <vector>

#ifndef _MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint320 multiply "; 

void ASSERT_UINT512(const uint320& A, const uint320& B, const std::string& TEST_MESSAGE);

template<typename T>
void ASSERT_ARRAY(T* A, T* B, size_t length, std::string TEST_MESSAGE, std::vector<bool>& RESULTS);

void printBytes(unsigned char* bytearray, size_t len) {
    for(size_t i=0; i<len; ++i) {
        printf("%02x ", bytearray[i]);
    }
    std::cout << "\n"; 
}
 
uint320 fac(size_t n) {
    uint320 factorial(1);
    uint320 iter = factorial;

    for(size_t i=1; i<=n; ++i) {
        factorial = factorial * iter;
        iter.limbs[0]++;
    }

    return factorial;
}

int main() {
    std::cout << "\n---------------------------------\n";
    std::cout << TEST_NAME << "\n=================================\n";

    // TEST VARIABLES
    ulongint max = 0xffffffffffffffff;        
    uint320 MAX(max,max,max,max,max);

    uint320
    
    CFAC67(0x45f0025cc534351, 0xd9eca1b12a7b1294, 0xa77c082c2962623d, 0xfe3152bcbff89f41, 0x0000000000000000),

    NUM1(0x50f8a1d3,
        0x681f0a0293e1e780,
        0x434d14d87362e5a1,
        0x8854c3a8be8c4421,
        0x3ed1f19133440c3e),

    NUM2(0x6b,
        0x022015fe6b0ed5d0,
        0xbdd1e57b51b30b3c,
        0x0212203893843bf0,
        0xd7b95830fde7b02b),

    CNUM1NUM2(0xa34a5ab361ab65ba, 0xc90872a07c00eae4, 0x84f144677012bfa0, 0xdf6a3a49e474a361, 0x48cc84c2b6caae6a),
        
    CMAXMAX(1);

    // ANSWERS
    uint320 FAC67 = fac(67),  
            MAXMAX = MAX * MAX,
            NUM1NUM2 = NUM1 * NUM2;
    
    // TESTING ANSWER
    
    ASSERT_UINT512(FAC67,CFAC67,"factorial(67)");
    ASSERT_UINT512(MAXMAX,CMAXMAX,"MAX*MAX");
    ASSERT_UINT512(NUM1NUM2,CNUM1NUM2,"NUM1*NUM2");


    // SUMMARY OF RESULTS  
    size_t failed_cnt = 0; 
    for(auto e : TEST_RESULTS) {     
        if(!e) failed_cnt++;
    }

    if(TEST_RESULTS.size()) {
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