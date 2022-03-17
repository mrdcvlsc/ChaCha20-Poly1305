#ifndef uidMRDCVLSC_uint128_HPP
#define uidMRDCVLSC_uint128_HPP

#include <iostream>
#include <bitset>
#include <utility>

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

    void swapHighLow();

    void printHex() const;

    void printHex_separated() const;

    void printBits() const;

    void printBits_separated() const;
};

#ifndef MAKE_LIB
    #include "uint128.cpp"
#endif

#endif