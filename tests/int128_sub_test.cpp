#include <iostream>
#include "../int-128-256.hpp"

#define TEST_CASES 5

int main() {

    bool Tests[TEST_CASES];

    int128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    int128 ZERO(0,0);
    int128 ONE(0,1);
    int128 TWO(0,2);

    int128 ZERO_MIN_ONE = ZERO - ONE;
    if(ZERO_MIN_ONE!=MAX) {
        std::cout << "int128test_zero-one : FAILED\n";
        Tests[0] = false;
    }
    else {
        std::cout << "int128test_zero-one : PASSED\n";
        Tests[0] = true;
    }

    int128 ZERO_MIN_TWO = ZERO - TWO;
    int128 ZERO_MIN_TWO_CORRECT(0xffffffffffffffff,0xfffffffffffffffe);
    if(ZERO_MIN_TWO!=ZERO_MIN_TWO_CORRECT) {
        std::cout << "int128test_zero-two : FAILED\n";
        Tests[1] = false;
    }
    else {
        std::cout << "int128test_zero-two : PASSED\n";
        Tests[1] = true;
    }

    int128 ZERO_MIN_ZERO = ZERO - ZERO;
    int128 ZERO_MIN_ZERO_CORRECT(0,0);
    if(ZERO_MIN_ZERO!=ZERO_MIN_ZERO_CORRECT) {
        std::cout << "int128test_zero-zero : FAILED\n";
        Tests[2] = false;
    }
    else {
        std::cout << "int128test_zero-zero : PASSED\n";
        Tests[2] = true;
    }

    int128 ZERO_MIN_MAX = ZERO - MAX;
    if(ZERO_MIN_MAX!=ONE) {
        std::cout << "int128test_zero-max : FAILED\n";
        Tests[3] = false;
    }
    else {
        std::cout << "int128test_zero-max : PASSED\n";
        // ZERO_MIN_MAX.printBits();
        Tests[3] = true;
    }

    int128 TWO_MIN_MAX = TWO - MAX;
    int128 THREE(0,3);
    if(TWO_MIN_MAX!=THREE) {
        std::cout << "int128test_two-max : FAILED\n";
        Tests[4] = false;
    }
    else {
        std::cout << "int128test_two-max : PASSED\n";
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
        std::cout << "int128 sub ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "int128 sub SOME test FAILED\n";
        return 1;
    }
}