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
    uint128 quotient1 = dividen1.ss_div(divisor1), correct_quotient1(0,0xfb376b);

    if(quotient1!=correct_quotient1) {
        std::cout << "uint128test_div1 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div1 : PASSED\n";
        Tests[0] = true;
    }

    // ----------------------------------------------------------------------------
    uint128 A(0x6ffffffcfffffffa, 0xfffffff4ffdff0ff), B(0x1234fffdfbcdefff, 0xa7affcccff000fff);
    uint128 AB = A.ss_div(B), BA = B.ss_div(A), correct_AB(0,0x6), correct_BA(0,0);

    uint128 C(98789182,87), D(1165125,8787737733);
    uint128 CD = C.ss_div(D), DC = D.ss_div(C), correct_CD(0,0x54), correct_DC(0,0);

    uint128 E(18446744073709551615ul,246810121416), F(0,90808664);
    uint128 EF = E.ss_div(F), FE = F.ss_div(E), correct_EF(0x2f4c00e9df, 0xf304781481b95104), correct_FE(0,0);

    if(AB!=correct_AB) {
        std::cout << "uint128test_div2 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div2 : PASSED\n";
        Tests[1] = true;
    }

    if(BA!=correct_BA) {
        std::cout << "uint128test_div3 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div3 : PASSED\n";
        Tests[2] = true;
    }

    if(CD!=correct_CD) {
        std::cout << "uint128test_div4 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div4 : PASSED\n";
        Tests[3] = true;
    }

    if(DC!=correct_DC) {
        std::cout << "uint128test_div5 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div5 : PASSED\n";
        Tests[4] = true;
    }

    if(EF!=correct_EF) {
        std::cout << "cEF = "; correct_EF.printBits();
        std::cout << " EF = "; EF.printBits();
        std::cout << "uint128test_div6 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div6 : PASSED\n";
        Tests[5] = true;
    }

    if(FE!=correct_FE) {
        std::cout << "uint128test_div7 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div7 : PASSED\n";
        Tests[6] = true;
    }

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "uint128 div ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "uint128 div SOME test FAILED\n";
        return 1;
    }
}