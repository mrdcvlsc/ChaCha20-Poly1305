#include <iostream>
#include <utility>
#include "../int-128-256.hpp"

#define TEST_CASES 1

int main() {

    bool Tests[TEST_CASES];
    for(size_t i=0; i<TEST_CASES; ++i)
        Tests[i] = false;

    int128 ZERO(0,0), ONE(0,1), MAX(0xffffffffffffffff,0xffffffffffffffff);
    int128 A(7879234,93);
    int128 B(7879234,94);
    
    if(
        !((ZERO == ZERO) && (ONE == ONE) && (A == A) && (MAX == MAX) &&
        (ZERO <= ZERO) && (ONE <= ONE) && (A <= A) && (MAX <= MAX) &&
        (ZERO >= ZERO) && (ONE >= ONE) && (A >= A) && (MAX >= MAX) &&
        (ZERO < ONE) && (ZERO < MAX) && (ZERO < A) && (ZERO < B) &&
        (ONE < MAX) && (ONE < A) && (ONE < B) &&
        (A < MAX) && (A < B) && (B < MAX) &&
        (MAX > ZERO) && (MAX > ONE) && (MAX > A) && (MAX > B) &&
        (B > ZERO) && (B > ONE) && (B > A) &&
        (A > ZERO) && (A > ONE) && (ONE > ZERO))
    ) {
        std::cout << "int128test_comparisons : FAILED\n";
    }
    else {
        std::cout << "int128test_comparisons : PASSED\n";
        Tests[0] = true;
    }

    // end of test
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "int128 mul ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "int128 mul SOME test FAILED\n";
        return 1;
    }
}