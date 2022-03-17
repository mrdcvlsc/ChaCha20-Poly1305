## ChaCha20 & Poly1305

**tests:**

![gcc-gnu](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/gcc-gnu.yml/badge.svg)
![clang](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/clang.yml/badge.svg)
![mingw32](https://github.com/mrdcvlsc/ChaCha20-Poly1305/actions/workflows/mingw32.yml/badge.svg)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

-----

this is a from scratch, C++ (with slight inline assembyl) implementation of ```ChaCha20 & Poly1305``` stream cipher described in [RFC - 8439](https://www.rfc-editor.org/info/rfc8439).

All information and content in this _readme.md_'s **Dev Notes** section about ```ChaCha20 & Poly1305``` came from or a pharaphrase of a certain sentence/paragraph/section in [RFC - 8439](https://www.rfc-editor.org/info/rfc8439)... except those bullets that has a **[dsi]** tag at the start which are _(me)developer's self interpretation_.

The [dsi] bullet points might possiblly be an inaccurate or incorrect description.

-----

## Dev Notes
- **ChaCha state** - The description of the ChaCha algorithm will at various time refer to the **ChaCha state** as a **"vector"** of 16 elements represented as a 4x4 **"matrix"**. The matrix notation is more _visually convenient_ and gives a better notion as to _why some rounds_ are called **"column rounds"** while others are called **"diagonal rounds"**.
    ```
    +-----------+
    | 0| 1| 2| 3|
    | 4| 5| 6| 7|
    | 8| 9|10|11|
    |12|13|14|15|
    +-----------+
    ```

- **ChaCha** - is the algorithm name

- **ChaCha20** - is a specific instance of ChaCha where **20 rounds** (or **80 quarter rounds**) are used.

- **ChaCha Quarter Round** - This is the basic operation of the algorithm. The "ChaCha Quarter Round" operates on four 32-bit ```unsigned int```; a, b, c & d.

- **[dsi] The bit left  roll operator (```<<<```)**
    ```c++
    // for little endian only
    unsigned int bit_left_roll(unsigned int num, size_t n)
    {
        unsigned int msb = (num << n);
        unsigned int lsb = (num >> 32-n);
        return msb | lsb;
    }
    ```

- **A Quarter Round on the ChaCha State** - The chacha works only on 4 of different index of the ChaCha state per quarter round ex: ```QUARTERROUND(1, 5, 9, 13)``` (Note that this run of quarter round is part of what is called a
 "column round".)
    ```
     0 *a  2  3
     4 *b  6  7
     8 *c 10 11
    12 *d 14 15
    ```

- **The ChaCha20 Block Function** - This function transforms a ChaCha state by running multiple quarter rounds

- **The inputs to ChaCha20 Block Function are:**
    - A 256-bit key, treated as a concatenation of eight 32-bit little-endian integers.
    - A 96-bit nonce, treated as a concatenation of three 32-bit little-endian integers.
    - A 32-bit block count parameter, treated as a 32-bit little-endian integer.

- **[dsi] ChaCha20 Sizes**

    - **ChaCha20 state : Size Equivalents**
        - 512-bits
        - 4x4 **32-bit** Matrix representation using 16 DWORD
        - 16 x **DWORD**s(32-bit), or sixteen ```unsigned int```
        - 64 x **BYTE**s(8-bit), or sixty-four ```unsigned char```

    - **Key : Size Equivalents**
        - 256-bits
        - 8 x **DWORD**s(32-bit), or eight ```unsigned int```
        - 32 x **BYTE**s(8-bit), or thrity-two ```unsigned char```

    - **Nonce : Size Equivalent**
        - 96-bits
        - 3 x **DWORD**s(32-bit), or three ```unsigned int```
        - 12 x **BYTE**s(8-bit), or twelve ```unsigned char```

    - **ChaCha20 Block Function Output : Size Equivalent**
        - 512-bits
        - 16 x **DWORD**s(32-bit), or sixteen ```unsigned int```
        - 64 x **BYTE**s(8-bit), or sixty-four ```unsigned char```

<br>

- **[dsi] Serializing the ChaCha20 Block output**

    ```c++
    unsigned int output_state[16];
    // ...
    // after some processing
    // ...
    unsigned char* serialize = (unsigned char*) output_state;
    ```

<br>

- **The ChaCha of RFC-8439**

    - The ChaCha algorithm described [here/RFC - 8439](https://www.rfc-editor.org/info/rfc8439) uses a **256-bit key**.

    - The _original algorithm_ also specified **128-bit** keys and an **8** and **12-round variants**, but these are out of scope for this [document/RFC - 8439](https://www.rfc-editor.org/info/rfc8439).

    - Note also that the original ChaCha had a 64-bit nonce and 64-bit block count. We have modified this here to be more consistent with recommendations in Section 3.2 of [RFC5116]. This limits the use of a single (key,nonce) combination to 2^32 blocks, or 256 GB, but that is enough for most uses. In cases where a single key is used by multiple senders, it is important to make sure that they don’t use the same nonces. This can be assured by partitioning the nonce space so that the first 32 bits are unique per sender, while the other 64 bits come from a counter.

- **The ChaCha20 state is initialized as follows:**

    - The first four words (0-3) are constants: ```0x61707865```, ```0x3320646e```, ```0x79622d32```, ```0x6b206574```.

    - The next eight words (4-11) are taken from the **256-bit key** by reading the bytes in little-endian order, in 4-byte chunks.

    - Word 12 is a block counter. Since each block is 64-byte, a 32-bit word is enough for 256 gigabytes of data.

    - Words 13-15 are a nonce, which MUST not be repeated for the same key. The 13th word is the first 32 bits of the input nonce taken as a little-endian integer, while the 15th word is the last 32 bits.

        ```
        cccccccc cccccccc cccccccc cccccccc
        kkkkkkkk kkkkkkkk kkkkkkkk kkkkkkkk
        kkkkkkkk kkkkkkkk kkkkkkkk kkkkkkkk
        bbbbbbbb nnnnnnnn nnnnnnnn nnnnnnnn

        c=constant k=key b=blockcount n=nonce
        ```
- **[dsi] ChaCha20 Block Encryption** - 
    1. get the **key_stream**, this is the 64 byte output of ChaCha20 Block Function.
    2. get the **plaintext_block**, this is a 64 byte part of a plaintext
    3. get the **cipher_text** by getting the XOR of **key_stream** & **plaintext_block**; ```cipher_text = plaintext_block ^ key_stream```

- **Poly1305** - a one-time authenticator designed by D. J. Bernstein.

    - Poly1305 takes a 32-byte one-time key and a message and produces a 16-byte tag. This tag is used to authenticate the message.

- **Inputs of Poly1305**
    - A 256-bit one-time key
    - An arbitrary length message

- **Output of Poly1305**
    - The output is a 128-bit tag.

- **Poly1305 key generation**
    
    Generate the one-time
    Poly1305 key pseudorandomly.

    To generate such a key pair (r,s), we will use the ChaCha20 block function.

    Note that in the AEAD construction defined in Section 2.8, the same key is used for encryption and key generation.

    Parameters:

    - The 256-bit session integrity key is used as the ChaCha20 key.

    - The block counter is set to zero.

    - The protocol will specify a 96-bit or 64-bit nonce. This MUST be unique per invocation with the same key, so it MUST NOT be randomly generated. A counter is a good way to implement this, but other methods, such as a Linear Feedback Shift Register (LFSR) are also . ChaCha20 as specified here requires a 96-bit nonce. So if the provided nonce is only 64-bit, then the first 32 bits of the nonce will be set to a constant number. This will usually be zero, but for protocols with multiple senders it may be different for each sender, but SHOULD be the same for all invocations of the function with the same key by a particular sender.

- **AEAD Construction**

    AEAD_CHACHA20_POLY1305 is an **authenticated encryption** with additional data algorithm.
    
    The **inputs** to AEAD_CHACHA20_POLY1305 are:

    - A 256-bit key = ```unsigned char key[32]```.
    - A 96-bit nonce = ```unsigned int[3]``` -- different for each invocation with the same key.
    - An arbitrary length plaintext.
    - Arbitrary length additional authenticated data (AAD).

    **Steps:**
    
    - First, a Poly1305 one-time key is generated from the 256-bit key and nonce.

    - Next, the ChaCha20 encryption function is called to encrypt the plaintext, using the same key and nonce, and with the initial counter set to 1.

    - Finally, the Poly1305 function is called with the Poly1305 key calculated above, and a message constructed as a concatenation of the following:

        - The AAD.
        - padding1 -- the padding is up to 15 zero bytes, and it brings the total length so far to an integral multiple of 16. If the length of the AAD was already an integral multiple of 16 bytes, this field is zero-length.
        - The ciphertext.
        - padding2 -- the padding is up to 15 zero bytes, and it brings the total length so far to an integral multiple of 16. If the length of the ciphertext was already an integral multiple of 16 bytes, this field is zero-length.
        - The length of the additional data in octets (as a 64-bit little-endian integer).
        - The length of the ciphertext in octets (as a 64-bit little-endian integer).

    - The **output** from the AEAD is the concatenation of:

        - A ciphertext of the same length as the plaintext.
        - A 128-bit tag, which is the output of the Poly1305 function.

    - **Decryption** is similar with the following differences:

        - The roles of ciphertext and plaintext are reversed, so the ChaCha20 encryption function is applied to the ciphertext, producing the plaintext.

        - The Poly1305 function is still run on the AAD and the ciphertext, not the plaintext.

        - The calculated tag is bitwise compared to the received tag. The message is authenticated if and only if the tags match.