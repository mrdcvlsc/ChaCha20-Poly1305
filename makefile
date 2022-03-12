CC := g++
CPPFLAGS := -Og
CXXFLAGS := -std=c++11

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
	@./$(SRC)/int128_add_test.out
	@./$(SRC)/int128_mul_test.out
	@./$(SRC)/int128_sub_test.out
	@./$(SRC)/int128_shifts_test.out
	@./$(SRC)/int128_comparisons_test.out
	

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