#include <iostream>
#include <utility>
#include <vector>
#include "../uint256.hpp"

#define PRINT_FAILED_OUTPUTS 1
std::vector<bool> TEST_RESULTS;
const static std::string TEST_NAME = "uint256 division";
void ASSERT_UINT256(const uint256& A, const uint256& B, const std::string& TEST_MESSAGE);

int main() {
    std::cout << "\n---------------------------------\n";
    std::cout << TEST_NAME << "\n=================================\n";

    // TEST VARIABLES
    uint256 MAX(uint128(0xffffffffffffffff,0xffffffffffffffff),uint128(0xffffffffffffffff,0xffffffffffffffff));

    // ANSWERS
    uint256 MAXMAX = MAX / MAX;

    // TEST CORRECT ANSWER
    uint256 MAXMAX_CORRECT(uint128(0,0), uint128(0,1));

    // TESTING ANSWER
    ASSERT_UINT256(MAXMAX,MAXMAX_CORRECT,"MAX/MAX");

    // SUMMARY OF RESULTS
    size_t failed_cnt = 0;
    for(auto e : TEST_RESULTS) {
        if(!e) failed_cnt++;
    }

    std::cout << "---------------------------------\n";
    std::cout << TEST_NAME << " RESULT:" << "\n";
    if(!failed_cnt) {
        std::cout << "ALL test PASSED\n";
        std::cout << "---------------------------------\n";
        return 0;
    }
    else {
        std::cout << "SOME test FAILED\n";
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