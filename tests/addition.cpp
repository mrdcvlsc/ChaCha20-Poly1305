#include <iostream>
#include <utility>
#include <vector>

#ifndef _MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint320 add "; 

void ASSERT_UINT512(const uint320& A, const uint320& B, const std::string& TEST_MESSAGE);

template<typename T>
void ASSERT_ARRAY(T* A, T* B, size_t length, std::string TEST_MESSAGE, std::vector<bool>& RESULTS);

void printBytes(unsigned char* bytearray, size_t len) {
    for(size_t i=0; i<len; ++i) {
        printf("%02x ", bytearray[i]);
    }
    std::cout << "\n"; 
}

uint320 fib(size_t nth) {
    uint320 base0(0,0,0,0,0), base1(0,0,0,0,1);
    uint320 nthfib(0,0,0,0,0);
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

    uint320 MAX(max,max,max,max,max);
    uint320 ZERO(0,0,0,0,0);
    uint320 ONE(0,0,0,0,1);
    uint320 TWO(0,0,0,0,2);

    uint320 NUM1(0x15453,0x00,0x253a56c05,0x19e87062bf,0x01),
            NUM2(0,0x2282,0x6e,0x12fca,0xa),
            NUMC(0x15453, 0x0000000000002282, 0x0000000253a56c73, 0x00000019e8719289, 0x000000000000000b),
            
    CFIB462(
        0xbf2e7ee72b7d39a0,
        0x4f98c3696e1e6f92,
        0x6c37f145ea105a4c,
        0x35aab97428b1a4d3,
        0x7af6ea1c8b65f068
    );

    uint320 CMAXMAX(max,max,max,max,0xfffffffffffffffe),

    CFIB100(0,0,0,0x13, 0x33db76a7c594bfc3);

    // ANSWERS        
    uint320 MAXONE = MAX + ONE, 
            MAXTWO = MAX + TWO,
            MAXMAX = MAX + MAX,
            NUMA = NUM1 + NUM2,
            FIB100 = fib(100),
            FIB462 = fib(462);

    // TESTING ANSWER       
    ASSERT_UINT512(MAXONE,ZERO,"MAX + 1");
    ASSERT_UINT512(MAXTWO,ONE,"MAX + 2");
    ASSERT_UINT512(MAXMAX,CMAXMAX,"MAX + MAX");
    ASSERT_UINT512(NUMA,NUMC,"NUM1 + NUM2");
    ASSERT_UINT512(FIB100,CFIB100,"fibonacci(100)");
    ASSERT_UINT512(FIB462,CFIB462,"fibonacci(739)");

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