#ifndef uidMRDCVLSC_uint128_HPP
#define uidMRDCVLSC_uint128_HPP

#include <iostream>
#include <bitset>

class uint128 {
    
    public:

    unsigned long *data;

    uint128(unsigned long msq, unsigned long lsq) {
        data = new unsigned long[2];
        data[0] = msq;
        data[1] = lsq;
    }

    // copy constructor
    uint128(const uint128& src) {
        data = new unsigned long[2];
        data[0] = src.msq();
        data[1] = src.lsq();
    }

    // move constructor
    uint128(uint128&& src) noexcept {
        data = src.data;
        src.data = NULL;
    }

    // copy assignment
    uint128& operator=(const uint128& src) {
        data[0] = src.msq();
        data[1] = src.lsq();
        return *this;
    }

    // move assignment
    uint128& operator=(uint128&& src) {
        if(data != NULL)
            delete [] data;

        data = src.data;
        src.data = NULL;
        return *this;
    }

    ~uint128(){
        if(data != NULL)
            delete [] data;
    }

    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint128 
    unsigned long& msq() { return data[0]; }
    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint128 
    unsigned long& lsq() { return data[1]; }
    /// returns the most significant QUADWORD, or the upper uint64 halve of the uint128 
    const unsigned long& msq() const { return data[0]; }
    /// returns the least significant QUADWORD, or the lower uint64 halve of the uint128 
    const unsigned long& lsq() const { return data[1]; }

    bool operator==(const uint128& roperand) const {
        return (msq() == roperand.msq()) && (lsq() == roperand.lsq());
    }

    bool operator!=(const uint128& roperand) const {
        return !(*this == roperand);
    }

    uint128 operator+(const uint128& add) const {
        
        uint128 sum(msq(),lsq());

        unsigned long old_lsq = sum.lsq();
        unsigned long old_msq = sum.msq();

        sum.lsq() += add.lsq();
        sum.msq() += add.msq();

        if(sum.lsq() < old_lsq) {
            sum.msq()++;
        }

        return sum;
    }

    bool operator<(const uint128& right) const {
        if(msq()<right.msq()) {
            return true;
        }
        else if(msq()==right.msq()) {
            if(lsq()<right.lsq()) {
                return true;
            }
        }
        return false;
    }

    bool operator<=(const uint128& right) const {
        return (*this < right) || (*this == right);
    }

    bool operator>(const uint128& right) const {
        return !(*this<=right);
    }

    bool operator>=(const uint128& right) const {
        return !(*this<right) || (*this == right);
    }

    bool operator&&(const uint128& right) const {
        return (msq() || lsq()) && (right.msq() || right.lsq());
    }

    bool operator||(const uint128& right) const {
        return (msq() || lsq()) || (right.msq() || right.lsq());
    }

    bool operator!() const {
        return !(msq() || lsq());
    }

    uint128 operator~() const {
        return uint128(~msq(),~lsq());
    }

    uint128 operator^(const uint128& right) const {
        return uint128(msq()^right.msq(), lsq()^right.lsq());
    }

    uint128 operator|(const uint128& right) const {
        return uint128(msq()|right.msq(), lsq()|right.lsq());
    }

    uint128 operator&(const uint128& right) const {
        return uint128(msq()&right.msq(), lsq()&right.lsq());
    }

    uint128 operator-(const uint128& sub) const {

        uint128 dif(msq(),lsq());

        unsigned long old_lsq = dif.lsq();
        unsigned long old_msq = dif.msq();

        dif.lsq() -= sub.lsq();
        dif.msq() -= sub.msq();

        if(dif.lsq() > old_lsq) {
            dif.msq()--;
        }

        return dif;
    }

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
    uint128 operator*(const uint128& mul) const {
    
        uint128 product(0,0);

#if(__MINGW32__)
    #error "uint128 multiplication has no implementation yet for mingw32."
#elif(__GNUC__ || __GNUG__ || __clang__ || __MINGW64__)
    #if (__x86_64__ || __ia64__ ||__amd__64__)
        asm volatile(
            "mov %[mr1], %%rax\n\t"
            "mul %[mc1]\n\t"
            "mov %%rax, %[pd3]\n\t"
            "mov %%rdx, %[pd2]\n\t"
            "mov %[mr1], %%rax\n\t"
            "mul %[mc0]\n\t"
            "add %%rax, %[pd2]\n\t"
            "mov %[mr0], %%rax\n\t"
            "mul %[mc1]\n\t"
            "add %%rax, %[pd2]"
            :[pd2]"+r"(product.msq()),[pd3]"+r"(product.lsq())
            :[mr0]"r"(mul.msq()), [mr1]"r"(mul.lsq()), [mc0]"r"(data[0]), [mc1]"r"(data[1])
            : "rax", "rdx", "memory", "cc"
        );
    #else
        #error "uint128 multiplication has no implementation yet for x86 architectures."
    #endif
#elif defined(_MSC_VER)
    #error "uint128 multiplication has no implementation yet for Microsoft Visual C++ Compiler."
#else
    #error "Unknown system : not supported"
#endif

        return product;
    }

    /** this is only for dividing two uint128 where the divisor's msq is zero
     * and the dividen has values in both it's own msq:lsq
     * 
     * msq - most significant quadword
     * lsq - least significant quadword
    */
    uint128 ep_div(const uint128& divisor)  const {
        uint128(0,0);
        // get the msq of the quotient
        unsigned long msqr = 0;


        return uint128(0,0);
    }

    /** long division using bits, shifts and subtract */
    uint128 ss_div(const uint128& divisor) const {
        
        uint128 quotient(0,0), pdvn(0,0), bit(0,0), one(0,1);

        for(size_t i=0; i<128; ++i) {
            
            pdvn = pdvn << 1;
            quotient = quotient << 1;

            bit = *this << i;
            bit = bit >> 127;

            pdvn = pdvn ^ bit;

            if(pdvn>=divisor) {
                quotient = quotient ^ one;

                pdvn = pdvn - divisor;
            }
        }

        return quotient;
    }

    uint128 operator/(const uint128& divisor) const {
        if(divisor.msq()==0 && divisor.lsq()==0) {
            throw std::domain_error("division by zero is not possible");
        }
        else if(*this == divisor) {
            return uint128(0,1); // remainder zero
        }
        else if(*this < divisor) {
            return uint128(0,0); // remainder *this (dividen)
        }
        return uint128(0,0);
    }

    uint128 operator<<(size_t lshift) const {
        uint128 lshifted = *this;
        if(lshift < (sizeof(unsigned long)*8)) {
            unsigned long msq_carry = lshifted.lsq() >> ((sizeof(unsigned long)*8)-lshift);
            lshifted.lsq() <<= lshift;
            lshifted.msq() <<= lshift;
            lshifted.msq() |= msq_carry;
        }
        else if(lshift >= (sizeof(unsigned long)*8) && lshift < (sizeof(unsigned long)*8*2)){
            lshift -= (sizeof(unsigned long)*8);
            lshifted.msq() = lshifted.lsq();
            lshifted.lsq() = 0;
            lshifted.msq() = lshifted.msq() << lshift;
        }
        else {
            lshifted.msq() = lshifted.lsq() = 0;
        }
        return lshifted;
    }

    uint128 operator>>(size_t rshift) const {
        uint128 rshifted = *this;
        if(rshift < (sizeof(unsigned long)*8)) {
            unsigned long lsq_carry = rshifted.msq() << ((sizeof(unsigned long)*8)-rshift);
            rshifted.msq() >>= rshift;
            rshifted.lsq() >>= rshift;
            rshifted.lsq() |= lsq_carry;
        }
        else if(rshift >= (sizeof(unsigned long)*8) && rshift < (sizeof(unsigned long)*8*2)){
            rshift -= (sizeof(unsigned long)*8);
            rshifted.lsq() = rshifted.msq();
            rshifted.msq() = 0;
            rshifted.lsq() >>= rshift;
        }
        else {
            rshifted.msq() = rshifted.lsq() = 0;
        }
        return rshifted;
    }

    uint128& operator<<=(size_t lshift) {
        if(lshift < (sizeof(unsigned long)*8)) {
            unsigned long msq_carry = lsq() >> ((sizeof(unsigned long)*8)-lshift);
            lsq() <<= lshift;
            msq() <<= lshift;
            msq() |= msq_carry;
        }
        else if(lshift >= (sizeof(unsigned long)*8) && lshift < (sizeof(unsigned long)*8*2)){
            lshift -= (sizeof(unsigned long)*8);
            msq() = lsq();
            lsq() = 0;
            msq() = msq() << lshift;
        }
        else {
            msq() = lsq() = 0;
        }
        return *this;
    }

    uint128& operator>>=(size_t rshift) {
        if(rshift < (sizeof(unsigned long)*8)) {
            unsigned long lsq_carry = msq() << ((sizeof(unsigned long)*8)-rshift);
            msq() >>= rshift;
            lsq() >>= rshift;
            lsq() |= lsq_carry;
        }
        else if(rshift >= (sizeof(unsigned long)*8) && rshift < (sizeof(unsigned long)*8*2)){
            rshift -= (sizeof(unsigned long)*8);
            lsq() = msq();
            msq() = 0;
            lsq() >>= rshift;
        }
        else {
            msq() = lsq() = 0;
        }
        return *this;
    }

    void printHex() const {
        printf("0x%016lx%016lx\n",msq(),lsq());
    }

    void printHex_separated() const {
        printf("0x%016lx|0x%016lx\n",msq(),lsq());
    }

    void printBits() const {
        std::bitset<64> msq(data[0]);
        std::bitset<64> lsq(data[1]);

        std::cout << msq << lsq << "\n";
    }

    void printBits_separated() const {
        std::bitset<64> msq(data[0]);
        std::bitset<64> lsq(data[1]);

        std::cout << msq << "|" << lsq << "\n";
    }
};

#endif