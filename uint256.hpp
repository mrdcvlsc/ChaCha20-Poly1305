#ifndef uidMRDCVLSC_uint256_HPP
#define uidMRDCVLSC_uint256_HPP

#include <iostream>
#include <bitset>
#include "uint128.hpp"

#define U256BITS 256
#define U255BITS 255
#define U256BITS_2x 512

const static uint128 __UINT128_CONSTANT_ZERO(0,0);
const static uint128 __UINT128_CONSTANT_ONE(0,1);
const static uint128 __UINT128_CONSTANT_TWO(0,2);

class uint256 {
    
    public:

    uint128 *dqdata;

    uint256(const uint128& msdq, const uint128& lsdq) {
        dqdata = new uint128[2];
        dqdata[0] = msdq;
        dqdata[1] = lsdq;
    }

    // copy constructor
    uint256(const uint256& src) {
        dqdata = new uint128[2];
        dqdata[0] = src.msdq();
        dqdata[1] = src.lsdq();
    }

    // move constructor
    uint256(uint256&& src) noexcept {
        dqdata = src.dqdata;
        src.dqdata = NULL;
    }

    // copy assignment
    uint256& operator=(const uint256& src) {
        dqdata[0] = src.msdq();
        dqdata[1] = src.lsdq();
        return *this;
    }

    // move assignment
    uint256& operator=(uint256&& src) {
        if(dqdata != NULL)
            delete [] dqdata;

        dqdata = src.dqdata;
        src.dqdata = NULL;
        return *this;
    }

    ~uint256(){
        if(dqdata != NULL)
            delete [] dqdata;
    }

    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint256.
    uint128& msdq() { return dqdata[0]; }
    
    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint256.
    uint128& lsdq() { return dqdata[1]; }

    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint256.
    const uint128& msdq() const { return dqdata[0]; }

    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint256.
    const uint128& lsdq() const { return dqdata[1]; }

    bool isOne() const {
        if((msdq().msq()==0 && msdq().lsq()==0) &&
           (lsdq().msq()==0 && lsdq().lsq()==1))
            return true;
        return false;
    }

    bool operator==(const uint256& roperand) const {
        return (msdq() == roperand.msdq()) && (lsdq() == roperand.lsdq());
    }

    bool operator!=(const uint256& roperand) const {
        return !(*this == roperand);
    }

    bool operator<(const uint256& right) const {
        if(msdq()<right.msdq()) {
            return true;
        }
        else if(msdq()==right.msdq()) {
            if(lsdq()<right.lsdq()) {
                return true;
            }
        }
        return false;
    }

    bool operator<=(const uint256& right) const {
        return (*this < right) || (*this == right);
    }

    bool operator>(const uint256& right) const {
        return !(*this<=right);
    }

    bool operator>=(const uint256& right) const {
        return !(*this<right) || (*this == right);
    }

    bool operator&&(const uint256& right) const {
        return (msdq() || lsdq()) && (right.msdq() || right.lsdq());
    }

    bool operator||(const uint256& right) const {
        return (msdq() || lsdq()) || (right.msdq() || right.lsdq());
    }

    bool operator!() const {
        return !(msdq() || lsdq());
    }

    uint256 operator~() const {
        return uint256(~msdq(),~lsdq());
    }

    uint256 operator^(const uint256& right) const {
        return uint256(msdq()^right.msdq(), lsdq()^right.lsdq());
    }

    uint256& operator^=(const uint256& right) {
        msdq()^=right.msdq(), lsdq()^=right.lsdq();
        return *this;
    }

    uint256 operator|(const uint256& right) const {
        return uint256(msdq()|right.msdq(), lsdq()|right.lsdq());
    }

    uint256& operator|=(const uint256& right) {
        msdq()|=right.msdq(), lsdq()|=right.lsdq();
        return *this;
    }

    uint256 operator&(const uint256& right) const {
        return uint256(msdq()&right.msdq(), lsdq()&right.lsdq());
    }

    uint256& operator&=(const uint256& right) {
        msdq()&=right.msdq(), lsdq()&=right.lsdq();
        return *this;
    }

    uint256 operator+(const uint256& add) const {
        
        uint256 sum(msdq(),lsdq());

        uint128 old_lsdq = sum.lsdq();
        uint128 old_msdq = sum.msdq();

        sum.lsdq() += add.lsdq();
        sum.msdq() += add.msdq();

        if(sum.lsdq() < old_lsdq) {
            sum.msdq()++;
        }

        return sum;
    }

    uint256& operator+=(const uint256& add) {

        uint128 old_lsdq = lsdq();
        uint128 old_msdq = msdq();

        lsdq() += add.lsdq();
        msdq() += add.msdq();

        if(lsdq() < old_lsdq) {
            msdq()++;
        }

        return *this;
    }

    uint256 operator-(const uint256& sub) const {

        uint256 dif(msdq(),lsdq());

        uint128 old_lsdq = dif.lsdq();
        uint128 old_msdq = dif.msdq();

        dif.lsdq() -= sub.lsdq();
        dif.msdq() -= sub.msdq();

        if(dif.lsdq() > old_lsdq) {
            dif.msdq()--;
        }

        return dif;
    }

    uint256& operator-=(const uint256& sub) {

        uint128 old_lsdq = lsdq();
        uint128 old_msdq = msdq();

        lsdq() -= sub.lsdq();
        msdq() -= sub.msdq();

        if(lsdq() > old_lsdq) {
            msdq()--;
        }

        return *this;
    }

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
    uint256 operator*(const uint256& mul) const {
        
        uint256 product(
            __UINT128_CONSTANT_ZERO,
            __UINT128_CONSTANT_ZERO
        );

        /// SLOWER VERSION
        // std::pair<uint128,uint128> UPPER12 = lsdq().__fmull(mul.lsdq());
        // uint128 UPPER1 = msdq() * mul.lsdq();
        // uint128 LOWER1 = lsdq() * mul.msdq();

        // product.lsdq() = std::move(UPPER12.second);
        // product.msdq() = std::move(UPPER12.first);
        // product.msdq() = product.msdq() + UPPER1;
        // product.msdq() = product.msdq() + LOWER1;
        
        /// FAST VERSION
#if(__MINGW32__)
    #error "uint256 multiplication has no implementation yet for mingw32."
#elif(__GNUC__ || __GNUG__ || __clang__ || __MINGW64__)
    #if (__x86_64__ || __ia64__ ||__amd__64__)
        asm volatile(

            "mov %[mc3], %%rax\n\t" // 1
            "mul %[mr3]\n\t"
            "mov %%rax, %[pd7]\n\t"
            "mov %%rdx, %[pd6]\n\t"

            "mov %[mc2], %%rax\n\t" // 2
            "mul %[mr3]\n\t"
            "add %%rax, %[pd6]\n\t"
            "mov %%rdx, %[pd5]\n\t"

            "mov %[mc1], %%rax\n\t" // 3
            "mul %[mr3]\n\t"
            "add %%rax, %[pd5]\n\t"
            "mov %%rdx, %[pd4]\n\t"
            
            "mov %[mc0], %%rax\n\t" // 4
            "mul %[mr3]\n\t"
            "add %%rax, %[pd4]\n\t"

            "mov %[mc3], %%rax\n\t" // 5
            "mul %[mr2]\n\t"
            "add %%rax, %[pd6]\n\t"
            "adc %%rdx, %[pd5]\n\t"
            "adc $0, %[pd4]\n\t"

            "mov %[mc2], %%rax\n\t" // 6
            "mul %[mr2]\n\t"
            "add %%rax, %[pd5]\n\t"
            "add %%rdx, %[pd4]\n\t"

            "mov %[mc1], %%rax\n\t" // 7
            "mul %[mr2]\n\t"
            "add %%rax, %[pd4]\n\t"

            "mov %[mc3], %%rax\n\t" // 8
            "mul %[mr1]\n\t"
            "add %%rax, %[pd5]\n\t"
            "adc %%rdx, %[pd4]\n\t"

            "mov %[mc2], %%rax\n\t" // 9
            "mul %[mr1]\n\t"
            "add %%rax, %[pd4]\n\t"

            "mov %[mc3], %%rax\n\t" // 10
            "mul %[mr0]\n\t"
            "add %%rax, %[pd4]\n\t"

            : // outputs
                [pd4]"+r"(product.msdq().msq()),
                [pd5]"+r"(product.msdq().lsq()),
                [pd6]"+r"(product.lsdq().msq()),
                [pd7]"+r"(product.lsdq().lsq())
            : // inputs 
                [mc0]"r"(msdq().msq()),
                [mc1]"r"(msdq().lsq()),
                [mc2]"r"(lsdq().msq()),
                [mc3]"r"(lsdq().lsq()),

                [mr0]"r"(mul.msdq().msq()),
                [mr1]"r"(mul.msdq().lsq()),
                [mr2]"r"(mul.lsdq().msq()),
                [mr3]"r"(mul.lsdq().lsq())
            : "rax", "rdx", "memory", "cc"
        );
    #else
        #error "uint256 multiplication has no implementation yet for x86 architectures."
    #endif
#elif defined(_MSC_VER)
    #error "uint256 multiplication has no implementation yet for Microsoft Visual C++ Compiler."
#else
    #error "Unknown system : not supported"
#endif

        return product;
    }

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
    uint256& operator*=(const uint256& mul) {
        *this = *this * mul;
        return *this;
    }

    // UNFINISHED
    /** this is only for dividing uint256 to a uint64,
     * this takes advantage of the "div" assembly instruction*/
    uint256 ep_div(const uint128& divisor)  const {

        // first we assign the value of the dividen to the quotient.
        // this is because the "div" instruction works on rdx:rax,
        // treating is as the dividen at first, then proceeds to store
        // the outputs:
        // quotient to the rax register
        // remainder to the rdx register 
        uint256 quotient = *this;
        uint128 remainder = __UINT128_CONSTANT_ZERO;

        if(divisor>msdq()) {
            // If the divisor is less than 264, but greater than the upper half of the dividend,
            // the upper halves of quotient and remainder are 0, and a single DIV instruction yields their lower halves.
#if(__MINGW32__)
    #error "uint256/uint64 division has no implementation yet for mingw32."
#elif(__GNUC__ || __GNUG__ || __clang__ || __MINGW64__)
    #if (__x86_64__ || __ia64__ ||__amd__64__)
        asm volatile(
            "mov %[qmsdq], %%rdx\n\t"
            "mov %[qlsdq], %%rax\n\t"
            "div %[dvsr]\n\t"
            "mov %%rax, %[qlsdq]\n\t"
            // "mov %%rdx, %[rmndr]" // enable line to get remainder
            :[qlsdq]"+m"(quotient.lsdq()) // , [rmndr]"=m"(remainder) // enable line to get remainder
            :[qmsdq]"m"(quotient.msdq()), [dvsr]"m"(dqdata[0])
            : "rax", "rdx", "memory", "cc"
        );
    #else
        #error "uint256/uint64 division has no implementation yet for x86 architectures."
    #endif
#elif defined(_MSC_VER)
    #error "uint256/uint64 division has no implementation yet for Microsoft Visual C++ Compiler."
#else
    #error "Unknown system : not supported"
#endif
        }
        else if(divisor<=msdq()) {
            // If the divisor is less than 264 and not greater than the upper half of the dividend,
            // the upper half of the remainder is 0 too, while its lower half and (both halves of)
            // the quotient are produced with two consecutive DIV instructions using the so-called
            // long alias schoolbook division (and 64-bit numbers as digits) to avoid an overflow
            // of the quotient.

            
        }

        return quotient;
    }

    /** long division using bits, shifts and subtract */
    uint256 ss_div(const uint256& divisor) const {
        
        uint256
            quotient(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO),
            pdvn(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO),
            bit(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO);

        for(size_t i=0; i<U256BITS; ++i) {

            pdvn <<= 1;
            quotient <<= 1;

            bit = *this << i;
            bit >>= U255BITS;

            pdvn.lsdq().lsq() |= bit.lsdq().lsq();

            if(pdvn>=divisor) {
                pdvn -= divisor;
                quotient.lsdq().lsq() |= 1;
            }
        }

        // pdvn is the remainder

        return quotient;
    }

    /** long division using bits, shifts and subtract */
    uint256 ss_mod(const uint256& divisor) const {
        
        uint256
            quotient(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO),
            pdvn(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO),
            bit(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO);

        for(size_t i=0; i<U256BITS; ++i) {

            pdvn <<= 1;
            quotient <<= 1;

            bit = *this << i;
            bit >>= U255BITS;

            pdvn.lsdq().lsq() |= bit.lsdq().lsq();

            if(pdvn>=divisor) {
                pdvn -= divisor;
                quotient.lsdq().lsq() |= 1;
            }
        }

        // pdvn is the remainder

        return pdvn;
    }

    uint256 operator/(const uint256& divisor) const {
        if(divisor.msdq()==__UINT128_CONSTANT_ZERO && divisor.lsdq()==__UINT128_CONSTANT_ZERO) {
            throw std::domain_error("division by zero is not possible");
        }
        else if(*this == divisor) {
            return uint256(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ONE); // remainder zero
        }
        else if(*this < divisor) {
            return uint256(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO); // remainder *this (dividen)
        }
        else if(divisor.isOne()) {
            return *this;
        }
        return this->ss_div(divisor);
    }

    uint256 operator%(const uint256& divisor) const {
        if(divisor.msdq()==__UINT128_CONSTANT_ZERO && divisor.lsdq()==__UINT128_CONSTANT_ZERO) {
            throw std::domain_error("% mod:division by zero is not possible");
        }
        else if(*this == divisor) {
            return uint256(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO); // remainder zero
        }
        else if(*this < divisor) {
            return *this; // remainder *this (dividen)
        }
        else if(divisor.isOne()) {
            return uint256(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO);  // remainder zero
        }
        return this->ss_mod(divisor);
    }

    uint256& operator/=(const uint256& divisor) {
        uint256 quotient(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO);
        if(divisor.msdq()==__UINT128_CONSTANT_ZERO && divisor.lsdq()==__UINT128_CONSTANT_ZERO) {
            throw std::domain_error("division by zero is not possible");
        }
        else if(*this == divisor) {
            quotient.msdq() = __UINT128_CONSTANT_ZERO;
            quotient.lsdq() = __UINT128_CONSTANT_ONE;
            // remainder zero
        }
        else if(*this < divisor) {
            quotient.msdq() = __UINT128_CONSTANT_ZERO;
            quotient.lsdq() = __UINT128_CONSTANT_ZERO;
            // remainder *this (dividen)
        }
        else if(divisor.isOne()) {
            return *this;
        }
        else {
            quotient = this->ss_div(divisor);
        }
        *this = std::move(quotient);
        return *this;
    }

    uint256& operator%=(const uint256& divisor) {
        uint256 remainder(__UINT128_CONSTANT_ZERO,__UINT128_CONSTANT_ZERO);
        if(divisor.msdq()==__UINT128_CONSTANT_ZERO && divisor.lsdq()==__UINT128_CONSTANT_ZERO) {
            throw std::domain_error("% mod:division by zero is not possible");
        }
        else if(*this == divisor) {
            remainder.msdq() = __UINT128_CONSTANT_ZERO;
            remainder.lsdq() = __UINT128_CONSTANT_ONE;
            // remainder zero
        }
        else if(*this < divisor) {
            return *this;
        }
        else if(divisor.isOne()) {
            remainder.msdq() = __UINT128_CONSTANT_ZERO;
            remainder.lsdq() = __UINT128_CONSTANT_ONE;
            // remainder zero
        }
        else {
            remainder = this->ss_mod(divisor);
        }
        *this = std::move(remainder);
        return *this;
    }

    uint256 operator<<(size_t lshift) const {
        uint256 lshifted = *this;
        if(!lshift) { /* Do nothing */ }
        else if(lshift < UINT128BITS) {
            uint128 msdq_carry = lshifted.lsdq() >> (UINT128BITS-lshift);
            lshifted.lsdq() <<= lshift;
            lshifted.msdq() <<= lshift;
            lshifted.msdq() |= msdq_carry;
        }
        else if(lshift >= UINT128BITS && lshift < UINT128BITS_2x){
            lshift -= UINT128BITS;
            lshifted.msdq() = lshifted.lsdq();
            lshifted.lsdq() = __UINT128_CONSTANT_ZERO;
            lshifted.msdq() = lshifted.msdq() << lshift;
        }
        else {
            lshifted.msdq() = lshifted.lsdq() = __UINT128_CONSTANT_ZERO;
        }
        return lshifted;
    }

    uint256 operator>>(size_t rshift) const {
        uint256 rshifted = *this;
        if(!rshift) { /* Do nothing */ }
        else if(rshift < UINT128BITS) {
            uint128 lsdq_carry = rshifted.msdq() << (UINT128BITS-rshift);
            rshifted.msdq() >>= rshift;
            rshifted.lsdq() >>= rshift;
            rshifted.lsdq() |= lsdq_carry;
        }
        else if(rshift >= UINT128BITS && rshift < UINT128BITS_2x){
            rshift -= UINT128BITS;
            rshifted.lsdq() = rshifted.msdq();
            rshifted.msdq() = __UINT128_CONSTANT_ZERO;
            rshifted.lsdq() >>= rshift;
        }
        else {
            rshifted.msdq() = rshifted.lsdq() = __UINT128_CONSTANT_ZERO;
        }
        return rshifted;
    }

    uint256& operator<<=(size_t lshift) {
        if(!lshift) { /* Do nothing */ }
        else if(lshift < UINT128BITS) {
            uint128 msdq_carry = lsdq() >> (UINT128BITS-lshift);
            lsdq() <<= lshift;
            msdq() <<= lshift;
            msdq() |= msdq_carry;
        }
        else if(lshift >= UINT128BITS && lshift < UINT128BITS_2x){
            lshift -= UINT128BITS;
            msdq() = lsdq();
            lsdq() = __UINT128_CONSTANT_ZERO;
            msdq() = msdq() << lshift;
        }
        else {
            msdq() = lsdq() = __UINT128_CONSTANT_ZERO;
        }
        return *this;
    }

    uint256& operator>>=(size_t rshift) {
        if(!rshift) { /* Do nothing */ }
        else if(rshift < UINT128BITS) {
            uint128 lsdq_carry = msdq() << (UINT128BITS-rshift);
            msdq() >>= rshift;
            lsdq() >>= rshift;
            lsdq() |= lsdq_carry;
        }
        else if(rshift >= UINT128BITS && rshift < UINT128BITS_2x){
            rshift -= UINT128BITS;
            lsdq() = msdq();
            msdq() = __UINT128_CONSTANT_ZERO;
            lsdq() >>= rshift;
        }
        else {
            msdq() = lsdq() = __UINT128_CONSTANT_ZERO;
        }
        return *this;
    }

    void printHex() const {
        printf("0x%016lx%016lx%016lx%016lx\n",msdq().msq(),msdq().lsq(),lsdq().msq(),lsdq().lsq());
    }

    void printHex_separated() const {
        printf("0x%016lx:%016lx|%016lx:%016lx\n",msdq().msq(),msdq().lsq(),lsdq().msq(),lsdq().lsq());
    }

    void printBits() const {
        std::bitset<64> msdq_msq(dqdata[0].msq());
        std::bitset<64> msdq_lsq(dqdata[0].lsq());

        std::bitset<64> lsdq_msq(dqdata[1].msq());
        std::bitset<64> lsdq_lsq(dqdata[1].lsq());

        std::cout << msdq_msq << msdq_lsq << lsdq_msq << lsdq_lsq << "\n";
    }

    void printBits_separated() const {
        std::bitset<64> msdq_msq(dqdata[0].msq());
        std::bitset<64> msdq_lsq(dqdata[0].lsq());

        std::bitset<64> lsdq_msq(dqdata[1].msq());
        std::bitset<64> lsdq_lsq(dqdata[1].lsq());

        std::cout << msdq_msq << ":" << msdq_lsq << "|" << lsdq_msq << ":" << lsdq_lsq << "\n";
    }
};

#endif