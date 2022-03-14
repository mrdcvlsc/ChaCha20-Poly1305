#include <iostream>
#include <utility>
#include "../uint256.hpp"

#define TEST_CASES 3

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

    uint256 MAX(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));
    MAX.printHex();
    uint256 FACTORIAL_32 = uint256_fac(34);
    FACTORIAL_32.printHex();

    std::cout << "\n\n";

    uint256 MAX128(uint128(0,0),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 MAX1282(uint128(0,0),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 PROD = MAX128 * MAX1282;

    MAX1282.printHex();
    PROD.printHex();

    // if(FACTORIAL_32!=FACTORIAL_OF_34_CORRECT) {
    //     std::cout << "uint256test_factorial(34) : FAILED\n";
    // }
    // else {
    //     std::cout << "uint256test_factorial(34) : PASSED\n";
    //     Tests[2] = true;
    // }

    // end of tests
    
    // size_t failed_cnt = 0;
    // for(size_t i=0; i<TEST_CASES; ++i) {
    //     if(!Tests[i])
    //         failed_cnt++;
    // }

    // if(!failed_cnt) {
    //     std::cout << "uint256 mul ALL test PASSED\n";
    //     return 0;
    // }
    // else {
    //     std::cout << "uint256 mul SOME test FAILED\n";
    //     return 1;
    // }
}