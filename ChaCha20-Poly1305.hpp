#ifndef idMRDCVLSC_ChaCha20_POLY1305_HP
#define idMRDCVLSC_ChaCha20_POLY1305_HP

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

/// uint512 least significant limb index.
#define UINT512_LS_LIMB 0

/// uint512 most significant limb index.
#define UINT512_MS_LIMB 7

#define LESS -1
#define EQUAL 0
#define GREAT 1

#if (__MINGW64__ || __MINGW64)
typedef unsigned long long ulongint;
#define PRINT_LIMBHEX "%016llx "
#elif (__clang__ || __GNUC__ || __GNUG__)
typedef unsigned long ulongint;
#define PRINT_LIMBHEX "%016lx "
#else
#error not supported
#endif

/// for intel & amd x86_64 & x64 architectures only.
class uint512 {
    public:

    /**least significant quadword starting from index 0
     * up to index 7 the most significant quadword.*/
    ulongint *limbs;

    uint512();
    uint512(ulongint num);
    uint512(const unsigned char *input_bytes, size_t bytes);
    uint512(
        ulongint n7, ulongint n6, ulongint n5, ulongint n4,
        ulongint n3, ulongint n2, ulongint n1, ulongint n0
    );

    /// copy constructor.
    uint512(const uint512& src);

    /// move constructor.
    uint512(uint512&& src) noexcept;

    /// copy assignment.
    uint512& operator=(const uint512& src);

    /// move assignment.
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

    /// @return returns true if the uint512 is not zero.
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

    /// This is the ugly part.
    uint512 operator*(const uint512& mr) const;
    uint512& operator*=(const uint512& mul);

    /** long division using bits, shifts and subtract. */
    uint512 ss_div(const uint512& divisor) const;

    /** long division using bits, shifts and subtract. */
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
#define CHACHA20_STATE_BYTES 64

/// Number of dwords(unsigned int) inside a ChaCha20 State.
#define CHACHA20_STATE_DWORDS 16

/// Number of bytes(unsigned char) inside a ChaCha20 key.
#define CHACHA20_KEY_BYTES 32

/// Number of dwords(unsigned int) inside a ChaCha20 key.
#define CHACHA20_KEY_DWORDS 8

/// Number of bytes(unsigned char) inside a ChaCha20 nonce.
#define CHACHA20_NONCE_BYTES 12

/// Number of dwords(unsigned int) inside a ChaCha20 nonce.
#define CHACHA20_NONCE_DWORDS 3

/// Number of bytes(unsigned char) inside a ChaCha20 block function output.
#define CHACHA20_BLK_FUNC_OUTPUT_BYTES 64

/// Number of dwords(unsigned int) inside a ChaCha20 block function output.
#define CHACHA20_BLK_FUNC_OUTPUT_DWORDS 16

/// Number of bytes(unsigned char) inside a Poly1305 tag/mac output.
#define POLY1305_MAC_BYTES 16

/// Index of the counter in the chacha state.
#define CHACHA_STATE_COUNTER_INDEX 12

/// Half of the size of the key in bytes.
#define HALF_KEY_BYTES 16

/// get the padding needed for the textLen to become a multiple of 16
#define PADDING16(textLen) (textLen%16==0) ? 0 : (16-(textLen%16))

/// internal namespace containing functions for chacha20 in aead encryption.
namespace chacha20
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
        unsigned int       *output,
        const unsigned int *key,
        unsigned int        counter,
        const unsigned int *nonce);

    /**Transforms an initial ChaCha state.
     * 
     * @note the input and output are both 16 unsigned int arrays.
     * 
     * @param output this is where the result of the transformed ChaCha state will go.
     * @param input this is the initial ChaCha state.*/
    void apply_20rounds(unsigned int *output, const unsigned int *input);

    /**The ChaCha20 Block Encryption.
     * @param key A 256-bit key, treated as a concatenation of eight 32-bit little-endian integers.
     * @param counter a 32 bit unsigned integer block counter, this is essential when
     * initializing ChaCha states for different ChaCha blocks, It creates uniqueness for different blocks.
     * @param nonce a number only once.
     * @param inputText plain text message to be encrypted.
     * @param textLen size of the message in bytes.
     * @return returns an unsigned char* (pointer) that contains the cipher text (heap size should be equal to inputText's textLen).
     * 
     * WARNING!!! - THE POINTER ASSIGNED TO THIS FUNCTION SHOULD NOT BE ALLOCATED YET, THE ALLOCATION
     * WILL HAPPED INSIDE THIS FUNCTION, AND AFTER THE ASSIGNED POINTER IS USED, IT
     * SHOULD BE FREED USING THE delete [] KEYWORD!.
    */
    unsigned char *encrypt( // function parameters
        const unsigned char *key,
        unsigned int         counter,
        const unsigned char *nonce,
        const unsigned char *inputText,
        size_t               textLen
    );

    /**The ChaCha20 Block Encryption.
     * @param outputCipher encrypted plaintext message,
     * heap size should be equal to the inputText's textLen.
     * @param key A 256-bit key, treated as a concatenation of eight 32-bit little-endian integers.
     * @param counter a 32 bit unsigned integer block counter, this is essential when
     * initializing ChaCha states for different ChaCha blocks, It creates uniqueness for different blocks.
     * @param nonce a number only once.
     * @param inputText plain text message to be encrypted.
     * @param textLen size of the message in bytes.
     * */
    void encrypt( // function parameters
        unsigned char       *outputCipher,
        const unsigned char *key,
        unsigned int         counter,
        const unsigned char *nonce,
        const unsigned char *inputText,
        size_t               textLen
    );
}

/// internal namespace containing functions for poly1305 in aead encryption.
namespace poly1305 {

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
     * @param counter optional argument, this defaults to 0.
    */
    void key_gen(unsigned char *output, const unsigned char *key, const unsigned int *nonce, unsigned int counter=0);
}

namespace ChaCha20_Poly1305
{   
    /**Encryption.
     * 
     * Note: the nonce is produced inside this function by concatenating
     * the constant and iv; (constant|iv).
     * 
     * @param outputCipher resulting cipher text output.
     * @param outputTag resulting 128-bit/16-bytes of unsigned
     * char array output, GENERATED after ENCRYPTION.
     * @param inputText plain text to be ENCRYPTED.
     * @param textLen size of the inputText in bytes.
     * @param AAD Arbitrary length additional authenticated data.
     * @param AAD_len size of the input AAD in bytes.
     * @param key a 256-bit key or 32 byte unsigned char array.
     * @param iv initialization vector or IV.
     * @param constant a 32-bit unsigned integer.
    */
    void aead_encrypt(
        unsigned char       *outputCipher,
        unsigned char       *outputTag,
        const unsigned char *inputText,
        size_t               textLen,
        const unsigned char *AAD,
        size_t               AAD_len,
        const unsigned char *key,
        const unsigned char *iv,
        const unsigned char *constant
    );

    /**Decryption.
     * 
     * Note: the nonce is produced inside this function by concatenating
     * the constant and iv; (constant|iv).
     * 
     * @param outputText recovered plain text output.
     * @param outputTag this is the resulting 128-bit/16-bytes of unsigned
     * char array output, GENERATED after DECRYPTION.
     * @param inputCipher cipher text to be DECRYPTED.
     * @param cipherLen size of the inputCipher in bytes.
     * @param AAD Arbitrary length additional authenticated data.
     * @param AAD_len size of the input add in bytes.
     * @param key a 256-bit key or 32 byte unsigned char array.
     * @param iv initialization vector or IV.
     * @param constant a 32-bit unsigned integer.
    */
    void aead_decrypt(
        unsigned char       *outputText,
        unsigned char       *outputTag,
        const unsigned char *inputCipher,
        size_t               cipherLen,
        const unsigned char *AAD,
        size_t               AAD_len,
        const unsigned char *key,
        const unsigned char *iv,
        const unsigned char *constant
    );

    // ----------------------------------

    /**Encryption.
     * 
     * @param outputCipher resulting cipher text output.
     * @param outputTag resulting 128-bit/16-bytes of unsigned
     * char array output, GENERATED after ENCRYPTION.
     * @param inputText plain text to be ENCRYPTED.
     * @param textLen size of the inputText in bytes.
     * @param AAD Arbitrary length additional authenticated data.
     * @param AAD_len size of the AAD in bytes.
     * @param key a 256-bit key or 32 byte unsigned char array.
     * @param nonce number only once.
    */
    void aead_encrypt(
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
     * @param outputText recovered plain text output.
     * @param outputTag resulting 128-bit/16-bytes of unsigned
     * char array output, GENERATED after DECRYPTION.
     * @param inputCipher cipher text to be DECRYPTED.
     * @param cipherLen size of the input inputCipher in bytes.
     * @param AAD Arbitrary length additional authenticated data.
     * @param AAD_len size of the AAD in bytes.
     * @param key a 256-bit key or 32 byte unsigned char array.
     * @param nonce number only once.
    */
    void aead_decrypt(
        unsigned char       *outputText,
        unsigned char       *outputTag,
        const unsigned char *inputCipher,
        size_t               cipherLen,
        const unsigned char *AAD,
        size_t               AAD_len,
        const unsigned char *key,
        const unsigned char *nonce
    );
}

#endif