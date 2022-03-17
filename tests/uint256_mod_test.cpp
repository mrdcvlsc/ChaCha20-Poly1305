#include <iostream>
#include <utility>
#include <vector>
#include "../ChaCha20.hpp"

// #define PRINT_FAILED_OUTPUTS
std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint256 mod";
void ASSERT_UINT256(const uint256& A, const uint256& B, const std::string& TEST_MESSAGE);

int main() {
    std::cout << "\n---------------------------------\n";
    std::cout << TEST_NAME << "\n=================================\n";

    // TEST VARIABLES
    uint256 MAX(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));
    uint256 NUM1(uint128(824824478,55712374343),uint128(1001023834,6672165363));
    uint256 ZERO(uint128(0,0),uint128(0,0));
    uint256 ONE(uint128(0,0),uint128(0,1));
    uint256 TWO(uint128(0,0),uint128(0,2));
    uint256 SEVENS(uint128(0,0),uint128(0,7777777777));

    // ANSWERS
    uint256 MAXMAX = MAX % MAX;
    uint256 MAXNUM1 = MAX % NUM1;
    uint256 MAXONE = MAX % ONE;
    uint256 MAXTWO = MAX % TWO;
    uint256 MAXSEVENS = MAX % SEVENS;

    uint256 SEVENSSEVENS = SEVENS % SEVENS;
    uint256 SEVENSMAX = SEVENS % MAX;
    uint256 SEVENSONE = SEVENS % ONE;
    uint256 SEVENSTWO = SEVENS % TWO;

    uint256 NUM1SEVENS = NUM1 % SEVENS;

    // TEST CORRECT ANSWER
    uint256 MAXMAX_CORRECT = ZERO;
    uint256 MAXNUM1_CORRECT(uint128(0x22e7b548, 0x749a443b2f9b21f4), uint128(0xc9502bfaa43c879b, 0xe92a920fa009e06d));
    uint256 MAXONE_CORRECT = ZERO;
    uint256 MAXTWO_CORRECT = ONE;
    uint256 MAXSEVENS_CORRECT(uint128(0, 0), uint128(0, 0x1748d7617));

    uint256 SEVENSSENVES_CORRECT = ZERO;
    uint256 SEVENSMAX_CORRECT(uint128(0,0),uint128(0,0x1cf977871));
    uint256 SEVENSONE_CORRECT = ZERO;
    uint256 SEVENSTWO_CORRECT = ONE;

    uint256 NUM1SEVENS_CORRECT(uint128(0,0), uint128(0, 0xe9f2422a));

    // TESTING ANSWER
    ASSERT_UINT256(MAXMAX,MAXMAX_CORRECT,"MAX/MAX");
    ASSERT_UINT256(MAXNUM1, MAXNUM1_CORRECT,"MAX/NUM1");
    ASSERT_UINT256(MAXONE,MAXONE_CORRECT,"MAX/1");
    ASSERT_UINT256(MAXTWO,MAXTWO_CORRECT,"MAX/2");
    ASSERT_UINT256(MAXSEVENS,MAXSEVENS_CORRECT,"MAX/SEVENS");

    ASSERT_UINT256(SEVENSSEVENS,SEVENSSENVES_CORRECT,"SEVENS/SEVENS");
    ASSERT_UINT256(SEVENSMAX,SEVENSMAX_CORRECT,"SEVENS/MAX");
    ASSERT_UINT256(SEVENSONE,SEVENSONE_CORRECT,"SEVENS/1");
    ASSERT_UINT256(SEVENSTWO,SEVENSTWO_CORRECT,"SEVENS/2");

    ASSERT_UINT256(NUM1SEVENS,NUM1SEVENS_CORRECT,"NUM1/SEVENS");


    // SUMMARY OF RESULTS
    size_t failed_cnt = 0;
    for(auto e : TEST_RESULTS) {
        if(!e) failed_cnt++;
    }

    std::cout << "---------------------------------\n";
    std::cout << TEST_NAME << " RESULT:" << "\n";
    if(!failed_cnt) {
        std::cout << "\tALL test PASSED\n";
        std::cout << "---------------------------------\n";
        return 0;
    }
    else {
        std::cout << "\tSOME test FAILED\n";
        std::cout << "---------------------------------\n";
        return 1;
    }
}

void ASSERT_UINT256(const uint256& A, const uint256& B, const std::string& TEST_MESSAGE) {
    std::cout << TEST_NAME << ":" << TEST_MESSAGE << " : ";
    if(A!=B) {
        std::cout << "FAILED\n";
        TEST_RESULTS.push_back(false);
        #ifdef PRINT_FAILED_OUTPUTS
        A.printHex();
        B.printHex();
        std::cout << "\n";
        #endif
    }
    else {
        std::cout << "PASSED\n";
        TEST_RESULTS.push_back(true);
    }
}