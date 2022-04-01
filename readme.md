## ChaCha20 & Poly1305

**tests:**

![gcc-gnu](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/gcc-gnu.yml/badge.svg)
![clang](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/clang.yml/badge.svg)
![mingw32](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/mingw32.yml/badge.svg)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

-----

**To Be Improved Later**
- implement and use a uint320 instead of using uint512 to reduce operations in the poly1305 tag generation.

**Environment Requirements:**
- x86_64 architecture
- little endian system

If your system does not have these requirements, it might produce wrong results or might not even compile (32-bit computer/compilers are not supported).

-----

## How to Use

There are two ways to compile your program with this library, Either Using the **header only mode** or you can **build** it statically and install it in your system.

1. **Static Build with makefiles**

    ```shell
    make -f staticlib
    make -f staticlib cleanup
    sudo make -f staticlib install
    ```

    - You can also run static tests first to check if the program is producing the correct results before installing it with ```sudo make -f staticlib install```.

        ```shell
        make -f staticlib static_test
        ```

    - Then you can now use it in your programs.

make -f staticlib
make -f staticlib static_test
make -f staticlib clean_tests
make -f staticlib clean