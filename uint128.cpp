#ifndef _UINT128_CPP_mrdcvlsc_
#define _UINT128_CPP_mrdcvlsc_
#include <iostream>

#ifdef MAKE_LIB
#include "ChaCha20-Poly1305.hpp"
#endif

uint128::uint128() {
    data = NULL;
}

uint128::uint128(unsigned long msq, unsigned long lsq) {
    data = new unsigned long[2];
    data[0] = msq;
    data[1] = lsq;
}

// copy constructor
uint128::uint128(const uint128& src) {
    data = new unsigned long[2];
    data[0] = src.msq();
    data[1] = src.lsq();
}

// move constructor
uint128::uint128(uint128&& src) noexcept {
    data = src.data;
    src.data = NULL;
}

// copy assignment
uint128& uint128::operator=(const uint128& src) {
    if(data == NULL)
        data = new unsigned long[2];

    data[0] = src.msq();
    data[1] = src.lsq();
    return *this;
}

// move assignment
uint128& uint128::operator=(uint128&& src) {
    if(data != NULL)
        delete [] data;

    data = src.data;
    src.data = NULL;
    return *this;
}

uint128::~uint128(){
    if(data != NULL)
        delete [] data;
}

/// returns the most significant QUADWORD, or the upper uint64 halve of the uint128.
unsigned long& uint128::msq() { return data[0]; }

/// returns the least significant QUADWORD, or the lower uint64 halve of the uint128.
unsigned long& uint128::lsq() { return data[1]; }

/// returns the most significant QUADWORD, or the upper uint64 halve of the uint128.
const unsigned long& uint128::msq() const { return data[0]; }

/// returns the least significant QUADWORD, or the lower uint64 halve of the uint128.
const unsigned long& uint128::lsq() const { return data[1]; }

bool uint128::operator==(const uint128& roperand) const {
    return (msq() == roperand.msq()) && (lsq() == roperand.lsq());
}

bool uint128::operator!=(const uint128& roperand) const {
    return !(*this == roperand);
}

bool uint128::operator<(const uint128& right) const {
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

bool uint128::operator<=(const uint128& right) const {
    return (*this < right) || (*this == right);
}

bool uint128::operator>(const uint128& right) const {
    return !(*this<=right);
}

bool uint128::operator>=(const uint128& right) const {
    return !(*this<right) || (*this == right);
}

bool uint128::operator&&(const uint128& right) const {
    return (msq() || lsq()) && (right.msq() || right.lsq());
}

bool uint128::operator||(const uint128& right) const {
    return (msq() || lsq()) || (right.msq() || right.lsq());
}

bool uint128::operator!() const {
    return !(msq() || lsq());
}

uint128 uint128::operator~() const {
    return uint128(~msq(),~lsq());
}

uint128 uint128::operator^(const uint128& right) const {
    return uint128(msq()^right.msq(), lsq()^right.lsq());
}

uint128& uint128::operator^=(const uint128& right) {
    msq()^=right.msq(), lsq()^=right.lsq();
    return *this;
}

uint128 uint128::operator|(const uint128& right) const {
    return uint128(msq()|right.msq(), lsq()|right.lsq());
}

uint128& uint128::operator|=(const uint128& right) {
    msq()|=right.msq(), lsq()|=right.lsq();
    return *this;
}

uint128 uint128::operator&(const uint128& right) const {
    return uint128(msq()&right.msq(), lsq()&right.lsq());
}

uint128& uint128::operator&=(const uint128& right) {
    msq()&=right.msq(), lsq()&=right.lsq();
    return *this;
}

uint128 uint128::operator+(const uint128& add) const {
    
    uint128 sum(msq(),lsq());

    unsigned long old_lsq = sum.lsq();
    // unsigned long old_msq = sum.msq();

    sum.lsq() += add.lsq();
    sum.msq() += add.msq();

    if(sum.lsq() < old_lsq) {
        sum.msq()++;
    }

    return sum;
}

uint128& uint128::operator+=(const uint128& add) {

    unsigned long old_lsq = lsq();
    // unsigned long old_msq = msq();

    lsq() += add.lsq();
    msq() += add.msq();

    if(lsq() < old_lsq) {
        msq()++;
    }

    return *this;
}

uint128& uint128::operator++() {

    unsigned long old_lsq = lsq();
    // unsigned long old_msq = msq();

    lsq() += 1;
    msq() += 0;

    if(lsq() < old_lsq) {
        msq()++;
    }

    return *this;
}

uint128& uint128::operator++(int) {
    return ++*this;
}

uint128 uint128::operator-(const uint128& sub) const {

    uint128 dif(msq(),lsq());

    unsigned long old_lsq = dif.lsq();
    // unsigned long old_msq = dif.msq();

    dif.lsq() -= sub.lsq();
    dif.msq() -= sub.msq();

    if(dif.lsq() > old_lsq) {
        dif.msq()--;
    }

    return dif;
}

uint128& uint128::operator-=(const uint128& sub) {

    unsigned long old_lsq = lsq();
    // unsigned long old_msq = msq();

    lsq() -= sub.lsq();
    msq() -= sub.msq();

    if(lsq() > old_lsq) {
        msq()--;
    }

    return *this;
}

uint128& uint128::operator--() {

    unsigned long old_lsq = lsq();
    // unsigned long old_msq = msq();

    lsq() -= 1;
    msq() -= 0;

    if(lsq() > old_lsq) {
        msq()--;
    }

    return *this;
}

uint128& uint128::operator--(int) {
    return --*this;
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
uint128 uint128::operator*(const uint128& mul) const {

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


// add with carry
std::pair<uint128,uint128> uint128::__fadd(const uint128& add) const {
    
    uint128 sum(msq(),lsq()), carry(0,0);

    unsigned long old_lsq = sum.lsq();
    unsigned long old_msq = sum.msq();

    sum.lsq() += add.lsq();
    sum.msq() += add.msq();

    if(sum.lsq() < old_lsq) {
        sum.msq()++;
    }

    if(sum.msq() < old_msq) {
        carry++;
    }

    return {carry,sum};
}

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
std::pair<uint128,uint128> uint128::__fmull(const uint128& mul) const {

    uint128 HIGH(0,0);
    uint128 LOW(0,0);

#if(__MINGW32__)
#error "uint128 multiplication has no implementation yet for mingw32."
#elif(__GNUC__ || __GNUG__ || __clang__ || __MINGW64__)
#if (__x86_64__ || __ia64__ ||__amd__64__)
    asm volatile(
        "mov %[mc1], %%rax\n\t"
        "mul %[mr1]\n\t"
        "mov %%rax, %[pd3]\n\t"
        "mov %%rdx, %[pd2]\n\t"

        "mov %[mc0], %%rax\n\t"
        "mul %[mr1]\n\t"
        "add %%rax, %[pd2]\n\t"
        "add %%rdx, %[pd1]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mul %[mr0]\n\t"
        "add %%rax, %[pd2]\n\t"
        "adc %%rdx, %[pd1]\n\t"
        "adc $0, %[pd0]\n\t"

        "mov %[mc0], %%rax\n\t"
        "mul %[mr0]\n\t"
        "add %%rax, %[pd1]\n\t"
        "add %%rdx, %[pd0]"

        :[pd0]"+r"(HIGH.msq()),[pd1]"+r"(HIGH.lsq()),[pd2]"+r"(LOW.msq()),[pd3]"+r"(LOW.lsq())
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

    return {HIGH,LOW};
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
uint128& uint128::operator*=(const uint128& mul) {
    *this = *this * mul;
    return *this;
}

// // UNFINISHED
// /** this is only for dividing uint128 to a uint64,
//  * this takes advantage of the "div" assembly instruction*/
// uint128 uint128::ep_div(unsigned int divisor)  const {

//     // first we assign the value of the dividen to the quotient.
//     // this is because the "div" instruction works on rdx:rax,
//     // treating is as the dividen at first, then proceeds to store
//     // the outputs:
//     // quotient to the rax register
//     // remainder to the rdx register 
//     uint128 quotient = *this;
//     // unsigned long remainder = 0;

//     if(divisor>msq()) {
//         // If the divisor is less than 264, but greater than the upper half of the dividend,
//         // the upper halves of quotient and remainder are 0, and a single DIV instruction yields their lower halves.
// #if(__MINGW32__)
//     #error "uint128/uint64 division has no implementation yet for mingw32."
// #elif(__GNUC__ || __GNUG__ || __clang__ || __MINGW64__)
//     #if (__x86_64__ || __ia64__ ||__amd__64__)
//         asm volatile(
//             "mov %[qmsq], %%rdx\n\t"
//             "mov %[qlsq], %%rax\n\t"
//             "div %[dvsr]\n\t"
//             "mov %%rax, %[qlsq]\n\t"
//             // "mov %%rdx, %[rmndr]" // enable line to get remainder
//             :[qlsq]"+m"(quotient.lsq()) // , [rmndr]"=m"(remainder) // enable line to get remainder
//             :[qmsq]"m"(quotient.msq()), [dvsr]"m"(data[0])
//             : "rax", "rdx", "memory", "cc"
//         );
//     #else
//         #error "uint128/uint64 division has no implementation yet for x86 architectures."
//     #endif
// #elif defined(_MSC_VER)
//     #error "uint128/uint64 division has no implementation yet for Microsoft Visual C++ Compiler."
// #else
//     #error "Unknown system : not supported"
// #endif
//     }
//     else if(divisor<=msq()) {
//         // If the divisor is less than 264 and not greater than the upper half of the dividend,
//         // the upper half of the remainder is 0 too, while its lower half and (both halves of)
//         // the quotient are produced with two consecutive DIV instructions using the so-called
//         // long alias schoolbook division (and 64-bit numbers as digits) to avoid an overflow
//         // of the quotient.

//         // 1. divide the upper quad-word of the quotient with the divisor and get the upper_quotient
//         // 2. mod the upper quad-word of the quotient with the divisor and get get the upper_remainder
//         // 3. set the upper quad-word of the quotient to the upper_quotient
//         // 4. put the upper_remainder in the rdx register
//         // 5. put the lower quad-word in the rax register
//         // 6. divide the rdx:rax with the divisor using the "div" assembly instruction
//         // 7. then move the values of the rdx:rax to the msq():lsq() of the quotient
                    
//     }

//     return quotient;
// }

/** long division using bits, shifts and subtract */
uint128 uint128::ss_div(const uint128& divisor) const {
    
    uint128 quotient(0,0), pdvn(0,0), bit(0,0);

    for(size_t i=0; i<128; ++i) {

        pdvn <<= 1;
        quotient <<= 1;

        bit = *this << i;
        bit >>= 127;

        pdvn.lsq() |= bit.lsq();

        if(pdvn>=divisor) {
            pdvn -= divisor;
            quotient.lsq() |= 1;
        }
    }

    // pdvn is the remainder

    return quotient;
}

uint128 uint128::operator/(const uint128& divisor) const {
    if(divisor.msq()==0 && divisor.lsq()==0) {
        throw std::domain_error("division by zero is not possible");
    }
    else if(*this == divisor) {
        return uint128(0,1); // remainder zero
    }
    else if(*this < divisor) {
        return uint128(0,0); // remainder *this (dividen)
    }
    else {
        return this->ss_div(divisor);
    }
    return uint128(0,0);
}

uint128& uint128::operator/=(const uint128& divisor) {
    uint128 quotient(0,0);
    if(divisor.msq()==0 && divisor.lsq()==0) {
        throw std::domain_error("division by zero is not possible");
    }
    else if(*this == divisor) {
        quotient.msq() = 0;
        quotient.lsq() = 1;
        // remainder zero
    }
    else if(*this < divisor) {
        quotient.msq() = 0;
        quotient.lsq() = 0;
        // remainder *this (dividen)
    }
    else {
        quotient = this->ss_div(divisor);
    }
    *this = std::move(quotient);
    return *this;
}

uint128 uint128::operator<<(size_t lshift) const {
    uint128 lshifted = *this;
    if(!lshift) { /* Do nothing */ }
    else if(lshift < ULONGBITS) {
        unsigned long msq_carry = lshifted.lsq() >> (ULONGBITS-lshift);
        lshifted.lsq() <<= lshift;
        lshifted.msq() <<= lshift;
        lshifted.msq() |= msq_carry;
    }
    else if(lshift >= ULONGBITS && lshift < ULONGBITS_2x){
        lshift -= ULONGBITS;
        lshifted.msq() = lshifted.lsq();
        lshifted.lsq() = 0;
        lshifted.msq() = lshifted.msq() << lshift;
    }
    else {
        lshifted.msq() = lshifted.lsq() = 0;
    }
    return lshifted;
}

uint128 uint128::operator>>(size_t rshift) const {
    uint128 rshifted = *this;
    if(!rshift) { /* Do nothing */ }
    else if(rshift < ULONGBITS) {
        unsigned long lsq_carry = rshifted.msq() << (ULONGBITS-rshift);
        rshifted.msq() >>= rshift;
        rshifted.lsq() >>= rshift;
        rshifted.lsq() |= lsq_carry;
    }
    else if(rshift >= ULONGBITS && rshift < ULONGBITS_2x){
        rshift -= ULONGBITS;
        rshifted.lsq() = rshifted.msq();
        rshifted.msq() = 0;
        rshifted.lsq() >>= rshift;
    }
    else {
        rshifted.msq() = rshifted.lsq() = 0;
    }
    return rshifted;
}

uint128& uint128::operator<<=(size_t lshift) {
    if(!lshift) { /* Do nothing */ }
    else if(lshift < ULONGBITS) {
        unsigned long msq_carry = lsq() >> (ULONGBITS-lshift);
        lsq() <<= lshift;
        msq() <<= lshift;
        msq() |= msq_carry;
    }
    else if(lshift >= ULONGBITS && lshift < ULONGBITS_2x){
        lshift -= ULONGBITS;
        msq() = lsq();
        lsq() = 0;
        msq() = msq() << lshift;
    }
    else {
        msq() = lsq() = 0;
    }
    return *this;
}

uint128& uint128::operator>>=(size_t rshift) {
    if(!rshift) { /* Do nothing */ }
    else if(rshift < ULONGBITS) {
        unsigned long lsq_carry = msq() << (ULONGBITS-rshift);
        msq() >>= rshift;
        lsq() >>= rshift;
        lsq() |= lsq_carry;
    }
    else if(rshift >= ULONGBITS && rshift < ULONGBITS_2x){
        rshift -= ULONGBITS;
        lsq() = msq();
        msq() = 0;
        lsq() >>= rshift;
    }
    else {
        msq() = lsq() = 0;
    }
    return *this;
}

void uint128::swapHighLow() {
    std::swap(data[0],data[1]);
}

void uint128::printHex() const {
    printf("0x%016lx%016lx\n",msq(),lsq());
}

void uint128::printHex_separated() const {
    printf("0x%016lx|0x%016lx\n",msq(),lsq());
}

void uint128::printBits() const {
    std::bitset<64> msq(data[0]);
    std::bitset<64> lsq(data[1]);

    std::cout << msq << lsq << "\n";
}

void uint128::printBits_separated() const {
    std::bitset<64> msq(data[0]);
    std::bitset<64> lsq(data[1]);

    std::cout << msq << "|" << lsq << "\n";
}

#endif