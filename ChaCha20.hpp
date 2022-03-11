#ifndef uidMRDCVLSC_ChaCha20_HPP
#define uidMRDCVLSC_ChaCha20_HPP

#include <iostream>
#include <cstring>

/// Number of bytes(unsigned char) inside a ChaCha20 State
static const int __CHAx220_STATE_BYTES__ = 64;
/// Number of dwords(unsigned int) inside a ChaCha20 State
static const int __CHAx220_STATE_DWORDS__ = 16;

/// Number of bytes(unsigned char) inside a ChaCha20 key
static const int __CHAx220_KEY_BYTES__ = 32;
/// Number of dwords(unsigned int) inside a ChaCha20 key
static const int __CHAx220_KEY_DWORDS__ = 8;

/// Number of bytes(unsigned char) inside a ChaCha20 nonce
static const int __CHAx220_NONCE_BYTES__ = 12;
/// Number of dwords(unsigned int) inside a ChaCha20 nonce
static const int __CHAx220_NONCE_DWORDS__ = 3;

/// Number of bytes(unsigned char) inside a ChaCha20 block function output
static const int __CHAx220_BLK_FUNC_OUTPUT_BYTES__ = 64;
/// Number of dwords(unsigned int) inside a ChaCha20 block function output
static const int __CHAx220_BLK_FUNC_OUTPUT_DWORDS__ = 16;

namespace __internal_chacha20
{
    inline unsigned int bit_left_roll(unsigned int num, size_t n) {
        
        // for debugging
        // if((n<1)^(n>32)) {
        //     throw std::out_of_range("bit_left_roll() : cannot bit_left_roll with an 'n' value less than 1 or greater than 32");
        // }

        unsigned int msb = (num << n);
        unsigned int lsb = (num >> (32-n));

        return msb | lsb;
    }

    inline void QUARTERROUND(unsigned int *state, size_t x, size_t y, size_t z, size_t w) {
        
        state[x] += state[y]; state[w] ^= state[x]; state[w] = bit_left_roll(state[w],16);
        state[z] += state[w]; state[y] ^= state[z]; state[y] = bit_left_roll(state[y],12);
        state[x] += state[y]; state[w] ^= state[x]; state[w] = bit_left_roll(state[w],8);
        state[z] += state[w]; state[y] ^= state[z]; state[y] = bit_left_roll(state[y],7);
    }

    void initialize_chacha_state( // function parameters
        unsigned int *initial_state,
        unsigned int *key,
        unsigned int counter,
        unsigned int *nounce) { // function body
        
        // indecies 0-3 : constants
        initial_state[0] = 0x61707865;
        initial_state[1] = 0x3320646e;
        initial_state[2] = 0x79622d32;
        initial_state[3] = 0x6b206574;

        // indecies 4-11 : 256-bit key by reading the bytes in little-endian order, in 4-byte chunks.
        initial_state[4] = key[0];
        initial_state[5] = key[1];
        initial_state[6] = key[2];
        initial_state[7] = key[3];
        initial_state[8] = key[4];
        initial_state[9] = key[5];
        initial_state[10] = key[6];
        initial_state[11] = key[7];

        // index 12 : a block counter.
        initial_state[12] = counter;

        // indecies 13-15
        initial_state[13] = nounce[0];
        initial_state[14] = nounce[1];
        initial_state[15] = nounce[2];
    }

    void chacha20_block( // function parameters
        unsigned int *key_stream,
        unsigned int *initial_state,
        unsigned int *key,
        unsigned int counter,
        unsigned int *nounce) { // function body
        
        // copy initial state to state
        for(size_t i=0; i<__CHAx220_STATE_DWORDS__; ++i)
            key_stream[i] = initial_state[i];

        // chacha 20 rounds
        for(size_t i=0; i<10; ++i) {
            // column rounds
            QUARTERROUND(key_stream, 0, 4, 8, 12);
            QUARTERROUND(key_stream, 1, 5, 9, 13);
            QUARTERROUND(key_stream, 2, 6, 10, 14);
            QUARTERROUND(key_stream, 3, 7, 11, 15);

            // diagonal rounds
            QUARTERROUND(key_stream, 0, 5, 10, 15);
            QUARTERROUND(key_stream, 1, 6, 11, 12);
            QUARTERROUND(key_stream, 2, 7, 8, 13);
            QUARTERROUND(key_stream, 3, 4, 9, 14);
        }

        // add initialized state to the output state
        for(size_t i=0; i<__CHAx220_STATE_DWORDS__; ++i)
            key_stream[i] += initial_state[i];
    }
}

namespace ChaCha20
{
    unsigned char *encrypt( // function parameters
        unsigned char *key,
        unsigned int counter,
        unsigned char *nonce,
        unsigned char *plaintext,
        size_t len) { // function body

        unsigned int *plaintext_blocked = (unsigned int*) plaintext;

        unsigned char *cipher_text = new unsigned char[len];
        unsigned int *cipher_blocked = (unsigned int*) cipher_text;

        size_t blocks = len/__CHAx220_BLK_FUNC_OUTPUT_BYTES__;
        size_t lastblock_bytes = (len%__CHAx220_BLK_FUNC_OUTPUT_BYTES__);

        unsigned int *initial_state = new unsigned int[__CHAx220_STATE_DWORDS__];
        unsigned int *key_stream = new unsigned int[__CHAx220_STATE_DWORDS__];
        unsigned char *key_bytestream = (unsigned char*) key_stream;

        for(size_t i=0; i<blocks; ++i) {

            // initialize ChaCha20 state for the current block
            __internal_chacha20::initialize_chacha_state(
                initial_state,
                (unsigned int*)key,
                counter+i,
                (unsigned int*)nonce
            );

            // perform ChaCha20 Block Function and get the key_stream output
            __internal_chacha20::chacha20_block(
                key_stream,
                initial_state,
                (unsigned int*)key,
                counter+i,
                (unsigned int*)nonce
            );

            if(blocks) {
                for( // loop condition
                    size_t j=(__CHAx220_BLK_FUNC_OUTPUT_DWORDS__*i), k=0;
                    j<((__CHAx220_BLK_FUNC_OUTPUT_DWORDS__*i)+__CHAx220_BLK_FUNC_OUTPUT_DWORDS__);
                    ++j) {

                    cipher_blocked[j] = plaintext_blocked[j] ^ key_stream[k++];
                }
            }
        }

        // XOR remaining key_stream bytes and plaintext bytes
        if(lastblock_bytes) {

            unsigned char *padded_last_bytes = new unsigned char[__CHAx220_BLK_FUNC_OUTPUT_BYTES__];
            unsigned int *padded_last_block = (unsigned int*) padded_last_bytes;
            memcpy(padded_last_bytes,plaintext+(__CHAx220_BLK_FUNC_OUTPUT_BYTES__*blocks),lastblock_bytes);

            // initialize ChaCha20 state for the current block
            __internal_chacha20::initialize_chacha_state(
                initial_state,
                (unsigned int*)key,
                counter+blocks,
                (unsigned int*)nonce
            );

            // perform ChaCha20 Block Function and get the key_stream output
            __internal_chacha20::chacha20_block(
                key_stream,
                initial_state,
                (unsigned int*)key,
                counter+blocks,
                (unsigned int*)nonce
            );
            
            for( // loop condition
                size_t j=(__CHAx220_BLK_FUNC_OUTPUT_DWORDS__*blocks), k=0;
                j<((__CHAx220_BLK_FUNC_OUTPUT_DWORDS__*blocks)+__CHAx220_BLK_FUNC_OUTPUT_DWORDS__);
                ++j, ++k) {

                padded_last_block[k] ^= key_stream[k];
            }

            memcpy(cipher_text+(__CHAx220_BLK_FUNC_OUTPUT_BYTES__*blocks),padded_last_bytes,lastblock_bytes);

            delete [] padded_last_bytes;
        }

        delete [] initial_state;
        delete [] key_stream;

        return cipher_text;
    }
}

#endif