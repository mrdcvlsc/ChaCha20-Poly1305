#include <iostream>
#include <utility>
#include "../uint128.hpp"

#define TEST_CASES 7

int main() {

    bool Tests[TEST_CASES];
    for(size_t i=0; i<TEST_CASES; ++i)
        Tests[i] = false;

    // ----------------------------------------------------------------------------
    uint128 dividen1(98782342,764827642), divisor1(6,2745723);
    uint128 quotient1 = dividen1, correct_quotient1(0,0xfb376b);
    quotient1 /= divisor1;

    if(quotient1!=correct_quotient1) {
        std::cout << "uint128test_/=_div1 : FAILED\n";
    }
    else {
        std::cout << "uint128test_/=_div1 : PASSED\n";
        Tests[0] = true;
    }

    // ----------------------------------------------------------------------------
    uint128 A(0x6ffffffcfffffffa, 0xfffffff4ffdff0ff), B(0x1234fffdfbcdefff, 0xa7affcccff000fff);
    uint128 AB = A, BA = B, correct_AB(0,0x6), correct_BA(0,0);
    AB /= B, BA /= A;

    uint128 C(98789182,87), D(1165125,8787737733);
    uint128 CD = C, DC = D, correct_CD(0,0x54), correct_DC(0,0);
    CD /= D, DC /= C;

    uint128 E(18446744073709551615ul,246810121416), F(0,90808664);
    uint128 EF = E, FE = F, correct_EF(0x2f4c00e9df, 0xf304781481b95104), correct_FE(0,0);
    EF /= F, FE /= E;

    // ----------------------------------------------------------------------------
    uint128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    uint128 TWO_RAISE_64(1,0);

    uint128 MAX_UINT64(0,0xffffffffffffffff);
    uint128 PRIME(0,97);

    uint128
        correct_MAX_DIV_MAXUINT64(0x1, 0x0000000000000001),
        correct_MAX_DIV_PRIME(0x2a3a0fd5c5f02a3, 0xa0fd5c5f02a3a0fd),
        correct_RAISE64_DIV_MAXUINT64(0,0x1),
        correct_RAISE64_DIV_PRIME(0, 0x2a3a0fd5c5f02a3);

    uint128
        MAX_DIV_MAXUINT64(0,0),
        MAX_DIV_PRIME(0,0),
        RAISE64_DIV_MAXUINT64(0,0),
        RAISE64_DIV_PRIME(0,0);

    // ----------------------------------------------------------------------------

    if(AB!=correct_AB) {
        std::cout << "uint128test_/=_div2 : FAILED\n";
    }
    else {
        std::cout << "uint128test_/=_div2 : PASSED\n";
        Tests[1] = true;
    }

    if(BA!=correct_BA) {
        std::cout << "uint128test_/=_div3 : FAILED\n";
    }
    else {
        std::cout << "uint128test_/=_div3 : PASSED\n";
        Tests[2] = true;
    }

    if(CD!=correct_CD) {
        std::cout << "uint128test_/=_div4 : FAILED\n";
    }
    else {
        std::cout << "uint128test_/=_div4 : PASSED\n";
        Tests[3] = true;
    }

    if(DC!=correct_DC) {
        std::cout << "uint128test_/=_div5 : FAILED\n";
    }
    else {
        std::cout << "uint128test_/=_div5 : PASSED\n";
        Tests[4] = true;
    }

    if(EF!=correct_EF) {
        std::cout << "uint128test_/=_div6 : FAILED\n";
    }
    else {
        std::cout << "uint128test_/=_div6 : PASSED\n";
        Tests[5] = true;
    }

    if(FE!=correct_FE) {
        std::cout << "uint128test_/=_div7 : FAILED\n";
    }
    else {
        std::cout << "uint128test_/=_div7 : PASSED\n";
        Tests[6] = true;
    }

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "uint128 assign-div ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "uint128 assign-div SOME test FAILED\n";
        return 1;
    }
}