#ifndef uidMRDCVLSC_uint256_HPP
#define uidMRDCVLSC_uint256_HPP

#include <iostream>
#include <bitset>
#include "uint128.hpp"

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
     * Multiplication of two 128-bit int using 4-bit uint128's.
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
    uint256& operator*=(const uint256& mul);

    // UNFINISHED
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

#ifndef MAKE_LIB
    #include "uint256.cpp"
#endif

#endif