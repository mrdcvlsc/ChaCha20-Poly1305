#include <iostream>
#include <utility>
#include "../int-128-256.hpp"

#define TEST_CASES 3

int128 uint128_fac(unsigned int n) {
    int128
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

    unsigned char cmax = 255;
    unsigned char cx64 = 7;
    unsigned char cprd = cmax * cx64;

    unsigned short scmax = 255;
    unsigned short scx64 = 7;
    unsigned short scprd = scmax * scx64;    

    int128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    int128 ZERO(0,0);
    int128 ONE(0,1);
    int128 TWO(0,2);

    int128 ZERO_MUL = ZERO * MAX;
    if(ZERO_MUL!=ZERO) {
        std::cout << "int128test_zero*max : FAILED\n";
    }
    else {
        std::cout << "int128test_zero*max : PASSED\n";
        Tests[0] = true;
    }

    int128 ONE_MUL = ONE * MAX;

    if(ONE_MUL!=MAX) { // should be max , only for debugging was set to zero
        std::cout << "int128test_one*max : FAILED\n";
    }
    else {
        std::cout << "int128test_one*max : PASSED\n";
        Tests[1] = true;
    }

    int128 FACTORIAL_OF_34_CORRECT(0xde1bc4d19efcac82,0x445da75b00000000);
    int128 FACTORIAL_32 = uint128_fac(34);
    if(FACTORIAL_32!=FACTORIAL_OF_34_CORRECT) {
        std::cout << "int128test_factorial(34) : FAILED\n";
    }
    else {
        std::cout << "int128test_factorial(34) : PASSED\n";
        Tests[2] = true;
    }

    
    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "int128 sub ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "int128 sub SOME test FAILED\n";
        return 1;
    }
}