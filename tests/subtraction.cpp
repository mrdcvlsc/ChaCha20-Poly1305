#include <iostream>
#include <utility>
#include <vector>

#ifndef _MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint320 subtract "; 

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
    ulongint max = 0xffffffffffffffff;


    uint320 MAX(max,max,max,max,max);
    uint320 ZERO(0,0,0,0,0);
    uint320 ONE(0,0,0,0,1);
    uint320 TWO(0,0,0,0,2);
    uint320 THREE(0,0,0,0,3);

    uint320 NUM1(997816278132,678823674,87123,0,1),
            NUM2(0,8834,110,77770,10),
            CMAXZERO = MAX,
            CMAXMAX = ZERO,
            CMAXONE(max,max,max,max,0xfffffffffffffffe),
            CMAXTWO(max,max,max,max,0xfffffffffffffffd),
                      
            CNUM1NUM2(
                0xe8527c1c74, 0x000000002875e478, 0x00000000000153e4, 0xfffffffffffed035, 0xfffffffffffffff7
            );

    // ANSWERS           
    uint320 MAXZERO = MAX - ZERO,
            MAXONE = MAX - ONE,
            MAXTWO = MAX - TWO,
            MAXMAX = MAX - MAX,
            NUM1NUM2 = NUM1 - NUM2,
            ZEROMAX = ZERO - MAX,
            ONEMAX = ONE - MAX,
            TWOMAX = TWO - MAX;  

    // TESTING ANSWER 
    ASSERT_UINT512(MAXZERO,CMAXZERO,"MAX - 0");
    ASSERT_UINT512(MAXONE,CMAXONE,"MAX - 1");
    ASSERT_UINT512(MAXTWO,CMAXTWO,"MAX - 2");
    ASSERT_UINT512(MAXMAX,CMAXMAX,"MAX - MAX");
    ASSERT_UINT512(NUM1NUM2,CNUM1NUM2,"NUM1 - NUM2");
    ASSERT_UINT512(ZEROMAX,ONE,"0 - MAX");
    ASSERT_UINT512(ONEMAX,TWO,"1 - MAX");
    ASSERT_UINT512(TWOMAX,THREE,"2 - MAX");

    NUM1.printHex();
    NUM2.printHex();

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