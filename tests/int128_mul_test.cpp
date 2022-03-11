#include <iostream>
#include <utility>
#include "../int-128-256.hpp"

#define TEST_CASES 1

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

    std::cout << "\ncmax  =         " << std::bitset<8>(cmax) << "\n";
    std::cout << "cx64  =         " << std::bitset<8>(cx64) << "\n";
    std::cout << "cprd  =         " << std::bitset<8>(cprd) << "\n\n";

    std::cout << "scmax = " << std::bitset<16>(scmax) << "\n";
    std::cout << "scx64 = " << std::bitset<16>(scx64) << "\n";
    std::cout << "scprd = " << std::bitset<16>(scprd) << "\n\n";
    

    int128 MAX(0xffffffffffffffff, 0xffffffffffffffff);
    int128 ZERO(0,0);
    int128 ONE(0,1);
    int128 TWO(0,2);

    int128 ZERO_MUL = ZERO * MAX;
    if(ZERO_MUL!=ZERO) {
        std::cout << "int128test_zero*max : FAILED\n";
    }
    else {
        ZERO_MUL.printBits();
        std::cout << "int128test_zero*max : PASSED\n";
        Tests[0] = true;
    }

    int128 ONE_MUL = ONE * MAX;

    if(ONE_MUL!=MAX) {
        std::cout << "int128test_one*max : FAILED\n";
    }
    else {
        std::cout << "int128test_one*max : PASSED\n";
        Tests[0] = true;

    }

    // unsigned char max = 255;
    // unsigned char zer = 0;
    // unsigned char one = 1;
    // unsigned char two = 2;

    // unsigned char max_mul_max = max * max;
    // unsigned char max_mul_zer = max * zer;
    // unsigned char max_mul_one = max * one;
    // unsigned char max_mul_two = max * two;

    // std::cout << "\nmax_mul_max  = " << std::bitset<8>(max_mul_max) << "\n"; 
    // std::cout << "max_mul_zer  = " << std::bitset<8>(max_mul_zer) << "\n";
    // std::cout << "max_mul_one  = " << std::bitset<8>(max_mul_one) << "\n";
    // std::cout << "max_mul_two  = " << std::bitset<8>(max_mul_two) << "\n";

    // unsigned short smax = 255;
    // unsigned short szer = 0;
    // unsigned short sone = 1;
    // unsigned short stwo = 2;

    // unsigned short smax_mul_max = smax * smax;
    // unsigned short smax_mul_zer = smax * szer;
    // unsigned short smax_mul_one = smax * sone;
    // unsigned short smax_mul_two = smax * stwo;

    // std::cout << "\nsmax_mul_max = " << std::bitset<16>(smax_mul_max) << "\n"; 
    // std::cout << "smax_mul_zer = " << std::bitset<16>(smax_mul_zer) << "\n";
    // std::cout << "smax_mul_one = " << std::bitset<16>(smax_mul_one) << "\n";
    // std::cout << "smax_mul_two = " << std::bitset<16>(smax_mul_two) << "\n\n";


    // unsigned char asm_prod = asm_mul(max,max);
    // std::cout << "asm max*max  = " << std::bitset<16>(asm_prod) << "\n";

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "int128 mul test ALL PASSED\n";
        return 0;
    }
    else {
        std::cout << "int128 mul test ALL FAILED\n";
        return 0;
    }
}