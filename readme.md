# ChaCha20 & Poly1305

A from scratch C++ (with slight inline assembly) implementation of ```ChaCha20 & Poly1305``` stream cipher described in [RFC - 8439](https://www.rfc-editor.org/info/rfc8439).

**tests:**

![gcc-gnu](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/gcc-gnu.yml/badge.svg)
![clang](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/clang.yml/badge.svg)
![mingw32](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/mingw32.yml/badge.svg)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

All test from RFC-8439 is implemented here and is inside the ```tests``` folder.

-----

# Environment Requirements
- x86_64 architecture
- little endian system

If your system does not have these requirements, it might produce wrong results or might not even compile.

-----

# Main Functions

Note: one **byte** is equivalent to one ```unsigned char```, If we have an array with a size of **5 bytes**, that is an array of type ```unsigned char``` with **5 elements**.

```c++
/**Encryption.
 * 
 * @param outputCipher resulting cipher text output, 
 * The size of outputCipher in bytes is equivalent to textLen.
 * 
 * @param outputTag resulting 128bit / 16 byte array,
 * GENERATED after ENCRYPTION.
 * 
 * @param inputText plain text to be ENCRYPTED.
 * 
 * @param textLen size of the inputText in bytes.
 * 
 * @param AAD Arbitrary length additional authenticated data.
 * 
 * @param AAD_len size of the AAD in bytes.
 * 
 * @param key a 256-bit key or 32 byte unsigned char array.
 * 
 * @param nonce number only once.
*/
ChaCha20_Poly1305::aead_encrypt(
    unsigned char       *outputCipher,
    unsigned char       *outputTag,
    const unsigned char *inputText,
    size_t               textLen,
    const unsigned char *AAD,
    size_t               AAD_len,
    const unsigned char *key,
    const unsigned char *nonce
);

/**Decryption.
 * 
 * @param outputText recovered plain text output, 
 * The size of outputText in bytes is equivalent to cipherLen.
 * 
 * @param outputTag resulting 128bit / 16 byte array,
 * GENERATED after DECRYPTION.
 * 
 * @param inputCipher cipher text to be DECRYPTED.
 * 
 * @param cipherLen size of the input inputCipher in bytes.
 * 
 * @param AAD Arbitrary length additional authenticated data.
 * 
 * @param AAD_len size of the AAD in bytes.
 * 
 * @param key a 256-bit key or 32 byte unsigned char array.
 * 
 * @param nonce number only once.
*/
ChaCha20_Poly1305::aead_decrypt(
    unsigned char       *outputText,
    unsigned char       *outputTag,
    const unsigned char *inputCipher,
    size_t               cipherLen,
    const unsigned char *AAD,
    size_t               AAD_len,
    const unsigned char *key,
    const unsigned char *nonce
);
```

**To know if the message is authentic:**

The **outputTag** of generated by encryption should match the **outputTag** generated by decryption.

1. The sender will **send** the encrypted **cipher text** along with the **encrypted tag**.

2. Then the receiver will decrypt the cipher text, the decryption function will generate the output **recovered plain text** and the output **decryption tag**.

3. The receiver then compares the **sender's encryption tag** to the generated output **decryption tag**; if both tag matched / is-equal then the message was authentic / not altered.

-----


# Compilation

There are **TWO WAYS** to use the library, either using the **header only mode**, or building the **static library**.

- ## **Using header only**

    Directly include the **Header-Mode-Chaha20-Poly1305.hpp** in your source code, and you can directly compile it without building the static library.

    ```c++
    #include "Header-Mode-Chaha20-Poly1305.hpp"
    ```

    But you always need to enable the optimization flags for this method to work.

    ```shell
    g++ main.cpp main.out -O2
    ```

    ```shell
    clang++ main.cpp main.out -O2
    ```

<br>

- ## **Static Build with GNU-GCC Linux makefiles**

    Build & install static library
    ```shell
    make -f staticlib
    sudo make -f staticlib install
    ```

    Run tests (optional)
    ```
    make -f staticlib build_test
    ```

    Include ```#include <ChaCha20-Poly1305.hpp>``` in your program then compile with **-lchacha20**

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

<br>

- ## **Static Build with Clang Linux makefiles**

    Build static library with clang
    ```shell
    make -f staticlib CC=clang++
    sudo make -f staticlib install
    ```

    Run tests with clang (optional)
    ```
    make -f staticlib build_test CC=clang++
    ```

    Include ```#include <ChaCha20-Poly1305.hpp>``` in your program then compile with **-lchacha20**

    ```
    clang++ main.cpp -o main.out -lchacha20
    ```

    To uninstall library
    ```
    sudo make -f staticlib uninstall
    ```

    Clean Up compiled objects
    ```
    make -f staticlib cleanup
    ```

<br>

- ## **Static Build with Mingw64 Windows makefiles**
    
    Build & install static library
    ```shell
    mingw32-make -f staticlib
    mingw32-make -f staticlib install INSTALL_PREFIX=(path/to/mingw64)
    ```

    Run tests (optional)
    ```
    mingw32-make -f staticlib build_test
    ```

    Include ```#include <ChaCha20-Poly1305.hpp>``` in your program then compile with **-lchacha20**

    ```
    g++ main.cpp -o main.out -lchacha20
    ```

    To uninstall library
    ```
    mingw32-make -f staticlib uninstall INSTALL_PREFIX=(path/to/mingw64)
    ```

    Clean Up compiled objects
    ```
    mingw32-make -f staticlib cleanup
    ```

You can also modify the installation path in linux by setting up a new value for the ```INSTALL_PREFIX=``` when executing the make build commands similar to what is shown in the windows build.