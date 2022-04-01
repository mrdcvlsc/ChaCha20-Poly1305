#include <iostream>
#include <utility>
#include <vector>

#ifndef MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "Left Shifts";

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
    ulongint max = 0xffffffffffffffff;
    uint512 MAX(max,max,max,max,max,max,max,max);
    uint512 ZERO(0,0,0,0,0,0,0,0);
    uint512 CLS032(max,max,max,max,max,max,max,0xffffffff00000000);
    uint512 CLS064(max,max,max,max,max,max,max,0);
    uint512 CLS096(max,max,max,max,max,max,0xffffffff00000000, 0);
    uint512 CLS128(max,max,max,max,max,max,0,0);
    uint512 CLS160(max,max,max,max,max,0xffffffff00000000,0,0);
    uint512 CLS192(max,max,max,max,max,0,0,0);
    uint512 CLS256(max,max,max,max,0,0,0,0);
    uint512 CLS500(0xfff0000000000000,0,0,0,0,0,0,0);

    // uint512 CLS000 = 

    // ANSWERS
    uint512 LS000 = MAX << 0;
    uint512 LS032 = MAX << 32;
    uint512 LS064 = MAX << 64;
    uint512 LS096 = MAX << 96;
    uint512 LS128 = MAX << 128;
    uint512 LS160 = MAX << 160;
    uint512 LS192 = MAX << 192;
    uint512 LS256 = MAX << 256;
    uint512 LS500 = MAX << 500;
    uint512 LS512 = MAX << 512;
    uint512 LS600 = MAX << 600;

    // TESTING ANSWER
    ASSERT_UINT512(LS000, MAX,    "MAX <<   0");
    ASSERT_UINT512(LS032, CLS032, "MAX <<  32");
    ASSERT_UINT512(LS064, CLS064, "MAX <<  64");
    ASSERT_UINT512(LS096, CLS096, "MAX <<  96");
    ASSERT_UINT512(LS128, CLS128, "MAX << 128");
    ASSERT_UINT512(LS160, CLS160, "MAX << 160");
    ASSERT_UINT512(LS192, CLS192, "MAX << 192");
    ASSERT_UINT512(LS256, CLS256, "MAX << 256");
    ASSERT_UINT512(LS500, CLS500, "MAX << 500");
    ASSERT_UINT512(LS512, ZERO, "MAX << 512");
    ASSERT_UINT512(LS600, ZERO, "MAX << 600");

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