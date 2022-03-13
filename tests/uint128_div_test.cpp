#include <iostream>
#include <utility>
#include "../uint128.hpp"

#define TEST_CASES 1

int main() {

    bool Tests[TEST_CASES];
    for(size_t i=0; i<TEST_CASES; ++i)
        Tests[i] = false;

    uint128 dividen1(98782342,764827642), divisor1(6,2745723);
    uint128 quotient1 = dividen1.ss_div(divisor1), correct_quotient1(0,0xfb376b);

    if(quotient1!=correct_quotient1) {
        std::cout << "uint128test_div1 : FAILED\n";
    }
    else {
        std::cout << "uint128test_div1 : PASSED\n";
        Tests[0] = true;
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