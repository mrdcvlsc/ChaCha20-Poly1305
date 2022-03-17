#include <iostream>
#include "../ChaCha20.hpp"

#define TEST_CASES 5

int main() {

    bool Tests[TEST_CASES];

    uint128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    uint128 ZERO(0,0);
    uint128 ONE(0,1);
    uint128 TWO(0,2);

    uint128 ZERO_MIN_ONE = ZERO;
    ZERO_MIN_ONE -= ONE;
    if(ZERO_MIN_ONE!=MAX) {
        std::cout << "uint128test_assign_zero-one : FAILED\n";
        Tests[0] = false;
    }
    else {
        std::cout << "uint128test_assign_zero-one : PASSED\n";
        Tests[0] = true;
    }

    uint128 ZERO_MIN_TWO = ZERO;
    ZERO_MIN_TWO -= TWO;
    uint128 ZERO_MIN_TWO_CORRECT(0xffffffffffffffff,0xfffffffffffffffe);
    if(ZERO_MIN_TWO!=ZERO_MIN_TWO_CORRECT) {
        std::cout << "uint128test_assign_zero-two : FAILED\n";
        Tests[1] = false;
    }
    else {
        std::cout << "uint128test_assign_zero-two : PASSED\n";
        Tests[1] = true;
    }

    uint128 ZERO_MIN_ZERO = ZERO;
    ZERO_MIN_ZERO -= ZERO;
    uint128 ZERO_MIN_ZERO_CORRECT(0,0);
    if(ZERO_MIN_ZERO!=ZERO_MIN_ZERO_CORRECT) {
        std::cout << "uint128test_assign_zero-zero : FAILED\n";
        Tests[2] = false;
    }
    else {
        std::cout << "uint128test_assign_zero-zero : PASSED\n";
        Tests[2] = true;
    }

    uint128 ZERO_MIN_MAX = ZERO;
    ZERO_MIN_MAX -= MAX;
    if(ZERO_MIN_MAX!=ONE) {
        std::cout << "uint128test_assign_zero-max : FAILED\n";
        Tests[3] = false;
    }
    else {
        std::cout << "uint128test_assign_zero-max : PASSED\n";
        // ZERO_MIN_MAX.printBits();
        Tests[3] = true;
    }

    uint128 TWO_MIN_MAX = TWO;
    TWO_MIN_MAX -= MAX;
    uint128 THREE(0,3);
    if(TWO_MIN_MAX!=THREE) {
        std::cout << "uint128test_assign_two-max : FAILED\n";
        Tests[4] = false;
    }
    else {
        std::cout << "uint128test_assign_two-max : PASSED\n";
        // TWO_MIN_MAX.printBits();
        Tests[4] = true;
    }

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "uint128 assign-sub ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "uint128 assign-sub SOME test FAILED\n";
        return 1;
    }
}