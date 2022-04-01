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
    make -f staticlib build_test
    sudo make -f staticlib install
    make -f staticlib cleanup
    ```

    - Then you can now use it in your programs.

    ```
    g++ main.cpp -o main.out -lchacha20
    ```

    - To unsintall
    ```
    sudo make -f staticlib uninstall
    ```

    - If compiling with mingw64 you need to specify INSTALL_PREFIX with the path your mingw64 is located example
    ```
    make -f staticlib install INSTALL_PREFIX="C:/User/Mingw64"
    ```