#include <iostream>
#include "../uint128.hpp"

#define TEST_CASES 6

int main() {

    bool Tests[TEST_CASES];
    for(size_t i=0; i<TEST_CASES; ++i)
        Tests[i] = false;

    uint128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    uint128 ONE(0,1);
    uint128 TWO(0,2);


    uint128 SUM = MAX;
    SUM += ONE;
    uint128 CORRECT_SUM(0,0);
    if(SUM!=CORRECT_SUM) {
        std::cout << "uint128test_max+=one : FAILED\n";
    }
    else{
        std::cout << "uint128test_max+=one : PASSED\n";
        // SUM.printBits();
        Tests[0] = true;
    }

    SUM = MAX;
    SUM += TWO;
    if(SUM!=ONE) {
        std::cout << "uint128test_max+=two : FAILED\n";
    }
    else{
        std::cout << "uint128test_max+=two : PASSED\n";
        // SUM.printBits();
        Tests[1] = true;
    }

    SUM = MAX;
    SUM += MAX;
    uint128 CORRECT_MAX_MAX_SUM(18446744073709551615ul,18446744073709551614ul);
    if(SUM!=CORRECT_MAX_MAX_SUM) {
        std::cout << "uint128test_max+=max : FAILED\n";
    }
    else{
        std::cout << "uint128test_max+=max : PASSED\n";
        // SUM.printBits();
        Tests[2] = true;
    }

    uint128 MAX_MSB_M5(0xfffffffffffffffa, 0xffffffffffffffff);
    uint128 TEN(0,10);
    uint128 CORRECT_MAX_MSB_M5_ADD_TEN(0xfffffffffffffffb,0x0000000000000009);
    SUM = MAX_MSB_M5;
    SUM += TEN;
    if(SUM!=CORRECT_MAX_MSB_M5_ADD_TEN) {
        std::cout << "uint128test_max_msb_m5+=ten : FAILED\n";
    }
    else{
        std::cout << "uint128test_max_msb_m5+=ten : PASSED\n";
        // SUM.printBits();
        Tests[3] = true;
    }

    uint128 MAX_M5(0xffffffffffffffff, 0xfffffffffffffffa);
    uint128 CRRTM5(0xffffffffffffffff, 0xfffffffffffffff9);
    SUM = MAX_M5;
    SUM += MAX;
    if(SUM!=CRRTM5) {
        std::cout << "uint128test_max_msb_m5+=max_m5 : FAILED\n";
    }
    else{
        std::cout << "uint128test_max_msb_m5+=max_m5 : PASSED\n";
        Tests[4] = true;
    }

    uint128 X64(0, 64);
    uint128 X64_CORRECT(0, 63);
    SUM = X64;
    SUM += MAX;
    if(SUM!=X64_CORRECT) {
        std::cout << "uint128test_max+=64 : FAILED\n";
    }
    else{
        std::cout << "uint128test_max+=64 : PASSED\n";
        Tests[5] = true;
    }
    
    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "uint128 add ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "uint128 add SOME test FAILED\n";
        return 1;
    }
}