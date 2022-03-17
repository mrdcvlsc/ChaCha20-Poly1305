#include <iostream>
#include <utility>
#include "../ChaCha20.hpp"

#define TEST_CASES 3

uint128 uint128_fac(unsigned int n) {
    uint128
        factorial(0,1),
        big_i(0,1),
        one(0,1);
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

    uint128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    uint128 ZERO(0,0);
    uint128 ONE(0,1);
    uint128 TWO(0,2);

    uint128 ZERO_MUL = ZERO * MAX;
    if(ZERO_MUL!=ZERO) {
        std::cout << "uint128test_zero*max : FAILED\n";
    }
    else {
        std::cout << "uint128test_zero*max : PASSED\n";
        Tests[0] = true;
    }

    uint128 ONE_MUL = ONE * MAX;

    if(ONE_MUL!=MAX) { // should be max , only for debugging was set to zero
        std::cout << "uint128test_one*max : FAILED\n";
    }
    else {
        std::cout << "uint128test_one*max : PASSED\n";
        Tests[1] = true;
    }

    uint128 FACTORIAL_OF_34_CORRECT(0xde1bc4d19efcac82,0x445da75b00000000);
    uint128 FACTORIAL_32 = uint128_fac(34);
    if(FACTORIAL_32!=FACTORIAL_OF_34_CORRECT) {
        std::cout << "uint128test_factorial(34) : FAILED\n";
    }
    else {
        std::cout << "uint128test_factorial(34) : PASSED\n";
        Tests[2] = true;
    }

    
    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "uint128 mul ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "uint128 mul SOME test FAILED\n";
        return 1;
    }
}