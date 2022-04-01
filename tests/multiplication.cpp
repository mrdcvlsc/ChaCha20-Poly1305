#include <iostream>
#include <utility>
#include <vector>

#ifndef MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint512 multiply "; 

void ASSERT_UINT512(const uint512& A, const uint512& B, const std::string& TEST_MESSAGE);

template<typename T>
void ASSERT_ARRAY(T* A, T* B, size_t length, std::string TEST_MESSAGE, std::vector<bool>& RESULTS);

void printBytes(unsigned char* bytearray, size_t len) {
    for(size_t i=0; i<len; ++i) {
        printf("%02x ", bytearray[i]);
    }
    std::cout << "\n"; 
}
 
uint512 fac(size_t n) {
    uint512 factorial(0,0,0,0,0,0,0,1);
    uint512 iter = factorial;

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
    uint512 MAX(max,max,max,max,max,max,max,max);

    uint512
    
    CFAC98(
        0xb3fdae4141a01eb8, 0x7d7eef7be85b2081,
        0xadb3d8455d37d503, 0xf972677dba345045,
        0x5447b6f366c6e855, 0x68b196e3bb65397b,
        0xe2e31f1380000000, 0x0000000000000000),

    NUM1(
        0x50f8a1d3,         0x681f0a0293e1e780,
        0x434d14d87362e5a1, 0x8854c3a8be8c4421,
        0x3ed1f19133440c3e, 0xf789c1fc4a08f0ec,
        0x25fc5431475fd566, 0x4973fcbf223cfecb),

    NUM2(
        0x6b,               0x022015fe6b0ed5d0,
        0xbdd1e57b51b30b3c, 0x0212203893843bf0,
        0xd7b95830fde7b02b, 0xbd435da2c22f3576,
        0xf00cd33ccd8eccf5, 0x83524543990f96a5
    ),

    CNUM1NUM2(
        0x3efd1b112afc8bbb, 0x8f0ccd5e20e624d2,
        0x612bee931aecbbe5, 0xeef8c00df7d67531,
        0xfd4426fcaf4fc118, 0xe906c55e2c874c11,
        0x741ff6714def8e44, 0x3d2cc13c0f802ad7
    ),
        
    CMAXMAX(0,0,0,0,0,0,0,1);

    // ANSWERS
    uint512 FAC98 = fac(98),  
            MAXMAX = MAX * MAX,
            NUM1NUM2 = NUM1 * NUM2;
    
    // TESTING ANSWER
    
    ASSERT_UINT512(FAC98,CFAC98,"factorial(98)");
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