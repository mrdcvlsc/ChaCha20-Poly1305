#include <iostream>
#include "../int-128-256.hpp"

int main() {

    bool Tests[5];

    int128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    int128 ONE(0,1);
    int128 TWO(0,2);

    int128 SUM = MAX + ONE;
    int128 CORRECT_SUM(0,0);
    if(SUM!=CORRECT_SUM) {
        std::cout << "int128test_max+one : FAILED\n";
        Tests[0] = false;
    }
    else{
        std::cout << "int128test_max+one : PASSED\n";
        // SUM.printBits();
        Tests[0] = true;
    }

    SUM = MAX + TWO;
    if(SUM!=ONE) {
        std::cout << "int128test_max+two : FAILED\n";
        Tests[1] = false;
    }
    else{
        std::cout << "int128test_max+two : PASSED\n";
        // SUM.printBits();
        Tests[1] = true;
    }

    SUM = MAX + MAX;
    int128 CORRECT_MAX_MAX_SUM(18446744073709551615ul,18446744073709551614ul);
    if(SUM!=CORRECT_MAX_MAX_SUM) {
        std::cout << "int128test_max+max : FAILED\n";
        Tests[2] = false;
    }
    else{
        std::cout << "int128test_max+max : PASSED\n";
        // SUM.printBits();
        Tests[2] = true;
    }

    int128 MAX_MSB_M5(0xfffffffffffffffa, 0xffffffffffffffff);
    int128 TEN(0,10);
    int128 CORRECT_MAX_MSB_M5_ADD_TEN(0xfffffffffffffffb,0x0000000000000009);
    SUM = MAX_MSB_M5 + TEN;
    if(SUM!=CORRECT_MAX_MSB_M5_ADD_TEN) {
        std::cout << "int128test_max_msb_m5+ten : FAILED\n";
        Tests[3] = false;
    }
    else{
        std::cout << "int128test_max_msb_m5+ten : PASSED\n";
        // SUM.printBits();
        Tests[3] = true;
    }

    int128 MAX_M5(0xffffffffffffffff, 0xfffffffffffffffa);
    int128 CRRTM5(0xffffffffffffffff, 0xfffffffffffffff9);
    SUM = MAX_M5 + MAX;
    if(SUM!=CRRTM5) {
        std::cout << "int128test_max_msb_m5+max_m5 : FAILED\n";
        Tests[4] = false;
    }
    else{
        std::cout << "int128test_max_msb_m5+max_m5 : PASSED\n";
        Tests[4] = true;
    }

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<5; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "int128 add test ALL PASSED\n";
        return 0;
    }
    else {
        std::cout << "int128 add test ALL FAILED\n";
        return 0;
    }
}