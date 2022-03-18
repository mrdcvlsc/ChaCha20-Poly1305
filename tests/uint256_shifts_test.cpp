#include <iostream>
#include <utility>
#include <vector>
#ifndef MAKE_LIB
#include "../Header-Mode-ChaCha20-Poly1305.hpp"
#else
#include <ChaCha20-Poly1305.hpp>
#endif

#define PRINT_FAILED_OUTPUTS 0
std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint256 shifts";
void ASSERT_UINT256(const uint256& A, const uint256& B, const std::string& TEST_MESSAGE);

int main() {
    std::cout << "\n---------------------------------\n";
    std::cout << TEST_NAME << "\n=================================\n";

    // ==================================================================
    // TEST VARIABLES
    uint256 MAXVAL(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));

    // ==================================================================
    // L-SHIFT ANSWERS
    uint256 ls000 = MAXVAL << 0;
    uint256 ls007 = MAXVAL << 7;
    uint256 ls064 = MAXVAL << 64;
    uint256 ls063 = MAXVAL << 63;
    uint256 ls127 = MAXVAL << 127;
    uint256 ls120 = MAXVAL << 120;
    uint256 ls128 = MAXVAL << 128;

    uint256 ls129 = MAXVAL << 129;
    uint256 ls130 = MAXVAL << 130;
    uint256 ls191 = MAXVAL << 191;
    uint256 ls192 = MAXVAL << 192;
    uint256 ls193 = MAXVAL << 193;
    uint256 ls254 = MAXVAL << 254;
    uint256 ls256 = MAXVAL << 256;
    uint256 ls257 = MAXVAL << 257;

    // R-SHIFT ANSWERS
    uint256 rs000 = MAXVAL >> 0;
    uint256 rs007 = MAXVAL >> 7;
    uint256 rs064 = MAXVAL >> 64;
    uint256 rs063 = MAXVAL >> 63;
    uint256 rs127 = MAXVAL >> 127;
    uint256 rs120 = MAXVAL >> 120;
    uint256 rs128 = MAXVAL >> 128;

    uint256 rs129 = MAXVAL >> 129;
    uint256 rs130 = MAXVAL >> 130;
    uint256 rs191 = MAXVAL >> 191;
    uint256 rs192 = MAXVAL >> 192;
    uint256 rs193 = MAXVAL >> 193;
    uint256 rs254 = MAXVAL >> 254;
    uint256 rs256 = MAXVAL >> 256;
    uint256 rs257 = MAXVAL >> 257;

    // L-ASIGN-SHIFT ANSWERS
    uint256 Als000 = MAXVAL; Als000 <<= 0;
    uint256 Als007 = MAXVAL; Als007 <<= 7;
    uint256 Als064 = MAXVAL; Als064 <<= 64;
    uint256 Als063 = MAXVAL; Als063 <<= 63;
    uint256 Als127 = MAXVAL; Als127 <<= 127;
    uint256 Als120 = MAXVAL; Als120 <<= 120;
    uint256 Als128 = MAXVAL; Als128 <<= 128;

    uint256 Als129 = MAXVAL; Als129 <<= 129;
    uint256 Als130 = MAXVAL; Als130 <<= 130;
    uint256 Als191 = MAXVAL; Als191 <<= 191;
    uint256 Als192 = MAXVAL; Als192 <<= 192;
    uint256 Als193 = MAXVAL; Als193 <<= 193;
    uint256 Als255 = MAXVAL; Als255 <<= 255;
    uint256 Als256 = MAXVAL; Als256 <<= 256;
    uint256 Als257 = MAXVAL; Als257 <<= 257;

    // R-ASIGN-SHIFT ANSWERS
    uint256 Ars000 = MAXVAL; Ars000 >>= 0;
    uint256 Ars007 = MAXVAL; Ars007 >>= 7;
    uint256 Ars064 = MAXVAL; Ars064 >>= 64;
    uint256 Ars063 = MAXVAL; Ars063 >>= 63;
    uint256 Ars127 = MAXVAL; Ars127 >>= 127;
    uint256 Ars120 = MAXVAL; Ars120 >>= 120;
    uint256 Ars128 = MAXVAL; Ars128 >>= 128;

    uint256 Ars129 = MAXVAL; Ars129 >>= 129;
    uint256 Ars130 = MAXVAL; Ars130 >>= 130;
    uint256 Ars191 = MAXVAL; Ars191 >>= 191;
    uint256 Ars192 = MAXVAL; Ars192 >>= 192;
    uint256 Ars193 = MAXVAL; Ars193 >>= 193;
    uint256 Ars254 = MAXVAL; Ars254 >>= 254;
    uint256 Ars256 = MAXVAL; Ars256 >>= 256;
    uint256 Ars257 = MAXVAL; Ars257 >>= 257;

    // ==================================================================
    // TEST CORRECT ANSWER

    // L-SHIFTS CORRECT ANSWERS
    uint256 LSH007(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff, 0xffffffffffffff80));
    uint256 LSH064(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff, 0x0000000000000000));
    uint256 LSH063(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff, 0x8000000000000000));
    uint256 LSH127(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0x8000000000000000, 0x0000000000000000));
    uint256 LSH120(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xff00000000000000, 0x0000000000000000));
    uint256 LSH128(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0x0000000000000000, 0x0000000000000000));

    // R-SHIFT CORRECT ANSWERS
    uint256 RSH007(uint128(0x01ffffffffffffff, 0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 RSH064(uint128(0x0000000000000000, 0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 RSH063(uint128(0x0000000000000001, 0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 RSH127(uint128(0x0000000000000000, 0x0000000000000001),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 RSH120(uint128(0x0000000000000000, 0x00000000000000ff),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 RSH128(uint128(0x0000000000000000, 0x0000000000000000),uint128(0xffffffffffffffff,0xffffffffffffffff));

    // ==================================================================
    // TESTING ANSWER

    // left shifts
    ASSERT_UINT256(ls007,LSH007,"max << 7");
    ASSERT_UINT256(ls063,LSH063,"max << 63");
    ASSERT_UINT256(ls064,LSH064,"max << 64");
    ASSERT_UINT256(ls120,LSH120,"max << 120");
    ASSERT_UINT256(ls127,LSH127,"max << 127");
    ASSERT_UINT256(ls128,LSH128,"max << 128");

    // right shifts
    ASSERT_UINT256(rs007,RSH007,"max >> 7");
    ASSERT_UINT256(rs063,RSH063,"max >> 63");
    ASSERT_UINT256(rs064,RSH064,"max >> 64");
    ASSERT_UINT256(rs120,RSH120,"max >> 120");
    ASSERT_UINT256(rs127,RSH127,"max >> 127");
    ASSERT_UINT256(rs128,RSH128,"max >> 128");

     // assign left shifts
    ASSERT_UINT256(Als007,LSH007,"max << 7");
    ASSERT_UINT256(Als063,LSH063,"max << 63");
    ASSERT_UINT256(Als064,LSH064,"max << 64");
    ASSERT_UINT256(Als120,LSH120,"max << 120");
    ASSERT_UINT256(Als127,LSH127,"max << 127");
    ASSERT_UINT256(Als128,LSH128,"max << 128");

    // assign right shifts
    ASSERT_UINT256(Ars007,RSH007,"max >> 7");
    ASSERT_UINT256(Ars063,RSH063,"max >> 63");
    ASSERT_UINT256(Ars064,RSH064,"max >> 64");
    ASSERT_UINT256(Ars120,RSH120,"max >> 120");
    ASSERT_UINT256(Ars127,RSH127,"max >> 127");
    ASSERT_UINT256(Ars128,RSH128,"max >> 128");

    // ==================================================================
    // SUMMARY OF RESULTS
    size_t failed_cnt = 0;
    for(auto e : TEST_RESULTS) {
        if(!e) failed_cnt++;
    }

    std::cout << "---------------------------------\n";
    std::cout << TEST_NAME << " RESULT : " << "\n";
    if(!failed_cnt) {
        std::cout << "\tALL TESTS PASSED\n";
        std::cout << "---------------------------------\n";
        return 0;
    }
    else {
        std::cout << "\tSOME TEST(s) FAILED\n";
        std::cout << "---------------------------------\n";
        return 1;
    } 
}

void ASSERT_UINT256(const uint256& A, const uint256& B, const std::string& TEST_MESSAGE) {
    std::cout << TEST_NAME << " -> " << TEST_MESSAGE << " : ";
    if(A!=B) {
        std::cout << "FAILED\n";
        TEST_RESULTS.push_back(false);
        #ifdef PRINT_FAILED_OUTPUTS
        A.printBits_separated();
        std::cout << "\n";
        B.printBits_separated();
        std::cout << "\n";
        #endif
    }
    else {
        std::cout << "PASSED\n";
        TEST_RESULTS.push_back(true);
    }
}