#include <iostream>
#include "../int-128-256.hpp"

int main() {

    bool Tests[3];

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
        Tests[0] = true;
    }

    SUM = MAX + TWO;
    if(SUM!=ONE) {
        std::cout << "int128test_max+two : FAILED\n";
        Tests[1] = false;
    }
    else{
        std::cout << "int128test_max+two : PASSED\n";
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
        Tests[2] = true;
    }

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<3; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "int128test ALL PASSED\n";
        return 0;
    }
    else {
        std::cout << "int128test ALL FAILED\n";
        return 0;
    }
}