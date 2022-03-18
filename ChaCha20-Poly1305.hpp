#ifndef uidMRDCVLSC_ChaCha20_POLY1305_HPP
#define uidMRDCVLSC_ChaCha20_POLY1305_HPP

#include <iostream>
#include <cstring>
#include <cmath>
#include <bitset>
#include <utility>

#define HALF_KEY_BYTES 16
#define __CHACHA_STATE_COUNTER_INDEX__ 12

#define UINT128BITS 128
#define UINT128BITS_2x 256
#define UINT127BITS 127
#define UINT128BYTES 16

#if (__x86_64__ || __ia64__ ||__amd__64__)
#define ULONGBITS 64
#define ULONGBITS_2x 128
#else
#define ULONGBITS 32
#define ULONGBITS_2x 64
#endif

/// Number of bytes(unsigned char) inside a ChaCha20 State
static const int __CHAx220_STATE_BYTES__ = 64;

/// Number of dwords(unsigned int) inside a ChaCha20 State
static const int __CHAx220_STATE_DWORDS__ = 16;

/// Number of bytes(unsigned char) inside a ChaCha20 key
static const int __CHAx220_KEY_BYTES__ = 32;
/// Number of dwords(unsigned int) inside a ChaCha20 key
static const int __CHAx220_KEY_DWORDS__ = 8;

/// Number of bytes(unsigned char) inside a ChaCha20 nonce
static const int __CHAx220_NONCE_BYTES__ = 12;
/// Number of dwords(unsigned int) inside a ChaCha20 nonce
static const int __CHAx220_NONCE_DWORDS__ = 3;

/// Number of bytes(unsigned char) inside a ChaCha20 block function output
static const int __CHAx220_BLK_FUNC_OUTPUT_BYTES__ = 64;
/// Number of dwords(unsigned int) inside a ChaCha20 block function output
static const int __CHAx220_BLK_FUNC_OUTPUT_DWORDS__ = 16;

class uint128 {
    public:

    unsigned long *data;

    uint128();
    uint128(unsigned long msq, unsigned long lsq);

    // copy constructor
    uint128(const uint128& src);

    // move constructor
    uint128(uint128&& src) noexcept;

    // copy assignment
    uint128& operator=(const uint128& src);

    // move assignment
    uint128& operator=(uint128&& src);

    ~uint128();

    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint128.
    unsigned long& msq();

    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint128.
    unsigned long& lsq();

    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint128.
    const unsigned long& msq() const;

    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint128.
    const unsigned long& lsq() const;

    bool operator==(const uint128& roperand) const;
    bool operator!=(const uint128& roperand) const;
    bool operator<(const uint128& right) const;
    bool operator<=(const uint128& right) const;
    bool operator>(const uint128& right) const;
    bool operator>=(const uint128& right) const;
    bool operator&&(const uint128& right) const;
    bool operator||(const uint128& right) const;
    bool operator!() const;

    uint128 operator~() const;
    uint128 operator^(const uint128& right) const;
    uint128& operator^=(const uint128& right);
    uint128 operator|(const uint128& right) const;
    uint128& operator|=(const uint128& right);
    uint128 operator&(const uint128& right) const;
    uint128& operator&=(const uint128& right);
    uint128 operator+(const uint128& add) const;
    uint128& operator+=(const uint128& add);
    uint128& operator++();
    uint128& operator++(int);
    uint128 operator-(const uint128& sub) const;
    uint128& operator-=(const uint128& sub);
    uint128& operator--();
    uint128& operator--(int);

    /** 
     * Multiplication of two 128-bit int using 4-bit unsigned int's.
     * 
     * this function is taking advantage of the "rdx:rax" registers
     * and the "mul" assembly instruction to get the "rdx" or the
     * upper quad-word when multiplying two unsigned 64-bit integers
     * 
     * mc = multiplicand
     * mr = multiplier
     * pd = product
     * 
     * This is the normal multiplication used to get the 256-bit product
     * 
     *                     | mc0 | mc1 |
     *      x              | mr0 | mr1 |
     *      -------------------------------
     *         | pd0 | pd1 | pd2 | pd3 |
     * 
     * but here we omit the operations to get the pd0, and pd1 since
     * we only need the 128-bit low part of the product [pd2:pd3]
    */
    uint128 operator*(const uint128& mul) const;

    // add with carry
    std::pair<uint128,uint128> __fadd(const uint128& add) const;
    
    /** 
     * Full Product of Multiplication of two 128-bit int using 4-bit unsigned int's.
     * 
     * this function is taking advantage of the "rdx:rax" registers
     * and the "mul" assembly instruction to get the "rdx" or the
     * upper quad-word when multiplying two unsigned 64-bit integers.
     * 
     * mc = multiplicand.
     * mr = multiplier.
     * pd = product.
     * 
     * This is the normal multiplication used to get the 256-bit product.
     * 
     *                     | mc0 | mc1 |
     *      x              | mr0 | mr1 |
     *      -------------------------------
     *         | pd0 | pd1 | pd2 | pd3 |
    */
    std::pair<uint128,uint128> __fmull(const uint128& mul) const;

    /** 
     * Multiplication of two 128-bit int using 4-bit unsigned int's.
     * 
     * this function is taking advantage of the "rdx:rax" registers
     * and the "mul" assembly instruction to get the "rdx" or the
     * upper quad-word when multiplying two unsigned 64-bit integers
     * 
     * mc = multiplicand
     * mr = multiplier
     * pd = product
     * 
     * This is the normal multiplication used to get the 256-bit product
     * 
     *                     | mc0 | mc1 |
     *      x              | mr0 | mr1 |
     *      -------------------------------
     *         | pd0 | pd1 | pd2 | pd3 |
     * 
     * but here we omit the operations to get the pd0, and pd1 since
     * we only need the 128-bit low part of the product [pd2:pd3]
    */
    uint128& operator*=(const uint128& mul);

    // UNFINISHED
    /** this is only for dividing uint128 to a uint64,
     * this takes advantage of the "div" assembly instruction*/
    uint128 ep_div(unsigned int divisor)  const;

    /** long division using bits, shifts and subtract */
    uint128 ss_div(const uint128& divisor) const;

    uint128 operator/(const uint128& divisor) const;
    uint128& operator/=(const uint128& divisor);

    uint128 operator<<(size_t lshift) const;
    uint128 operator>>(size_t rshift) const;
    uint128& operator<<=(size_t lshift);
    uint128& operator>>=(size_t rshift);

    /// swaps the values of msq() and lsq()
    void swapHighLow();

    void printHex() const;
    void printHex_separated() const;
    void printBits() const;
    void printBits_separated() const;
};

#define U256BITS 256
#define U255BITS 255
#define U256BITS_2x 512

#define POLY1305_KEYBYTES 32

const static uint128 __UINT128_CONSTANT_ZERO(0,0);
const static uint128 __UINT128_CONSTANT_ONE(0,1);
const static uint128 __UINT128_CONSTANT_TWO(0,2);

class uint256 {
    public:

    uint128 *dqdata;

    /**This is a special constructor for the poly1305 algorithm.
     * 
     * This stores the 16 byte part of a key into the lower 128-bit
     * half of the uin256 and set the higher part to zero.
    */
    uint256(unsigned char *input_byte);

    uint256(const uint128& msdq, const uint128& lsdq);

    // copy constructor
    uint256(const uint256& src);

    // move constructor
    uint256(uint256&& src) noexcept;

    // copy assignment
    uint256& operator=(const uint256& src);

    // move assignment
    uint256& operator=(uint256&& src);

    ~uint256();

    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint256.
    uint128& msdq();
    
    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint256.
    uint128& lsdq();

    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint256.
    const uint128& msdq() const;

    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint256.
    const uint128& lsdq() const;

    /// @return returns true if the uin256 value is equal to 1
    bool isOne() const;

    bool operator==(const uint256& roperand) const;
    bool operator!=(const uint256& roperand) const;
    bool operator<(const uint256& right) const;
    bool operator<=(const uint256& right) const;
    bool operator>(const uint256& right) const;
    bool operator>=(const uint256& right) const;
    bool operator&&(const uint256& right) const;
    bool operator||(const uint256& right) const;
    bool operator!() const;

    uint256 operator~() const;
    uint256 operator^(const uint256& right) const;
    uint256& operator^=(const uint256& right);
    uint256 operator|(const uint256& right) const;
    uint256& operator|=(const uint256& right);
    uint256 operator&(const uint256& right) const;
    uint256& operator&=(const uint256& right);
    uint256 operator+(const uint256& add) const;
    uint256& operator+=(const uint256& add);
    uint256 operator-(const uint256& sub) const;
    uint256& operator-=(const uint256& sub);

    /** 
     * Multiplication of two 256-bit.
     * 
     * this function is taking advantage of the "rdx:rax" registers
     * and the "mul" assembly instruction to get the "rdx" or the
     * upper quad-word when multiplying two unsigned 64-bit integers.
     * 
     * mc = multiplicand.
     * mr = multiplier.
     * pd = product.
     * 
     * This is the normal multiplication used to get the 256-bit product.
     *
     * [0 - most] - [3 - least].
     * 
     *         | mc0 | mc1 | mc2 | mc3 |.
     *      x  | mr0 | mr1 | mr2 | mr3 |.
     *      -------------------------------.
     *         | pd4 | pd5 | pd6 | pd7 | .
     * 
     * here we omit the operations to get the pd0, pd1, pd2 and pd3 since
     * we only need the 256-bit low part of the product [pd4:pd5:pd6:pd7].
    */
    uint256 operator*(const uint256& mul) const;

    /** 
     * Multiplication of two 256-bit.
     * 
     * this function is taking advantage of the "rdx:rax" registers
     * and the "mul" assembly instruction to get the "rdx" or the
     * upper quad-word when multiplying two unsigned 64-bit integers.
     * 
     * mc = multiplicand.
     * mr = multiplier.
     * pd = product.
     * 
     * This is the normal multiplication used to get the 256-bit product.
     *
     * [0 - most] - [3 - least].
     * 
     *         | mc0 | mc1 | mc2 | mc3 |.
     *      x  | mr0 | mr1 | mr2 | mr3 |.
     *      -------------------------------.
     *         | pd4 | pd5 | pd6 | pd7 | .
     * 
     * here we omit the operations to get the pd0, pd1, pd2 and pd3 since
     * we only need the 256-bit low part of the product [pd4:pd5:pd6:pd7].
    */
    uint256& operator*=(const uint256& mul);

    // UNFINISHED - FOR OPTIMIZATION IN FUTURE
    /** this is only for dividing uint256 to a uint64,
     * this takes advantage of the "div" assembly instruction*/
    uint256 ep_div(const uint128& divisor)  const;

    /** long division using bits, shifts and subtract */
    uint256 ss_div(const uint256& divisor) const;

    /** long division using bits, shifts and subtract */
    uint256 ss_mod(const uint256& divisor) const;

    uint256 operator/(const uint256& divisor) const;
    uint256 operator%(const uint256& divisor) const;
    uint256& operator/=(const uint256& divisor);
    uint256& operator%=(const uint256& divisor);

    uint256 operator<<(size_t lshift) const;
    uint256 operator>>(size_t rshift) const;
    uint256& operator<<=(size_t lshift);
    uint256& operator>>=(size_t rshift);

    void printHex() const;
    void printHex_separated() const;
    void printBits() const;
    void printBits_separated() const;
};

namespace __internal_chacha20
{
    inline unsigned int bit_left_roll(unsigned int num, size_t n);

    void QUARTERROUND(unsigned int *state, size_t x, size_t y, size_t z, size_t w);

    /**Initializes a ChaCha state.
    * 
    * note The output is an unsigned int array with 16 elements.
    * 
    * @param output this is the resulting initial ChaCha state.
    * @param key a 32-bytes/256-bits key.
    * @param counter a 32 bit unsigned integer block counter, this is essential when
    * initializing ChaCha states for different ChaCha blocks, It creates uniqueness for different blocks.
    * @param nonce a 12-bytes/96-bits number only once, in some crypto scheme, this is called an IV.*/
    void init_state( // function parameters
        unsigned int *output,
        unsigned int *key,
        unsigned int counter,
        unsigned int *nonce);

    /**Transforms an initial ChaCha state.
     * 
     * @note the input and output are both 16 unsigned int arrays.
     * 
     * @param output this is where the result of the transformed ChaCha state will go.
     * @param input this is the initial ChaCha state.*/
    void apply_20rounds( // function parameters
        unsigned int *output,
        unsigned int *input);
}

namespace __internal_poly1305 {

    void clamp(unsigned char r[HALF_KEY_BYTES]);

    /**Generates the 16 byte(128-bits) tag using the 32 byte(256-bits) one time key
     * and an arbitrary length message.
     * 
     * @param output an `unsigned char` array with an element size of 16 where the output tag will be written.
     * @param key an `unsigned char` array with 32 elements (bytes).
     * @param msg an `unsigned char` array, this is the message.
     * @param msg_len the length of the `msg` array.
    */
    void mac(unsigned char *output, unsigned char *key, unsigned char* msg, size_t msg_len);

    /**Generate the one-time Poly1305 key pseudorandomly.
     * 
     * @param output a 32 byte unsigned char array where the generated key will be stored.
     * @param key aa 32 byte or 256-bit session integrity key.
     * @param nonce a number only once, this must be unique per invocation with he same key,
     * so it MUST NOT be randomly generated (because random number generation might produce
     * the same value). Instead a counter is a good way to get the value for the nonce instead.
     * other methods, such as a Linear Feedback Shift Register (LFSR) are also possible.
     * @param counter optional argument, this defaults to 0
    */
    void key_gen(unsigned char *output, unsigned char *key, unsigned int *nonce, unsigned int counter=0);
}

namespace ChaCha20_Poly1305
{
    /**The ChaCha20 Block Encryption.
     * @param key A 256-bit key, treated as a concatenation of eight 32-bit little-endian integers.
     * @param counter a 32 bit unsigned integer block counter, this is essential when
     * initializing ChaCha states for different ChaCha blocks, It creates uniqueness for different blocks.
     * @param nonce a number only once.
     * @param plaintext this is the message to be encrypted.
     * @param len this is the size of the message in bytes.
     * @return cipher_text this is the encrypted plaintext message,
     * the return value is an unsigned char pointer, the allocated heap size
     * is the as the plaintext length or the size of the message.
     * 
     * WARNING!!! - THE POINTER ASSIGNED TO THIS FUNCTION SHOULD NOT BE ALLOCATED YET, THE ALLOCATION
     * WILL HAPPED INSIDE THIS FUNCTION, AND AFTER THE ASSIGNED POINTER IS USED, IT
     * SHOULD BE FREED USING THE delete [] KEYWORD!.
    */
    unsigned char *encrypt( // function parameters
        unsigned char *key,
        unsigned int counter,
        unsigned char *nonce,
        unsigned char *plaintext,
        size_t len
    );

    /**The ChaCha20 Block Encryption.
     * @param cipher_text this is the encrypted plaintext message,
     * here it is a heap allocated unsigned char array pointer that
     * has the same length as the plaintext.
     * @param key A 256-bit key, treated as a concatenation of eight 32-bit little-endian integers.
     * @param counter a 32 bit unsigned integer block counter, this is essential when
     * initializing ChaCha states for different ChaCha blocks, It creates uniqueness for different blocks.
     * @param nonce a number only once.
     * @param plaintext this is the message to be encrypted.
     * @param len this is the size of the message in bytes.
     * */
    void encrypt( // function parameters
        unsigned char *cipher_text,
        unsigned char *key,
        unsigned int counter,
        unsigned char *nonce,
        unsigned char *plaintext,
        size_t len
    );

    /**This function gets the next multiple of 16 from a given number.
     *
     * @return returns the needed padding size in bytes.*/
    size_t pad16_size(size_t len);
    
    /**AEAD_CHACHA20_POLY1305 is an authenticated encryption.
     * 
     * Note: the nonce is produced inside this function by concatenating
     * the constant and iv; (constant|iv).
     * 
     * @param ciphertext this is the resulting cipher text output.
     * @param tag this is the resulting 128-bit/16-bytes of unsigned
     * char array output, GENERATED after ENCRYPTION, compare this to the decryption tag to check authenticity.
     * @param plaintext this is the input plain text that will be ENCRYPTED.
     * @param text_len this is the size of the input plaintext and the output ciphertext in bytes.
     * @param aad Arbitrary length additional authenticated data (AAD).
     * @param aad_len this is the size of the input add in bytes.
     * @param key a 256-bit key or 32 byte unsigned char array.
     * @param iv initialization vector or IV.
     * @param constant a 32-bit unsigned integer.
    */
    void aead_encrypt(
        unsigned char *ciphertext, unsigned char *tag,
        unsigned char *plaintext, size_t text_len,
        unsigned char *aad, size_t aad_len, unsigned char *key,
        unsigned char *iv, unsigned char *constant
    );

    /**AEAD_CHACHA20_POLY1305 authenticated decryption.
     * 
     * Note: the nonce is produced inside this function by concatenating
     * the constant and iv; (constant|iv).
     * 
     * @param plaintext this is the resulting recovered plain text output.
     * @param tag this is the resulting 128-bit/16-bytes of unsigned
     * char array output, GENERATED after DECRYPTION, compare this to the encryption tag to check authenticity.
     * @param ciphertext this is the input cipher text that will be DECRYPTED.
     * @param text_len this is the size of the input plaintext and the output ciphertext in bytes.
     * @param aad Arbitrary length additional authenticated data (AAD).
     * @param aad_len this is the size of the input add in bytes.
     * @param key a 256-bit key or 32 byte unsigned char array.
     * @param iv initialization vector or IV.
     * @param constant a 32-bit unsigned integer.
    */
    void aead_decrypt(
        unsigned char *plaintext, unsigned char *tag,
        unsigned char *ciphertext, size_t text_len,
        unsigned char *aad, size_t aad_len, unsigned char *key,
        unsigned char *iv, unsigned char *constant
    );
}

#endif