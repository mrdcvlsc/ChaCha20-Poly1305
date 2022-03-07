#ifndef uidMRDCVLSC_ChaCha20_HPP
#define uidMRDCVLSC_ChaCha20_HPP

#include <iostream>
#include <cstring>

/// Number of bytes(unsigned char) inside a ChaCha20 State
static const int __CHAx220_STATE_BYTES__ = 16;
/// Number of bytes(unsigned char) inside a ChaCha20 key
static const int __CHAx220_KEY_BYTES__ = 32;
/// Number of dwords(unsigned int) inside a ChaCha20 key
static const int __CHAx220_KEY_DWORDS__ = 8;
/// Number of bytes(unsigned char) inside a ChaCha20 nonce
static const int __CHAx220_NONCE_BYTES__ = 12;
/// Number of dwords(unsigned int) inside a ChaCha20 nonce
static const int __CHAx220_NONCE_DWORDS__ = 3;

namespace ChaCha20
{

}

namespace __internal_chacha20
{
    unsigned int bit_left_roll(unsigned int num, size_t n) {
        
        if((n<1)^(n>32)) {
            throw std::out_of_range("bit_left_roll() : cannot bit_left_roll with an 'n' value less than 1 or greater than 32");
        }

        unsigned int msb = (num << n);
        unsigned int lsb = (num >> 32-n);

        return msb | lsb;
    }

    void QUARTERROUND(unsigned int *state, size_t x, size_t y, size_t z, size_t w) {
        
        state[x] += state[y]; state[w] ^= state[x]; state[w] = bit_left_roll(state[w],16);
        state[z] += state[w]; state[y] ^= state[z]; state[y] = bit_left_roll(state[y],12);
        state[x] += state[y]; state[w] ^= state[x]; state[w] = bit_left_roll(state[w],8);
        state[z] += state[w]; state[y] ^= state[z]; state[y] = bit_left_roll(state[y],7);
    }

    unsigned int *initialize_chacha_state(unsigned int *key, unsigned int counter, unsigned int *nounce) {
        
        unsigned int *state = new unsigned int[__CHAx220_STATE_BYTES__];

        // indecies 0-3 : constants
        state[0] = 0x61707865;
        state[1] = 0x3320646e;
        state[2] = 0x79622d32;
        state[3] = 0x6b206574;

        // indecies 4-11 : 256-bit key by reading the bytes in little-endian order, in 4-byte chunks.
        state[4] = key[0];
        state[5] = key[1];
        state[6] = key[2];
        state[7] = key[3];
        state[8] = key[4];
        state[9] = key[5];
        state[10] = key[6];
        state[11] = key[7];

        // index 12 : a block counter.
        state[12] = counter;

        // indecies 13-15
        state[13] = nounce[0];
        state[14] = nounce[1];
        state[15] = nounce[2];

        return state;
    }

    unsigned int *chacha20_block(unsigned int *initial_state, unsigned int *key, unsigned int counter, unsigned int *nounce) {
        
        // unsigned int *initial_state = initialize_chacha_state(key, counter, nounce);
        unsigned int *state = new unsigned int [__CHAx220_STATE_BYTES__];

        // copy initial state to state
        for(size_t i=0; i<__CHAx220_STATE_BYTES__; ++i)
            state[i] = initial_state[i];

        // chacha 20 rounds
        for(size_t i=0; i<10; ++i) {
            // column rounds
            QUARTERROUND(state, 0, 4, 8, 12);
            QUARTERROUND(state, 1, 5, 9, 13);
            QUARTERROUND(state, 2, 6, 10, 14);
            QUARTERROUND(state, 3, 7, 11, 15);

            // diagonal rounds
            QUARTERROUND(state, 0, 5, 10, 15);
            QUARTERROUND(state, 1, 6, 11, 12);
            QUARTERROUND(state, 2, 7, 8, 13);
            QUARTERROUND(state, 3, 4, 9, 14);
        }

        // add initialized state to the output state
        for(size_t i=0; i<__CHAx220_STATE_BYTES__; ++i)
            state[i] += initial_state[i];

        return state;

    }

    // 8 unsigned int, 32 bytes, 256-bits
    // Key = 00:01:02:03: 04:05:06:07: 08:09:0a:0b: 0c:0d:0e:0f: 10:11:12:13: 14:15:16:17: 18:19:1a:1b: 1c:1d:1e:1f.
}

#endif