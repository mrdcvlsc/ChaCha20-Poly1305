#include <iostream>
#include <utility>
#include <vector>

#ifndef MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint512 subtract "; 

void ASSERT_UINT512(const uint512& A, const uint512& B, const std::string& TEST_MESSAGE);

template<typename T>
void ASSERT_ARRAY(T* A, T* B, size_t length, std::string TEST_MESSAGE, std::vector<bool>& RESULTS);

void printBytes(unsigned char* bytearray, size_t len) {
    for(size_t i=0; i<len; ++i) {
        printf("%02x ", bytearray[i]);
    }
    std::cout << "\n"; 
}

uint512 fib(size_t nth) {
    uint512 base0(0,0,0,0,0,0,0,0), base1(0,0,0,0,0,0,0,1);
    uint512 nthfib(0,0,0,0,0,0,0,0);
    for(size_t i=2; i<=nth; ++i) {
        nthfib = base0 + base1;
        base0 = base1;
        base1 = nthfib;
    }
    return nthfib;
}

int main() {
    std::cout << "\n---------------------------------\n";
    std::cout << TEST_NAME << "\n=================================\n";

    // TEST VARIABLES
    unsigned long max = 0xffffffffffffffff;


    uint512 MAX(max,max,max,max,max,max,max,max);
    uint512 ZERO(0,0,0,0,0,0,0,0);
    uint512 ONE(0,0,0,0,0,0,0,1);
    uint512 TWO(0,0,0,0,0,0,0,2);
    uint512 THREE(0,0,0,0,0,0,0,3);

    uint512 NUM1(997816278132,678823674,1177717772171,87123,0,9993284613,111273861823,1),
            NUM2(0,0,0,0,8834,110,77770,10),
            CMAXZERO = MAX,
            CMAXMAX = ZERO,
            CMAXONE(max,max,max,max,max,max,max,0xfffffffffffffffe),
            CMAXTWO(max,max,max,max,max,max,max,0xfffffffffffffffd),
            CNUM1NUM2(
                0xe8527c1c74, 0x00000000287606fa,
                0x0000011235730f8b, 0x0000000000015452,
                0xffffffffffffdd7e, 0x0000000253a56b97,
                0x00000019e86f32f4, 0xfffffffffffffff7
            );

    // ANSWERS           
    uint512 MAXZERO = MAX - ZERO,
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