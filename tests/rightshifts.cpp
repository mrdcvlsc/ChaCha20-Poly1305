#include <iostream>
#include <utility>
#include <vector>

#ifndef _MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "Right Shifts";

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
    uint320 CRS032(0xffffffff,max,max,max,max);
    uint320 CRS064(0,max,max,max,max);
    uint320 CRS096(0,0xffffffff,max,max,max);
    uint320 CRS128(0,0,max,max,max);
    uint320 CRS160(0,0,0xffffffff,max,max);
    uint320 CRS192(0,0,0,max,max);
    uint320 CRS256(0,0,0,0,max);
    uint320 CRS500(0,0,0,0,0);

    // ANSWERS
    uint320 RS000 = MAX >> 0;
    uint320 RS032 = MAX >> 32;
    uint320 RS064 = MAX >> 64;
    uint320 RS096 = MAX >> 96;
    uint320 RS128 = MAX >> 128;
    uint320 RS160 = MAX >> 160;
    uint320 RS192 = MAX >> 192;
    uint320 RS256 = MAX >> 256;
    uint320 RS500 = MAX >> 500;
    uint320 RS512 = MAX >> 512;
    uint320 RS600 = MAX >> 600;

    // TESTING ANSWER
    ASSERT_UINT512(RS000, MAX,    "MAX >>   0");
    ASSERT_UINT512(RS032, CRS032, "MAX >>  32");
    ASSERT_UINT512(RS064, CRS064, "MAX >>  64");
    ASSERT_UINT512(RS096, CRS096, "MAX >>  96");
    ASSERT_UINT512(RS128, CRS128, "MAX >> 128");
    ASSERT_UINT512(RS160, CRS160, "MAX >> 160");
    ASSERT_UINT512(RS192, CRS192, "MAX >> 192");
    ASSERT_UINT512(RS256, CRS256, "MAX >> 256");
    ASSERT_UINT512(RS500, CRS500, "MAX >> 500");
    ASSERT_UINT512(RS512, ZERO, "MAX >> 512");
    ASSERT_UINT512(RS600, ZERO, "MAX >> 600");

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