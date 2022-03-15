#include <iostream>
#include <utility>
#include "../uint256.hpp"

#define TEST_CASES 6

uint256 uint256_fac(unsigned int n) {
    uint256
        factorial(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ONE),
        big_i(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ONE),
        one(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ONE);
    for(size_t i=0; i<n; ++i) {
        factorial = factorial * big_i;
        big_i = big_i + one;
    }
    return factorial;
}

int main() {
    
    bool Tests[TEST_CASES];
    for(size_t i=0; i<TEST_CASES; ++i)
        Tests[i] = false;

    uint256 ZERO(uint128(0,0),uint128(0,0));
    uint256 MAX(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 FACTORIAL_34 = uint256_fac(34);
    uint256 FACTORIAL_48 = uint256_fac(48);
    uint256 FACTORIAL_57 = uint256_fac(57);

    uint256 FAC34(uint128(0,0),uint128(0xde1bc4d19efcac82, 0x445da75b00000000));
    uint256 FAC48(uint128(0x7b9,0xa69e35cb2d866437), uint128(0xe5c47f97aef2c42c, 0xaee5c00000000000));
    uint256 FAC57(uint128(0x59996c6ef58409a7, 0x1b05be0bada2445e), uint128(0xb7c017d09442e7d1, 0x58e0000000000000));
    uint256 SQRDU128(uint128(0xffffffffffffffff, 0xfffffffffffffffe), uint128(0x0000000000000000, 0x0000000000000001));

    uint256 MAXMAX(uint128(0,0),uint128(0,0x1));
    uint256 MAX128(uint128(0,0),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 UPPER_MAX128(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0,0));

    UPPER_MAX128.printHex();

    uint256 PROD = MAX128 * MAX128;
    uint256 PRODMAX = MAX * MAX;
    uint256 PROD_UPPER_MAX = UPPER_MAX128*UPPER_MAX128;

    if(FACTORIAL_34!=FAC34) {
        std::cout << "uint256test_factorial(34) : FAILED\n";
    }
    else {
        std::cout << "uint256test_factorial(34) : PASSED\n";
        Tests[0] = true;
    }

    if(FACTORIAL_48!=FAC48) {
        std::cout << "uint256test_factorial(48) : FAILED\n";
    }
    else {
        std::cout << "uint256test_factorial(48) : PASSED\n";
        Tests[1] = true;
    }

    if(FACTORIAL_57!=FAC57) {
        std::cout << "uint256test_factorial(57) : FAILED\n";
    }
    else {
        std::cout << "uint256test_factorial(57) : PASSED\n";
        Tests[2] = true;
    }

    if(PROD!=SQRDU128) {
        std::cout << "uint256test_maxb128^2 : FAILED\n";
    }
    else {
        std::cout << "uint256test_maxb128^2 : PASSED\n";
        Tests[3] = true;
    }

    if(PRODMAX!=MAXMAX) {
        std::cout << "uint256test_max256^2 : FAILED\n";
        MAXMAX.printHex();
        PRODMAX.printHex();
    }
    else {
        std::cout << "uint256test_max256^2 : PASSED\n";
        Tests[4] = true;
    }


    if(PROD_UPPER_MAX!=ZERO) {
        std::cout << "uint256test_upper_max128^2 : FAILED\n";
    }
    else {
        std::cout << "uint256test_upper_max128^2 : PASSED\n";
        Tests[5] = true;
    }

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }


    if(!failed_cnt) {
        std::cout << "uint256 mul ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "uint256 mul SOME test FAILED\n";
        return 1;
    }
}