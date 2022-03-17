#ifndef uidMRDCVLSC_ChaCha20_HPP
#define uidMRDCVLSC_ChaCha20_HPP

#include <iostream>
#include <cstring>
#include <cmath>
#include "uint256.hpp"

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

#define HALF_KEY_BYTES 16

namespace __internal_chacha20
{
    inline unsigned int bit_left_roll(unsigned int num, size_t n) {

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

    /**Initializes a ChaCha state.
    * 
    * note The output is an unsigned int array with 16 elements.
    * 
    * @param output this is the resulting initial ChaCha state.
    * @param key a 32-bytes/256-bits key.
    * @param counter a 32 bit unsigned integer block counter, this is essential when
    * initializing ChaCha states for different ChaCha blocks, It creates uniqueness for different blocks.
    * @param nonce a 12-bytes/96-bits number only once, in some crypto scheme, this is called an IV.*/
    void initialize_chacha_state( // function parameters
        unsigned int *output,
        unsigned int *key,
        unsigned int counter,
        unsigned int *nonce) { // function body
        
        // indecies 0-3 : constants
        output[0] = 0x61707865;
        output[1] = 0x3320646e;
        output[2] = 0x79622d32;
        output[3] = 0x6b206574;

        // indecies 4-11 : 256-bit key by reading the bytes in little-endian order, in 4-byte chunks.
        output[4] = key[0];
        output[5] = key[1];
        output[6] = key[2];
        output[7] = key[3];
        output[8] = key[4];
        output[9] = key[5];
        output[10] = key[6];
        output[11] = key[7];

        // index 12 : a block counter.
        output[12] = counter;

        // indecies 13-15
        output[13] = nonce[0];
        output[14] = nonce[1];
        output[15] = nonce[2];
    }

    /**Transforms an initial ChaCha state.
     * 
     * @note the input and output are both 16 unsigned int arrays.
     * 
     * @param output this is where the result of the transformed ChaCha state will go.
     * @param input this is the initial ChaCha state.
     * @param nonce a 12-bytes/96-bits number only once, in some crypto scheme, this is called an IV.
    */
    void chacha20_block( // function parameters
        unsigned int *output,
        unsigned int *input) { // function body
        
        // copy initial state to state
        for(size_t i=0; i<__CHAx220_STATE_DWORDS__; ++i)
            output[i] = input[i];

        // chacha 20 rounds
        for(size_t i=0; i<10; ++i) {

            // column rounds
            QUARTERROUND(output, 0, 4, 8, 12);
            QUARTERROUND(output, 1, 5, 9, 13);
            QUARTERROUND(output, 2, 6, 10, 14);
            QUARTERROUND(output, 3, 7, 11, 15);

            // diagonal rounds
            QUARTERROUND(output, 0, 5, 10, 15);
            QUARTERROUND(output, 1, 6, 11, 12);
            QUARTERROUND(output, 2, 7, 8, 13);
            QUARTERROUND(output, 3, 4, 9, 14);
        }

        // add initialized state to the output state
        for(size_t i=0; i<__CHAx220_STATE_DWORDS__; ++i)
            output[i] += input[i];
    }
}

namespace __internal_poly1305 {

    uint256
        PLY_CNSTNT_1(uint128(0,0),uint128(0,1)),
        PLY_CNSTNT_5(uint128(0,0),uint128(0,5)),
        PLY_CNSTNT_2POWER128(uint128(0,0x1), uint128(0x0000000000000000, 0x0000000000000000));

    void clamp(unsigned char r[HALF_KEY_BYTES]) {
        r[3] &= 15;
        r[7] &= 15;
        r[11] &= 15;
        r[15] &= 15;
        r[4] &= 252;
        r[8] &= 252;
        r[12] &= 252;
    }

    /**Generates the 16 byte(128-bits) tag using the 32 byte(256-bits) one time key
     * and an arbitrary length message.
     * 
     * @param key an `unsigned char` array with 32 elements (bytes).
     * @param msg an `unsigned char` array, this is the message.
     * @param msg_len the length of the `msg` array.
     * @param output an `unsigned char` array with an element size of 16, the output will be written here.
    */
    void poly1305_mac(unsigned char *key, unsigned char* msg, size_t msg_len, unsigned char *output) {
        
        unsigned char *unclamped_r = new unsigned char[HALF_KEY_BYTES];
        memcpy(unclamped_r,key,HALF_KEY_BYTES);
        clamp(unclamped_r);
        
        uint256 r(unclamped_r);
        uint256 s(key+HALF_KEY_BYTES);
        uint256 a(uint128(0,0),uint128(0,0));
        uint256 p = (PLY_CNSTNT_1 << 130)-PLY_CNSTNT_5;

        size_t blocks = msg_len/HALF_KEY_BYTES;
        size_t remain = msg_len%HALF_KEY_BYTES;

        r.lsdq().swapHighLow();
        s.lsdq().swapHighLow();

        // 16 byte blocks
        for(size_t i=0; i<blocks; ++i) {
            uint256 n(msg+(i*HALF_KEY_BYTES));
            n.lsdq().swapHighLow();

            n += PLY_CNSTNT_2POWER128;
            a += n;
            a = (r * a) % p;
        }

        // remaining bytes
        if(remain) {
            unsigned char *last_block = new unsigned char[HALF_KEY_BYTES];
            memcpy(last_block,msg+(blocks*HALF_KEY_BYTES),remain);
            memset(last_block+remain+1,0x00,(HALF_KEY_BYTES-remain)-1);
            last_block[remain] = 0x01;

            uint256 n(last_block);
            n.lsdq().swapHighLow();

            a += n;
            a = (r * a) % p;

            delete [] last_block;
        }

        a+=s;
        a.lsdq().swapHighLow();

        memcpy(output,(unsigned char*)a.lsdq().data,UINT128BYTES);
        delete [] unclamped_r;
    }

    // void poly1305_key_gen(unsigned char *output, unsigned char *key, unsigned int *nonce) {
    //     unsigned int counter = 0;
    //     // __internal_chacha20::chacha20_block(output,key,)
    // }
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
                initial_state
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
                initial_state
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