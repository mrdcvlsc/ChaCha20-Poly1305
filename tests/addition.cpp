#include <iostream>
#include <utility>
#include <vector>

#ifndef MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint512 add "; 

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
    ulongint max = 0xffffffffffffffff;

    uint512 MAX(max,max,max,max,max,max,max,max);
    uint512 ZERO(0,0,0,0,0,0,0,0);
    uint512 ONE(0,0,0,0,0,0,0,1);
    uint512 TWO(0,0,0,0,0,0,0,2);

    uint512 NUM1(0,0,0,87123,0,9993284613,111273861823,1),
            NUM2(0,0,0,0,8834,110,77770,10),
            NUMC(0,0,0,0x15453, 0x0000000000002282, 0x0000000253a56c73, 0x00000019e8719289, 0x000000000000000b);

    uint512 CMAXMAX(max,max,max,max,max,max,max,0xfffffffffffffffe);

    uint512 CFIB739(
        0xec30e4c53f672485, 0x7556f50afc80013b,
        0x0995173248cef4d3, 0x8bc099887fa83367, 
        0xd5dbd26953b22fe5, 0x7ecd1921fb7b6309,
        0xca7cac791eb08301, 0x891182cd299e32b5
    ), 

    CFIB100(0,0,0,0,0,0,0x13, 0x33db76a7c594bfc3);

    // ANSWERS        
    uint512 MAXONE = MAX + ONE, 
            MAXTWO = MAX + TWO,
            MAXMAX = MAX + MAX,
            NUMA = NUM1 + NUM2,
            FIB100 = fib(100),
            FIB739 = fib(739);

    // TESTING ANSWER       
    ASSERT_UINT512(MAXONE,ZERO,"MAX + 1");
    ASSERT_UINT512(MAXTWO,ONE,"MAX + 2");
    ASSERT_UINT512(MAXMAX,CMAXMAX,"MAX + MAX");
    ASSERT_UINT512(NUMA,NUMC,"NUM1 + NUM2");
    ASSERT_UINT512(FIB100,CFIB100,"fibonacci(100)");
    ASSERT_UINT512(FIB739,CFIB739,"fibonacci(739)");

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
    std::cout <<  ":" << TEST_MESSAGE << " : ";
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