#include <iostream>
#include <utility>
#include <vector>

#ifndef MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint512 divide "; 

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

    uint512

    NUM1(
        0x50f8a1d3,         0x681f0a0293e1e780,
        0x434d14d87362e5a1, 0x8854c3a8be8c4421,
        0x3ed1f19133440c3e, 0xf789c1fc4a08f0ec,
        0x25fc5431475fd566, 0x4973fcbf223cfecb),

    NUM2(
        0x6b,               0x022015fe6b0ed5d0,
        0xbdd1e57b51b30b3c, 0x0212203893843bf0,
        0xd7b95830fde7b02b, 0xbd435da2c22f3576,
        0xf00cd33ccd8eccf5, 0x83524543990f96a5); 
        
    uint512 CNUM1NUM2_MOD(
                      0x3f, 0x414ff188f62e9df4,
        0x88f5cfbff270f24a, 0x96d2fa8041573250,
        0x6d2eaeef6e4a38db, 0xebe541c76ab47710,
        0xf86c0f4885be80d3, 0x749161d5d182c17c);
        
    uint512 CNUM2NUM1_MOD(
                      0x6b, 0x022015fe6b0ed5d0,
        0xbdd1e57b51b30b3c, 0x0212203893843bf0,
        0xd7b95830fde7b02b, 0xbd435da2c22f3576,
        0xf00cd33ccd8eccf5, 0x83524543990f96a5);
    
    uint512 CMAXNUM1(0x3295fbec5),
        
    CMAXNUM1_MOD(
                 0x712d6e2, 0xfa659cd94c1017de,
        0x3f3ff66d1fb1f1be, 0x4fc6385eeba3217e,
        0x5743464383eba5ae, 0x026ebf3cb8a2d762,
        0x857f4d2a83ca1202, 0x96082faf51a043c8);
        
    uint512 CMAXNUM2(0x264703f12ce034b),
    
    CMAXNUM2_MOD(
                      0x58, 0x6cdea70994f7ab77,
        0x4d68060b3d2bb9f5, 0xd2e20f560e44dd8c,
        0x35bc093c29ea5e4e, 0xb73305577f06e0e4,
        0x39a528e804d11698, 0x73bb64a926e4eea8);

    uint512 CNUM1NUM2(0xc1b5e3), CNUM2NUM1(0), CMAXMAX(1);

    // ANSWERS

    uint512 MAXMAX = MAX / MAX,
            NUM1NUM2 = NUM1 / NUM2,
            NUM2NUM1 = NUM2 / NUM1,
            NUM1NUM2_MOD = NUM1 % NUM2,
            NUM2NUM1_MOD = NUM2 % NUM1,
            MAXNUM1 = MAX / NUM1, MAXNUM1_MOD = MAX % NUM1,
            MAXNUM2 = MAX / NUM2, MAXNUM2_MOD = MAX % NUM2;
    

    // TESTING ANSWER
    ASSERT_UINT512(MAXMAX,CMAXMAX,"MAX/MAX");
    ASSERT_UINT512(NUM1NUM2,CNUM1NUM2,"NUM1/NUM2");
    ASSERT_UINT512(NUM2NUM1,CNUM2NUM1,"NUM2/NUM1");
    ASSERT_UINT512(NUM1NUM2_MOD,CNUM1NUM2_MOD,"NUM1%NUM2");
    ASSERT_UINT512(NUM2NUM1_MOD,CNUM2NUM1_MOD,"NUM2%NUM1");
    ASSERT_UINT512(MAXNUM1,CMAXNUM1,"MAX/NUM1");
    ASSERT_UINT512(MAXNUM1_MOD,CMAXNUM1_MOD,"MAX%NUM1");
    ASSERT_UINT512(MAXNUM2,CMAXNUM2,"MAX/NUM2");
    ASSERT_UINT512(MAXNUM2_MOD,CMAXNUM2_MOD,"MAX%NUM2");


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