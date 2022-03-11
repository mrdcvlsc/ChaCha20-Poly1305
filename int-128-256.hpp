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

    int128 operator*(const int128& mul) const {
    
        int128 pro(0,0);

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    #if defined(__x86_64__) || defined(__ia64__) || defined(__amd__64__)
        asm volatile(
            "mov %[rhlsb], %%rax\n\t"
            "mul %[lhlsb]\n\t"
            "mov %%rax, %[plsb]\n\t"
            "mov %%rdx, %[pmsb]\n\t"
            "mov %[rhlsb], %%rax\n\t"
            "mul %[lhmsb]\n\t"
            "add %%rax, %[pmsb]\n\t"
            "mov %[rhmsb], %%rax\n\t"
            "mul %[lhlsb]\n\t"
            "add %%rax, %[pmsb]"
            :[pmsb]"=r"(pro.msb()),[plsb]"=r"(pro.lsb())
            :[rhmsb]"r"(mul.msb()), [rhlsb]"r"(mul.lsb()), [lhmsb]"r"(data[0]), [lhlsb]"r"(data[1])
            : "rax", "rdx", "memory"
        );
    #else
        #error "int128 multiplication has no implementation yet for x86 architectures (32-bit executables)."
    #endif
#elif defined(_MSC_VER)
    #error "int128 multiplication has no implementation yet for Microsoft Visual C++ Compiler."
#endif

        return pro;
    }

    void printHex() const {
        printf("0x%08lx%08lx\n",msb(),lsb());
    }

    void printHex_separated() const {
        printf("0x%08lx|0x%08lx\n",msb(),lsb());
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