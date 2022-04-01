#ifndef uidMRDCVLSC_ChaCha20_POLY1305_HPP
#define uidMRDCVLSC_ChaCha20_POLY1305_HPP

#include <iostream>
#include <cstring>
#include <cmath>
#include <bitset>
#include <utility>

//============================== uint512 ==============================

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

// -----------------------------

#define UINT64BYTES 8
#define UINT64BITS 64

#define UINT512LIMBS 8
#define UINT512BYTES 64
#define UINT511BITS 511
#define UINT512BITS 512
#define UINT512BITS_x2 1024

/// uint512 least significant limb index
#define UINT512_LS_LIMB 0

/// uint512 most significant limb index
#define UINT512_MS_LIMB 7

#define LESS -1
#define EQUAL 0
#define GREAT 1

/// for intel & amd x86_64 & x64 architectures only
class uint512 {
    public:

    /**least significant quadword starting from index 0
     * up to index 7 the most significant quadword.*/
    unsigned long *limbs;

    uint512();

    uint512(unsigned long num);
    
    uint512(const unsigned char *input_bytes, size_t bytes);

    uint512(
        unsigned long n7, unsigned long n6, unsigned long n5, unsigned long n4,
        unsigned long n3, unsigned long n2, unsigned long n1, unsigned long n0
    );

    /// copy constructor
    uint512(const uint512& src);

    /// move constructor
    uint512(uint512&& src) noexcept;

    /// copy assignment
    uint512& operator=(const uint512& src);

    /// move assignment
    uint512& operator=(uint512&& src);

    ~uint512();

    /// @return returns; 0 : if uint512 == 0, 1 : if uint512 == 1, and -1 : if uint512 != to 0 or 1.
    int one_or_zero() const;

    /// @return returns; -1 : if less than, 0 : if equal, 1 : if greater than.
    int compare(const uint512& with) const;

    bool operator==(const uint512& with) const;

    bool operator!=(const uint512& with) const;

    bool operator<(const uint512& with) const;

    bool operator>(const uint512& with) const;

    bool operator<=(const uint512& with) const;

    bool operator>=(const uint512& with) const;

    /// @return returns true if the uint512 is not zero
    bool boolean() const;

    bool operator&&(const uint512& with) const;

    bool operator||(const uint512& with) const;

    bool operator!() const;

    uint512 operator&(const uint512& with) const;

    uint512 operator|(const uint512& with) const;

    uint512 operator^(const uint512& with) const;

    uint512 operator~() const;

    uint512& operator&=(const uint512& with);

    uint512& operator|=(const uint512& with);

    uint512& operator^=(const uint512& with);

    uint512 operator+(const uint512& add) const;

    uint512& operator+=(const uint512& add);

    uint512 operator-(const uint512& sub) const;

    uint512& operator-=(const uint512& sub);

    /// This is the ugly part
    uint512 operator*(const uint512& mr) const;

    uint512& operator*=(const uint512& mul);

    /** long division using bits, shifts and subtract */
    uint512 ss_div(const uint512& divisor) const;

    /** long division using bits, shifts and subtract */
    uint512 ss_mod(const uint512& divisor) const;

    uint512 operator/(const uint512& divisor) const;

    uint512& operator/=(const uint512& divisor);

    uint512 operator%(const uint512& divisor) const;

    uint512& operator%=(const uint512& divisor);

    // LEFT SHIFT
    
    uint512 operator<<(size_t lshift) const;

    uint512& operator<<=(size_t lshift);

    // RIGHT SHIFT 

    uint512 operator>>(size_t rshift) const;

    uint512& operator>>=(size_t rshift);

    /// the limb[7] will be printed first then 6,5, ..., the limb[0] will be printed last.
    void printHex() const;

    /// the limb[7] will be printed first then 6,5, ..., the limb[0] will be printed last.
    void printBits() const;
};

//=====================================================================

/// Number of bytes(unsigned char) inside a ChaCha20 State.
static const int __CHAx220_STATE_BYTES__ = 64;

/// Number of dwords(unsigned int) inside a ChaCha20 State.
static const int __CHAx220_STATE_DWORDS__ = 16;

/// Number of bytes(unsigned char) inside a ChaCha20 key.
static const int __CHAx220_KEY_BYTES__ = 32;

/// Number of dwords(unsigned int) inside a ChaCha20 key.
static const int __CHAx220_KEY_DWORDS__ = 8;

/// Number of bytes(unsigned char) inside a ChaCha20 nonce.
static const int __CHAx220_NONCE_BYTES__ = 12;

/// Number of dwords(unsigned int) inside a ChaCha20 nonce.
static const int __CHAx220_NONCE_DWORDS__ = 3;

/// Number of bytes(unsigned char) inside a ChaCha20 block function output.
static const int __CHAx220_BLK_FUNC_OUTPUT_BYTES__ = 64;

/// Number of dwords(unsigned int) inside a ChaCha20 block function output.
static const int __CHAx220_BLK_FUNC_OUTPUT_DWORDS__ = 16;


/// Number of bytes(unsigned char) inside a Poly1305 tag/mac output.
static const int __POLY1305_MAC_BYTES__ = 16;

#define HALF_KEY_BYTES 16
#define __CHACHA_STATE_COUNTER_INDEX__ 12

#define HALF_KEY_BYTES 16

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
        const unsigned int *key,
        unsigned int counter,
        const unsigned int *nonce);

    /**Transforms an initial ChaCha state.
     * 
     * @note the input and output are both 16 unsigned int arrays.
     * 
     * @param output this is where the result of the transformed ChaCha state will go.
     * @param input this is the initial ChaCha state.*/
    void apply_20rounds( // function parameters
        unsigned int *output,
        const unsigned int *input);
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
    void mac(unsigned char *output, const unsigned char *key, const unsigned char* msg, size_t msg_len);

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
    void key_gen(unsigned char *output, const unsigned char *key, const unsigned int *nonce, unsigned int counter=0);
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
        const unsigned char *key,
        unsigned int counter,
        const unsigned char *nonce,
        const unsigned char *plaintext,
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
        const unsigned char *key,
        unsigned int counter,
        const unsigned char *nonce,
        const unsigned char *plaintext,
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
        const unsigned char *plaintext, size_t text_len,
        const unsigned char *aad, size_t aad_len, const unsigned char *key,
        const unsigned char *iv, const unsigned char *constant
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
        const unsigned char *ciphertext, size_t text_len,
        const unsigned char *aad, size_t aad_len, const unsigned char *key,
        const unsigned char *iv, const unsigned char *constant
    );
}

#endif