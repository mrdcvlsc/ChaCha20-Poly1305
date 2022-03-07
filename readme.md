## ChaCha20 & Poly1305

this is a C++ implementation of ```ChaCha20 & Poly1305``` stream cipher described in [RFC - 8439](https://www.rfc-editor.org/info/rfc8439).

All information and content in this _readme.md_'s **Dev Notes** section about ```ChaCha20 & Poly1305``` came from or a pharaphrase of a certain sentence/paragraph/section in [RFC - 8439](https://www.rfc-editor.org/info/rfc8439)... except those bullets that has a **[dsi]** tag at the start which are _(me)developer's self interpretation_.

The [dsi] bullet points might possiblly be an inaccurate or incorrect description.

-----

## Dev Notes
- **ChaCha state** - The description of the ChaCha algorithm will at various time refer to the **ChaCha state** as a **"vector"** of 16 elements represented as a 4x4 **"matrix"**. The matrix notation is more _visually convenient_ and gives a better notion as to _why some rounds_ are called **"column rounds"** while others are called **"diagonal rounds"**.

```
0 1 2 3
4 5 6 7
8 9 10 11
12 13 14 15
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

- **The output ChaCha20 Block is a 64 random-looking bytes.**

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