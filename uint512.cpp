#ifndef uidMRDCVLSC_uint512_CPP
#define uidMRDCVLSC_uint512_CPP
#include <iostream>
#include <cstring>
#include <bitset>

#ifdef MAKE_LIB
#include "ChaCha20-Poly1305.hpp"
#endif

// #define DEVMODE
#ifdef DEVMODE
#include "ChaCha20-Poly1305.hpp"
#endif

uint512::uint512() {
    limbs = new unsigned long[UINT512LIMBS];
}

uint512::uint512(unsigned long num) {
    limbs = new unsigned long[UINT512LIMBS];
    limbs[0] = num;
    limbs[1] = 0UL;
    limbs[2] = 0UL;
    limbs[3] = 0UL;
    limbs[4] = 0UL;
    limbs[5] = 0UL;
    limbs[6] = 0UL;
    limbs[7] = 0UL;
}

uint512::uint512(const unsigned char *input_bytes, size_t bytes) {

    limbs = new unsigned long[UINT512LIMBS];

    if(bytes>UINT512BYTES)
        throw std::overflow_error("There should be only <= 64 bytes of data when initializing a uint512");

    size_t byte_offset = UINT512BYTES - bytes;    

    unsigned char *byte_form = (unsigned char*) limbs;

    memcpy(byte_form, input_bytes, bytes);
    memset(byte_form+bytes, 0x00, byte_offset);
}

uint512::uint512(
    unsigned long n7, unsigned long n6, unsigned long n5, unsigned long n4,
    unsigned long n3, unsigned long n2, unsigned long n1, unsigned long n0
) {

    limbs = new unsigned long[UINT512LIMBS];
    limbs[0] = n0;
    limbs[1] = n1;
    limbs[2] = n2;
    limbs[3] = n3;
    limbs[4] = n4;
    limbs[5] = n5;
    limbs[6] = n6;
    limbs[7] = n7;
}

/// copy constructor
uint512::uint512(const uint512& src) {

    limbs = new unsigned long[UINT512LIMBS];
    memcpy(limbs, src.limbs, UINT512BYTES);
}

/// move constructor
uint512::uint512(uint512&& src) noexcept {

    limbs = src.limbs;
    src.limbs = NULL;
}

/// copy assignment
uint512& uint512::operator=(const uint512& src) {

    if(limbs == NULL)
        limbs = new unsigned long[UINT512LIMBS];

    memcpy(limbs, src.limbs, UINT512BYTES);
    return *this;
}

/// move assignment
uint512& uint512::operator=(uint512&& src) {
    if(limbs != NULL)
        delete [] limbs;

    limbs = src.limbs;
    src.limbs = NULL;
    return *this;
}

uint512::~uint512(){
    if(limbs != NULL)
        delete [] limbs;
}

/// @return returns; 0 : if uint512 == 0, 1 : if uint512 == 1, and -1 : if uint512 != to 0 or 1.
int uint512::one_or_zero() const {

    if(limbs[UINT512_LS_LIMB] > 1)
        return -1;

    for(size_t i=1; i<UINT512LIMBS; ++i) {
        if(limbs[i])
            return -1;
    }

    return limbs[UINT512_LS_LIMB];
}

/// @return returns; -1 : if less than, 0 : if equal, 1 : if greater than.
int uint512::compare(const uint512& with) const {

    int value = 0;

    for(size_t i=0; i<UINT512LIMBS; ++i) {

        if(limbs[UINT512LIMBS-1-i]<with.limbs[UINT512LIMBS-1-i]) {
            value = -1;
            break;
        }

        if(limbs[UINT512LIMBS-1-i]>with.limbs[UINT512LIMBS-1-i]) {
            value = 1;
            break;
        }
    }
    return value;
}

bool uint512::operator==(const uint512& with) const {
    if(compare(with)==EQUAL)
        return true;
    return false;
}

bool uint512::operator!=(const uint512& with) const {
    return !(*this==with);
}

bool uint512::operator<(const uint512& with) const {
    if(compare(with)==LESS)
        return true;
    return false;
}

bool uint512::operator>(const uint512& with) const {
    if(compare(with)==GREAT)
        return true;
    return false;
}

bool uint512::operator<=(const uint512& with) const {
    int value = compare(with);
    if(value==LESS || value==EQUAL)
        return true;
    return false;
}

bool uint512::operator>=(const uint512& with) const {
    int value = compare(with);
    if(value==GREAT || value==EQUAL)
        return true;
    return false;
}

/// @return returns true if the uint512 is not zero
bool uint512::boolean() const {
    for(size_t i=0; i<UINT512LIMBS; ++i) {
        if(limbs[i])
            return true;
    }
    return false;
}

bool uint512::operator&&(const uint512& with) const {
    return boolean() && with.boolean();
}

bool uint512::operator||(const uint512& with) const {
    return boolean() || with.boolean();
}

bool uint512::operator!() const {
    return !boolean();
}

uint512 uint512::operator&(const uint512& with) const {
    uint512 result(0,0,0,0,0,0,0,0);
    for(size_t i=0; i<UINT512LIMBS; ++i)
        result.limbs[i] = limbs[i] & with.limbs[i];
    return result;
}

uint512 uint512::operator|(const uint512& with) const {
    uint512 result(0,0,0,0,0,0,0,0);
    for(size_t i=0; i<UINT512LIMBS; ++i)
        result.limbs[i] = limbs[i] | with.limbs[i];
    return result;
}

uint512 uint512::operator^(const uint512& with) const {
    uint512 result(0,0,0,0,0,0,0,0);
    for(size_t i=0; i<UINT512LIMBS; ++i)
        result.limbs[i] = limbs[i] ^ with.limbs[i];
    return result;
}

uint512 uint512::operator~() const {
    uint512 result(0,0,0,0,0,0,0,0);
    for(size_t i=0; i<UINT512LIMBS; ++i)
        result.limbs[i] = ~limbs[i];
    return result;
}

uint512& uint512::operator&=(const uint512& with) {
    for(size_t i=0; i<UINT512LIMBS; ++i)
        limbs[i] &= with.limbs[i];
    return *this;
}

uint512& uint512::operator|=(const uint512& with) {
    for(size_t i=0; i<UINT512LIMBS; ++i)
        limbs[i] |= with.limbs[i];
    return *this;
}

uint512& uint512::operator^=(const uint512& with) {
    for(size_t i=0; i<UINT512LIMBS; ++i)
        limbs[i] ^= with.limbs[i];
    return *this;
}

uint512 uint512::operator+(const uint512& add) const {
    
    uint512 sum = *this;

#if(__x86_64 || __x86_64__ || __amd64 || __amd64__)
#if(_MSC_VER || _PURE_CPP)
    unsigned long carry = 0, prev;

    for(size_t i=0; i<UINT512LIMBS; ++i) {
        prev = sum.limbs[i];
        sum.limbs[i] += add.limbs[i] + carry;

        if(sum.limbs[i]<prev || (carry && (limbs[i] == add.limbs[i]) && (add.limbs[i] == __UINT64_MAX__))) {
            carry = 1;
        }
        else {
            carry = 0;
        }
    }
#elif(__clang__ || __GNUC__ || __GNUG__ || __MINGW64__)
#ifndef _HIDE_WARNING
#warning using GCC inline asm, please enable optimization flag, recomended : -O2, to enable use C++ implementation instead, enable the -D_PURE_CPP flag.
#endif
    asm (
        "add %[adn0], %[sum0]\n\t"
        "adc %[adn1], %[sum1]\n\t"
        "adc %[adn2], %[sum2]\n\t"
        "adc %[adn3], %[sum3]\n\t"
        "adc %[adn4], %[sum4]\n\t"
        "adc %[adn5], %[sum5]\n\t"
        "adc %[adn6], %[sum6]\n\t"
        "adc %[adn7], %[sum7]"
        :
        [sum0]"+r"(sum.limbs[0]),
        [sum1]"+r"(sum.limbs[1]),
        [sum2]"+r"(sum.limbs[2]),
        [sum3]"+r"(sum.limbs[3]),
        [sum4]"+r"(sum.limbs[4]),
        [sum5]"+r"(sum.limbs[5]),
        [sum6]"+r"(sum.limbs[6]),
        [sum7]"+&r"(sum.limbs[7])
        :
        [adn0]"m"(add.limbs[0]),
        [adn1]"m"(add.limbs[1]),
        [adn2]"m"(add.limbs[2]),
        [adn3]"m"(add.limbs[3]),
        [adn4]"m"(add.limbs[4]),
        [adn5]"m"(add.limbs[5]),
        [adn6]"m"(add.limbs[6]),
        [adn7]"m"(add.limbs[7])
        : "cc"
    );
#else
    #error uint512 is not supported on this compiler
#endif
#elif
#error uint512 is not supported on 32-bit computers (x86 architectures)
#endif

    return sum;
}

uint512& uint512::operator+=(const uint512& add) {
    return *this = *this + add;
}

uint512 uint512::operator-(const uint512& sub) const {
    
    uint512 dif = *this;

    unsigned long carry = 0, prev;

    if(dif.limbs[0]<sub.limbs[0])
        carry = 1;

    dif.limbs[0] -= sub.limbs[0];

    for(size_t i=1; i<UINT512LIMBS; ++i) {
        prev = dif.limbs[i];
        dif.limbs[i] -= carry;
        dif.limbs[i] -= sub.limbs[i];

        if(prev<sub.limbs[i]){
            carry = 1;
        }
        else if(sub.limbs[i]==UINT64_MAX && carry) {
            carry = 1;
        }
        else {
            carry = 0;
        }
    }

    return dif;
}

uint512& uint512::operator-=(const uint512& sub) {
    return *this = *this - sub;
}

/// This is the ugly part
uint512 uint512::operator*(const uint512& mr) const {
    
    uint512 pd(0);

#if(__x86_64 || __x86_64__ || __amd64 || __amd64__)
#if(_MSC_VER || _PURE_CPP)
    unsigned long carry = 0, prev;

    for(size_t i=0; i<UINT512LIMBS; ++i) {
        prev = pd.limbs[i];
        pd.limbs[i] += mr.limbs[i] + carry;

        if(pd.limbs[i]<prev || (carry && (limbs[i] == mr.limbs[i]) && (mr.limbs[i] == __UINT64_MAX__))) {
            carry = 1;
        }
        else {
            carry = 0;
        }
    }
#elif(__clang__ || __GNUC__ || __GNUG__ || __MINGW64__)
#ifndef _HIDE_WARNING
#warning using GCC inline asm, please enable optimization flag, recomended : -O2, to enable use C++ implementation instead, enable the -D_PURE_CPP flag.
#endif
    asm volatile (
        // ------------------- 1
        "mov %[mc0], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "mov %%rax, %[pd0]\n\t"
        "mov %%rdx, %[pd1]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd1]\n\t"
        "adc %%rdx, %[pd2]\n\t"

        "mov %[mc2], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd2]\n\t"
        "adc %%rdx, %[pd3]\n\t"

        "mov %[mc3], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd3]\n\t"
        "adc %%rdx, %[pd4]\n\t"

        "mov %[mc4], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd4]\n\t"
        "adc %%rdx, %[pd5]\n\t"

        "mov %[mc5], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd5]\n\t"
        "adc %%rdx, %[pd6]\n\t"

        "mov %[mc6], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd6]\n\t"
        "adc %%rdx, %[pd7]\n\t"

        "mov %[mc7], %%rax\n\t"
        "mov %[mr0], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        // ------------------- 2

        "mov %[mc0], %%rax\n\t"
        "mov %[mr1], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd1]\n\t"
        "adc %%rdx, %[pd2]\n\t"
        "adc $0, %[pd3]\n\t"
        "adc $0, %[pd4]\n\t"
        "adc $0, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mov %[mr1], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd2]\n\t"
        "adc %%rdx, %[pd3]\n\t"
        "adc $0, %[pd4]\n\t"
        "adc $0, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc2], %%rax\n\t"
        "mov %[mr1], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd3]\n\t"
        "adc %%rdx, %[pd4]\n\t"
        "adc $0, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc3], %%rax\n\t"
        "mov %[mr1], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd4]\n\t"
        "adc %%rdx, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc4], %%rax\n\t"
        "mov %[mr1], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd5]\n\t"
        "adc %%rdx, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc5], %%rax\n\t"
        "mov %[mr1], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd6]\n\t"
        "adc %%rdx, %[pd7]\n\t"

        "mov %[mc6], %%rax\n\t"
        "mov %[mr1], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        // -------------------- 3

        "mov %[mc0], %%rax\n\t"
        "mov %[mr2], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd2]\n\t"
        "adc %%rdx, %[pd3]\n\t"
        "adc $0, %[pd4]\n\t"
        "adc $0, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mov %[mr2], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd3]\n\t"
        "adc %%rdx, %[pd4]\n\t"
        "adc $0, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc2], %%rax\n\t"
        "mov %[mr2], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd4]\n\t"
        "adc %%rdx, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc3], %%rax\n\t"
        "mov %[mr2], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd5]\n\t"
        "adc %%rdx, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc4], %%rax\n\t"
        "mov %[mr2], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd6]\n\t"
        "adc %%rdx, %[pd7]\n\t"

        "mov %[mc5], %%rax\n\t"
        "mov %[mr2], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        // -------------------- 4

        "mov %[mc0], %%rax\n\t"
        "mov %[mr3], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd3]\n\t"
        "adc %%rdx, %[pd4]\n\t"
        "adc $0, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mov %[mr3], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd4]\n\t"
        "adc %%rdx, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc2], %%rax\n\t"
        "mov %[mr3], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd5]\n\t"
        "adc %%rdx, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc3], %%rax\n\t"
        "mov %[mr3], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd6]\n\t"
        "adc %%rdx, %[pd7]\n\t"

        "mov %[mc4], %%rax\n\t"
        "mov %[mr3], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        // ------------------- 5

        "mov %[mc0], %%rax\n\t"
        "mov %[mr4], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd4]\n\t"
        "adc %%rdx, %[pd5]\n\t"
        "adc $0, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mov %[mr4], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd5]\n\t"
        "adc %%rdx, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc2], %%rax\n\t"
        "mov %[mr4], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd6]\n\t"
        "adc %%rdx, %[pd7]\n\t"

        "mov %[mc3], %%rax\n\t"
        "mov %[mr4], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        // ------------------- 6

        "mov %[mc0], %%rax\n\t"
        "mov %[mr5], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd5]\n\t"
        "adc %%rdx, %[pd6]\n\t"
        "adc $0, %[pd7]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mov %[mr5], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd6]\n\t"
        "adc %%rdx, %[pd7]\n\t"

        "mov %[mc2], %%rax\n\t"
        "mov %[mr5], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        :
        [pd0]"+r"(pd.limbs[0]),
        [pd1]"+r"(pd.limbs[1]),
        [pd2]"+r"(pd.limbs[2]),
        [pd3]"+r"(pd.limbs[3]),
        [pd4]"+r"(pd.limbs[4]),
        [pd5]"+r"(pd.limbs[5]),
        [pd6]"+r"(pd.limbs[6]),
        [pd7]"+r"(pd.limbs[7])
        :
        [mr0]"m"(mr.limbs[0]),
        [mr1]"m"(mr.limbs[1]),
        [mr2]"m"(mr.limbs[2]),
        [mr3]"m"(mr.limbs[3]),
        [mr4]"m"(mr.limbs[4]),
        [mr5]"m"(mr.limbs[5]),

        [mc0]"m"(limbs[0]),
        [mc1]"m"(limbs[1]),
        [mc2]"m"(limbs[2]),
        [mc3]"m"(limbs[3]),
        [mc4]"m"(limbs[4]),
        [mc5]"m"(limbs[5]),
        [mc6]"m"(limbs[6]),
        [mc7]"m"(limbs[7])
        : "rax","rdx", "rbx","cc"
    );

    asm volatile (
        // -------------------- 7

        "mov %[mc0], %%rax\n\t"
        "mov %[mr6], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd6]\n\t"
        "adc %%rdx, %[pd7]\n\t"

        "mov %[mc1], %%rax\n\t"
        "mov %[mr6], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        // -------------------- 8

        "mov %[mc0], %%rax\n\t"
        "mov %[mr7], %%rbx\n\t"
        "mul %%rbx\n\t"
        "add %%rax, %[pd7]\n\t"

        :
        [pd6]"+r"(pd.limbs[6]),
        [pd7]"+r"(pd.limbs[7])
        :
        [mr6]"m"(mr.limbs[6]),
        [mr7]"m"(mr.limbs[7]),

        [mc0]"m"(limbs[0]),
        [mc1]"m"(limbs[1])
        : "rax","rdx", "rbx","cc"
    );
#else
    #error uint512 is not supported on this compiler
#endif
#elif
#error uint512 is not supported on 32-bit computers (x86 architectures)
#endif

    return pd;
}

uint512& uint512::operator*=(const uint512& mul) {
    *this = *this * mul;
    return *this;
}

/** long division using bits, shifts and subtract */
uint512 uint512::ss_div(const uint512& divisor) const {
    
    uint512
        quotient(0),
        pdvn(0),
        bit(0);

    for(size_t i=0; i<UINT512BITS; ++i) {

        pdvn = pdvn << 1;
        quotient = quotient << 1;

        bit = *this << i;
        bit = bit >> UINT511BITS;

        pdvn.limbs[UINT512_LS_LIMB] |= bit.limbs[UINT512_LS_LIMB];

        if(pdvn>=divisor) {
            pdvn = pdvn - divisor;            
            quotient.limbs[UINT512_LS_LIMB] |= 1;
        }
    }

    return quotient;
}

/** long division using bits, shifts and subtract */
uint512 uint512::ss_mod(const uint512& divisor) const {
    
    uint512
        quotient(0),
        pdvn(0),
        bit(0);

    for(size_t i=0; i<UINT512BITS; ++i) {

        pdvn = pdvn << 1;
        quotient = quotient << 1;

        bit = *this << i;
        bit = bit >> UINT511BITS;

        pdvn.limbs[UINT512_LS_LIMB] |= bit.limbs[UINT512_LS_LIMB];

        if(pdvn>=divisor) {
            pdvn -= divisor;
            quotient.limbs[UINT512_LS_LIMB] |= 1;
        }
    }

    return pdvn;
}

uint512 uint512::operator/(const uint512& divisor) const {

    int value = divisor.one_or_zero();
    if(value == 0) {
        std::cout << "\nError!!!\nuint512 operands:\n";
        std::cout << "Dividen = "; printHex();
        std::cout << "Divisor = "; divisor.printHex();
        throw std::domain_error("division by zero is not possible");
    }
    else if(*this == divisor) {
        return uint512(1); // remainder zero
    }
    else if(*this < divisor) {
        return uint512(0); // remainder *this (dividen)
    }
    else if(value == 1) {
        return *this;
    }
    return this->ss_div(divisor);
}

uint512& uint512::operator/=(const uint512& divisor) {
    *this = *this / divisor;
    return *this;
}

uint512 uint512::operator%(const uint512& divisor) const {

    int value = divisor.one_or_zero();
    if(value == 0) {
        std::cout << "\nError!!!\nuint512 operands:\n";
        std::cout << "Dividen = "; printHex();
        std::cout << "Divisor = "; divisor.printHex();
        throw std::domain_error("% mod:division by zero is not possible");
    }
    else if(*this == divisor) {
        return uint512(0);
    }
    else if(*this < divisor) {
        return *this;
    }
    else if(value == 1) {
        return uint512(0);
    }
    return this->ss_mod(divisor);
}

uint512& uint512::operator%=(const uint512& divisor) {
    *this = *this % divisor;
    return *this;
}

// LEFT SHIFT

uint512 uint512::operator<<(size_t lshift) const {

    uint512 result(0,0,0,0,0,0,0,0);

    if(lshift>UINT512BITS)
        return result;

    size_t limb_shifts = lshift / UINT64BITS;
    size_t bit_shifts = lshift % UINT64BITS;

    // shift limbs
    if(limb_shifts) {
        for(int i=limb_shifts; i<UINT512LIMBS; ++i) {
            result.limbs[i] = limbs[(i-limb_shifts)%UINT512LIMBS];
        }
    }
    else {
        result = *this;
    }

    if(bit_shifts) {
        // compute carries.
        unsigned long carries[UINT512LIMBS-1];
        for(size_t i=0; i<UINT512LIMBS-1; ++i) {
            carries[i] = result.limbs[i] >> (UINT64BITS-bit_shifts);
        }

        // apply shift to first index.
        result.limbs[0] <<= bit_shifts;

        // apply shifts and the carry over to the procceding indecies.
        for(size_t i=1; i<UINT512LIMBS; ++i) {
            result.limbs[i] <<= bit_shifts;
            result.limbs[i] |= carries[i-1];
        }
    }

    return result;
}

uint512& uint512::operator<<=(size_t lshift) {
    return (*this = *this << lshift);
}

// RIGHT SHIFT 

uint512 uint512::operator>>(size_t rshift) const {

    uint512 result(0,0,0,0,0,0,0,0);

    if(rshift>UINT512BITS)
        return result;

    size_t limb_shifts = rshift / UINT64BITS;
    size_t bit_shifts = rshift % UINT64BITS;

    // shift limbs
    if(limb_shifts) {
        for(size_t i=0; i<(UINT512LIMBS-limb_shifts); ++i) {
            result.limbs[i] = limbs[(i+limb_shifts)%UINT512LIMBS];
        }
    }
    else {
        result = *this;
    }

    if(bit_shifts) {
        // compute carries.
        unsigned long carries[UINT512LIMBS-1];
        for(size_t i=0; i<UINT512LIMBS-1; ++i) {
            carries[i] = result.limbs[i+1] << (UINT64BITS-bit_shifts);
        }

        // apply shift to last index.
        result.limbs[7] >>= bit_shifts;

        // apply shifts and the carry over to the procceding indecies.
        for(size_t i=0; i<UINT512LIMBS-1; ++i) {
            result.limbs[i] >>= bit_shifts;
            result.limbs[i] |= carries[i];
        }
    }

    return result;
}

uint512& uint512::operator>>=(size_t rshift) {
    return (*this = *this >> rshift);
}

/// the limb[7] will be printed first then 6,5, ..., the limb[0] will be printed last.
void uint512::printHex() const {
    std::cout << "0x";
    for(size_t i=0; i<UINT512LIMBS; ++i)
        printf("%016lx",limbs[UINT512LIMBS-1-i]);
    std::cout << "\n";
}

/// the limb[7] will be printed first then 6,5, ..., the limb[0] will be printed last.
void uint512::printBits() const {
    for(size_t i=0; i<UINT512LIMBS; ++i) {
        std::cout << std::bitset<64>(limbs[UINT512LIMBS-1-i]) ;
    }
    std::cout << "\n";
}

#endif