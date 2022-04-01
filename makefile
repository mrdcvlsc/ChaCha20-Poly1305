CC := g++
CPPFLAGS := -g -Og -D_HIDE_WARNING
CXXFLAGS := -std=c++11 -Wall -Wextra

OS := $(shell uname)

ifeq ($(OS), Linux)
CPPFLAGS += -fsanitize=address
endif

SRC := tests
SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ := $(patsubst $(SRC)/%.cpp,$(SRC)/%.out,$(SRC_FILES))

# -------------------------- run test programs ---------------------------

header_test: $(OBJ)
	@echo "----------------------------------------------------"
	@echo "Running Initial Tests..."
	@./$(SRC)/QuarterRound_test.out
	@./$(SRC)/BlockFunction_test.out
	@./$(SRC)/Encryption_test.out
	@./$(SRC)/constructor.out
	@./$(SRC)/comparison.out
	@./$(SRC)/leftshifts.out
	@./$(SRC)/rightshifts.out
	@./$(SRC)/addition.out
	@./$(SRC)/subtraction.out
	@./$(SRC)/multiplication.out
	@./$(SRC)/division.out
	@./$(SRC)/poly1305_mac_test.out
	@./$(SRC)/poly1305_keygen.out
	@./$(SRC)/chacha20_aead_enc_dec.out
	@echo "----------------------------------------------------"
	@echo "Running Additional Tests..."
	@./$(SRC)/BlockFunction_test1.out
	@./$(SRC)/BlockFunction_test2.out
	@./$(SRC)/BlockFunction_test3.out
	@./$(SRC)/BlockFunction_test4.out
	@./$(SRC)/BlockFunction_test5.out
	@./$(SRC)/ChaCha20Encryption_t1.out
	@./$(SRC)/ChaCha20Encryption_t2.out
	@./$(SRC)/ChaCha20Encryption_t3.out

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