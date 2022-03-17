CC := g++
CPPFLAGS := -g -Og
CXXFLAGS := -std=c++11 -Wall -Wextra

OS := $(shell uname)

ifeq ($(OS), Linux)
CPPFLAGS += -fsanitize=address
endif

SRC := tests
SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ := $(patsubst $(SRC)/%.cpp,$(SRC)/%.out,$(SRC_FILES))

# -------------------------- run test programs ---------------------------

test: $(OBJ)
	@./$(SRC)/QuarterRound_test.out
	@./$(SRC)/BlockFunction_test.out
	@./$(SRC)/Encryption_test.out
	@./$(SRC)/uint128_shifts_test.out
	@./$(SRC)/uint128_comparisons_test.out
	@./$(SRC)/uint128_add_test.out
	@./$(SRC)/uint128_mul_test.out
	@./$(SRC)/uint128_sub_test.out
	@./$(SRC)/uint128_div_test.out
	@./$(SRC)/uint128_assign_add_test.out
	@./$(SRC)/uint128_assign_mul_test.out
	@./$(SRC)/uint128_assign_sub_test.out
	@./$(SRC)/uint128_div_ope_assign_test.out
	@./$(SRC)/uint128_div_operator_test.out
	@./$(SRC)/uint256_mul_test.out
	@./$(SRC)/uint256_shifts_test.out
	@./$(SRC)/uint256_div_test.out
	@./$(SRC)/uint256_mod_test.out
	@./$(SRC)/poly1305_mac_test.out

# test: $(OBJ) # not working for some reasons
# 	@echo "running test programs"
# 	@for o in $(SRC)/*.out; do ./$$o; done

# -------------------------- test program compilation ---------------------------

$(SRC)/%.out: $(SRC)/%.cpp
	@echo "compiling test program - compiler : $(CC)"
	@$(CC) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

clean:
	@echo "deleting compiled test programs"
	@rm ./$(SRC)/*.out

# install:
# 	@ln -s $(dir $(abspath $(lastword $(MAKEFILE_LIST))))/chacha20 /usr/local/include/

# uninstall:
# 	@unlink /usr/local/include/chacha20