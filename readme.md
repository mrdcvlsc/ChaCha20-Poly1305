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

1. **Static Build with GNU-GCC Linux makefiles**

    Build & install static library
    ```shell
    make -f staticlib
    sudo make -f staticlib install
    ```

    Run tests (optional)
    ```
    make -f staticlib build_test
    ```

    Include ```#include <ChaCha20-Poly1305.hpp>``` in your program the compile with.

    ```
    g++ main.cpp -o main.out -lchacha20
    ```

    To uninstall library
    ```
    sudo make -f staticlib uninstall
    ```

    Clean Up compiled objects
    ```
    make -f staticlib cleanup
    ```

2. **Static Build with Mingw64 windows makefiles**
    
    Build & install static library
    ```shell
    mingw32-make -f staticlib
    mingw32-make -f staticlib install INSTALL_PREFIX=(path/to/mingw64)
    ```

    Run tests (optional)
    ```
    mingw32-make -f staticlib build_test
    ```

    Include ```#include <ChaCha20-Poly1305.hpp>``` in your program the compile with.

    ```
    g++ main.cpp -o main.out -lchacha20
    ```

    To uninstall library
    ```
    mingw32-make -f staticlib uninstall
    ```

    Clean Up compiled objects
    ```
    mingw32-make -f staticlib cleanup
    ```

    - If compiling with mingw64 you need to specify INSTALL_PREFIX with the path your mingw64 is located example
    ```
    mingw32-make -f staticlib install INSTALL_PREFIX="C:/User/Mingw64"
    ```