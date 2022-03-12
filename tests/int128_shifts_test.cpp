#include <iostream>
#include "../int-128-256.hpp"

#include <iostream>
#include <utility>
#include "../int-128-256.hpp"

#define TEST_CASES 24

int main() {

    bool Tests[TEST_CASES];
    for(size_t i=0; i<TEST_CASES; ++i)
        Tests[i] = false;

    int128 MAXVAL(0xffffffffffffffff, 0xffffffffffffffff);

    // L-SHIFTS CORRECT ANSWERS
    int128 LSH007(0xffffffffffffffff, 0xffffffffffffff80);
    int128 LSH064(0xffffffffffffffff, 0x0000000000000000);
    int128 LSH063(0xffffffffffffffff, 0x8000000000000000);
    int128 LSH127(0x8000000000000000, 0x0000000000000000);
    int128 LSH120(0xff00000000000000, 0x0000000000000000);
    int128 LSH128(0x0000000000000000, 0x0000000000000000);

    // R-SHIFT CORRECT ANSWERS
    int128 RSH007(0x01ffffffffffffff, 0xffffffffffffffff);
    int128 RSH064(0x0000000000000000, 0xffffffffffffffff);
    int128 RSH063(0x0000000000000001, 0xffffffffffffffff);
    int128 RSH127(0x0000000000000000, 0x0000000000000001);
    int128 RSH120(0x0000000000000000, 0x00000000000000ff);
    int128 RSH128(0x0000000000000000, 0x0000000000000000);

    // L-SHIFT ANSWERS
    int128 ls007 = MAXVAL << 7;
    int128 ls064 = MAXVAL << 64;
    int128 ls063 = MAXVAL << 63;
    int128 ls127 = MAXVAL << 127;
    int128 ls120 = MAXVAL << 120;
    int128 ls128 = MAXVAL << 128;

    // R-SHIFT ANSWERS
    int128 rs007 = MAXVAL >> 7;
    int128 rs064 = MAXVAL >> 64;
    int128 rs063 = MAXVAL >> 63;
    int128 rs127 = MAXVAL >> 127;
    int128 rs120 = MAXVAL >> 120;
    int128 rs128 = MAXVAL >> 128;

    // L-ASIGN-SHIFT ANSWERS
    int128 Als007 = MAXVAL; Als007 <<= 7;
    int128 Als064 = MAXVAL; Als064 <<= 64;
    int128 Als063 = MAXVAL; Als063 <<= 63;
    int128 Als127 = MAXVAL; Als127 <<= 127;
    int128 Als120 = MAXVAL; Als120 <<= 120;
    int128 Als128 = MAXVAL; Als128 <<= 128;

    // R-ASIGN-SHIFT ANSWERS
    int128 Ars007 = MAXVAL; Ars007 >>= 7;
    int128 Ars064 = MAXVAL; Ars064 >>= 64;
    int128 Ars063 = MAXVAL; Ars063 >>= 63;
    int128 Ars127 = MAXVAL; Ars127 >>= 127;
    int128 Ars120 = MAXVAL; Ars120 >>= 120;
    int128 Ars128 = MAXVAL; Ars128 >>= 128;

    // L-SHIFT TESTS

    if(ls007!=LSH007) {
        std::cout << "int128 max << 7 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 7 : PASSED\n";
        Tests[0] = true;
    }
    
    if(ls064!=LSH064) {
        std::cout << "int128 max << 64 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 64 : PASSED\n";
        Tests[1] = true;
    }

    if(ls063!=LSH063) {
        std::cout << "int128 max << 63 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 63 : PASSED\n";
        Tests[2] = true;
    }

    if(ls127!=LSH127) {
        std::cout << "int128 max << 127 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 127 : PASSED\n";
        Tests[3] = true;
    }

    if(ls120!=LSH120) {
        std::cout << "int128 max << 120 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 120 : PASSED\n";
        Tests[4] = true;
    }

    if(ls128!=LSH128) {
        std::cout << "int128 max << 128 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 128 : PASSED\n";
        Tests[5] = true;
    }

    // R-SHIFT TESTS

    if(rs007!=RSH007) {
        std::cout << "int128 max >> 7 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 7 : PASSED\n";
        Tests[6] = true;
    }
    
    if(rs064!=RSH064) {
        std::cout << "int128 max >> 64 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 64 : PASSED\n";
        Tests[7] = true;
    }

    if(rs063!=RSH063) {
        std::cout << "int128 max >> 63 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 63 : PASSED\n";
        Tests[8] = true;
    }

    if(rs127!=RSH127) {
        std::cout << "int128 max >> 127 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 127 : PASSED\n";
        Tests[9] = true;
    }

    if(rs120!=RSH120) {
        std::cout << "int128 max >> 120 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 120 : PASSED\n";
        Tests[10] = true;
    }

    if(rs128!=RSH128) {
        std::cout << "int128 max >> 128 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 128 : PASSED\n";
        Tests[11] = true;
    }

    // ASIGN-L-SHIFT TESTS

    if(Als007!=LSH007) {
        std::cout << "int128 max << 7 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 7 : PASSED\n";
        Tests[12] = true;
    }
    
    if(Als064!=LSH064) {
        std::cout << "int128 max << 64 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 64 : PASSED\n";
        Tests[13] = true;
    }

    if(Als063!=LSH063) {
        std::cout << "int128 max << 63 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 63 : PASSED\n";
        Tests[14] = true;
    }

    if(Als127!=LSH127) {
        std::cout << "int128 max << 127 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 127 : PASSED\n";
        Tests[15] = true;
    }

    if(Als120!=LSH120) {
        std::cout << "int128 max << 120 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 120 : PASSED\n";
        Tests[16] = true;
    }

    if(Als128!=LSH128) {
        std::cout << "int128 max << 128 : FAILED\n";
    }
    else {
        std::cout << "int128 max << 128 : PASSED\n";
        Tests[17] = true;
    }

    // ASSIGN-R-SHIFT TESTS

    if(Ars007!=RSH007) {
        std::cout << "int128 max >> 7 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 7 : PASSED\n";
        Tests[18] = true;
    }
    
    if(Ars064!=RSH064) {
        std::cout << "int128 max >> 64 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 64 : PASSED\n";
        Tests[19] = true;
    }

    if(Ars063!=RSH063) {
        std::cout << "int128 max >> 63 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 63 : PASSED\n";
        Tests[20] = true;
    }

    if(Ars127!=RSH127) {
        std::cout << "int128 max >> 127 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 127 : PASSED\n";
        Tests[21] = true;
    }

    if(Ars120!=RSH120) {
        std::cout << "int128 max >> 120 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 120 : PASSED\n";
        Tests[22] = true;
    }

    if(Ars128!=RSH128) {
        std::cout << "int128 max >> 128 : FAILED\n";
    }
    else {
        std::cout << "int128 max >> 128 : PASSED\n";
        Tests[23] = true;
    }

    // end of tests
    
    size_t failed_cnt = 0;
    for(size_t i=0; i<TEST_CASES; ++i) {
        if(!Tests[i])
            failed_cnt++;
    }

    if(!failed_cnt) {
        std::cout << "int128 shifts ALL test PASSED\n";
        return 0;
    }
    else {
        std::cout << "int128 shifts some test FAILED\n";
        return 1;
    }
}