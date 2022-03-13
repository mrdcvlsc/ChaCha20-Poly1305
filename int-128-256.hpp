#ifndef uidMRDCVLSC_int128_HPP
#define uidMRDCVLSC_int128_HPP

#include <iostream>
#include <bitset>

class int128 {
    
    public:

    unsigned long *data;

    int128(unsigned long msb, unsigned long lsb) {
        data = new unsigned long[2];
        data[0] = msb;
        data[1] = lsb;
    }

    // copy constructor
    int128(const int128& src) {
        data = new unsigned long[2];
        data[0] = src.msb();
        data[1] = src.lsb();
    }

    // move constructor
    int128(int128&& src) noexcept {
        data = src.data;
        src.data = NULL;
    }

    // copy assignment
    int128& operator=(const int128& src) {
        data[0] = src.msb();
        data[1] = src.lsb();
        return *this;
    }

    // move assignment
    int128& operator=(int128&& src) {
        if(data != NULL)
            delete [] data;

        data = src.data;
        src.data = NULL;
        return *this;
    }

    ~int128(){
        if(data != NULL)
            delete [] data;
    }

    unsigned long& msb() { return data[0]; }
    unsigned long& lsb() { return data[1]; }
    const unsigned long& msb() const { return data[0]; }
    const unsigned long& lsb() const { return data[1]; }

    bool operator==(const int128& roperand) const {
        return (msb() == roperand.msb()) && (lsb() == roperand.lsb());
    }

    bool operator!=(const int128& roperand) const {
        return !(*this == roperand);
    }

    int128 operator+(const int128& add) const {
        
        int128 sum(msb(),lsb());

        unsigned long old_lsb = sum.lsb();
        unsigned long old_msb = sum.msb();

        sum.lsb() += add.lsb();
        sum.msb() += add.msb();

        if(sum.lsb() < old_lsb) {
            sum.msb()++;
        }

        return sum;
    }

    bool operator<(const int128& right) const {
        if(msb()<right.msb()) {
            return true;
        }
        else if(msb()==right.msb()) {
            if(lsb()<right.lsb()) {
                return true;
            }
        }
        return false;
    }

    bool operator<=(const int128& right) const {
        return (*this < right) || (*this == right);
    }

    bool operator>(const int128& right) const {
        return !(*this<=right);
    }

    bool operator>=(const int128& right) const {
        return !(*this<right) || (*this == right);
    }

    bool operator&&(const int128& right) const {
        return (msb() || lsb()) && (right.msb() || right.lsb());
    }

    bool operator||(const int128& right) const {
        return (msb() || lsb()) || (right.msb() || right.lsb());
    }

    bool operator!() const {
        return !(msb() || lsb());
    }

    int128 operator~() const {
        return int128(~msb(),~lsb());
    }

    int128 operator^(const int128& right) const {
        return int128(msb()^right.msb(), lsb()^right.lsb());
    }

    int128 operator|(const int128& right) const {
        return int128(msb()|right.msb(), lsb()|right.lsb());
    }

    int128 operator&(const int128& right) const {
        return int128(msb()&right.msb(), lsb()&right.lsb());
    }

    int128 operator-(const int128& sub) const {

        int128 dif(msb(),lsb());

        unsigned long old_lsb = dif.lsb();
        unsigned long old_msb = dif.msb();

        dif.lsb() -= sub.lsb();
        dif.msb() -= sub.msb();

        if(dif.lsb() > old_lsb) {
            dif.msb()--;
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
    int128 operator*(const int128& mul) const {
    
        int128 product(0,0);

#if(__MINGW32__)
    #error "int128 multiplication has no implementation yet for mingw32."
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
            :[pd2]"+r"(product.msb()),[pd3]"+r"(product.lsb())
            :[mr0]"r"(mul.msb()), [mr1]"r"(mul.lsb()), [mc0]"r"(data[0]), [mc1]"r"(data[1])
            : "rax", "rdx", "memory", "cc"
        );
    #else
        #error "int128 multiplication has no implementation yet for x86 architectures."
    #endif
#elif defined(_MSC_VER)
    #error "int128 multiplication has no implementation yet for Microsoft Visual C++ Compiler."
#else
    #error "Unknown system : not supported"
#endif

        return product;
    }

    int128 ep_div(const int128& divisor)  const {

    }

    int128 ss_div(const int128& divisor) const {

    }

    int128 operator/(const int128& divisor) const {
        if(divisor.msb()==0 && divisor.lsb()==0) {
            throw std::domain_error("division by zero is not possible");
        }
        else if(*this == divisor) {
            return int128(0,1); // remainder zero
        }
        else if(*this < divisor) {
            return int128(0,0); // remainder *this (dividen)
        }
        return int128(0,0);
    }

    int128 operator<<(size_t lshift) const {
        int128 lshifted = *this;
        if(lshift < (sizeof(unsigned long)*8)) {
            unsigned long msb_carry = lshifted.lsb() >> ((sizeof(unsigned long)*8)-lshift);
            lshifted.lsb() <<= lshift;
            lshifted.msb() <<= lshift;
            lshifted.msb() |= msb_carry;
        }
        else if(lshift >= (sizeof(unsigned long)*8) && lshift < (sizeof(unsigned long)*8*2)){
            lshift -= (sizeof(unsigned long)*8);
            lshifted.msb() = lshifted.lsb();
            lshifted.lsb() = 0;
            lshifted.msb() = lshifted.msb() << lshift;
        }
        else {
            lshifted.msb() = lshifted.lsb() = 0;
        }
        return lshifted;
    }

    int128 operator>>(size_t rshift) const {
        int128 rshifted = *this;
        if(rshift < (sizeof(unsigned long)*8)) {
            unsigned long lsb_carry = rshifted.msb() << ((sizeof(unsigned long)*8)-rshift);
            rshifted.msb() >>= rshift;
            rshifted.lsb() >>= rshift;
            rshifted.lsb() |= lsb_carry;
        }
        else if(rshift >= (sizeof(unsigned long)*8) && rshift < (sizeof(unsigned long)*8*2)){
            rshift -= (sizeof(unsigned long)*8);
            rshifted.lsb() = rshifted.msb();
            rshifted.msb() = 0;
            rshifted.lsb() >>= rshift;
        }
        else {
            rshifted.msb() = rshifted.lsb() = 0;
        }
        return rshifted;
    }

    int128& operator<<=(size_t lshift) {
        if(lshift < (sizeof(unsigned long)*8)) {
            unsigned long msb_carry = lsb() >> ((sizeof(unsigned long)*8)-lshift);
            lsb() <<= lshift;
            msb() <<= lshift;
            msb() |= msb_carry;
        }
        else if(lshift >= (sizeof(unsigned long)*8) && lshift < (sizeof(unsigned long)*8*2)){
            lshift -= (sizeof(unsigned long)*8);
            msb() = lsb();
            lsb() = 0;
            msb() = msb() << lshift;
        }
        else {
            msb() = lsb() = 0;
        }
        return *this;
    }

    int128& operator>>=(size_t rshift) {
        if(rshift < (sizeof(unsigned long)*8)) {
            unsigned long lsb_carry = msb() << ((sizeof(unsigned long)*8)-rshift);
            msb() >>= rshift;
            lsb() >>= rshift;
            lsb() |= lsb_carry;
        }
        else if(rshift >= (sizeof(unsigned long)*8) && rshift < (sizeof(unsigned long)*8*2)){
            rshift -= (sizeof(unsigned long)*8);
            lsb() = msb();
            msb() = 0;
            lsb() >>= rshift;
        }
        else {
            msb() = lsb() = 0;
        }
        return *this;
    }

    void printHex() const {
        printf("0x%016lx%016lx\n",msb(),lsb());
    }

    void printHex_separated() const {
        printf("0x%016lx|0x%016lx\n",msb(),lsb());
    }

    void printBits() const {
        std::bitset<64> msb(data[0]);
        std::bitset<64> lsb(data[1]);

        std::cout << msb << lsb << "\n";
    }

    void printBits_separated() const {
        std::bitset<64> msb(data[0]);
        std::bitset<64> lsb(data[1]);

        std::cout << msb << "|" << lsb << "\n";
    }
};

#endif