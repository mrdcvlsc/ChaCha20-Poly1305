#include <iostream>
#include <utility>

#ifndef _MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

#define TEST_CASES 1

int main() {

    bool Tests[TEST_CASES];
    for(size_t i=0; i<TEST_CASES; ++i)
        Tests[i] = false;

    ulongint max = 0xffffffffffffffff;
    uint320 ZERO(0), ONE(1), MAX(max,max,max,max,max);
    uint320 A(0,0,0,7879234,93);
    uint320 B(0,0,0,7879234,94);
    
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
        std::cout << "uint320:comparisons : FAILED\n";
    }
    else {
        std::cout << "uint320:comparisons : PASSED\n";
        Tests[0] = true;
    }

    // end of test
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "uint320 comparision ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "uint320 comparision SOME test FAILED\n";
        return 1;
    }
}