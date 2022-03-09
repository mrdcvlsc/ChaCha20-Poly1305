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
	@echo "running test programs"
	@for o in $(SRC)/*.out; do ./$$o; done

# -------------------------- test program compilation ---------------------------

$(SRC)/%.out: $(SRC)/%.cpp
	@echo "compiling test program"
	@g++ $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

clean:
	@echo "deleting compiled test programs"
	@rm ./tests/*.out

# install:
# 	@ln -s $(dir $(abspath $(lastword $(MAKEFILE_LIST))))/chacha20 /usr/local/include/

# uninstall:
# 	@unlink /usr/local/include/chacha20