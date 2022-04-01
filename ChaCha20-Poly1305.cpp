#ifndef _CHACHA20_CPP_mrdcvlsc_
#define _CHACHA20_CPP_mrdcvlsc_
#include <iostream>

#ifdef MAKE_LIB
#include "ChaCha20-Poly1305.hpp"
#endif

// #define DEVMODE
#ifdef DEVMODE
#include "ChaCha20-Poly1305.hpp"
#endif

namespace __internal_chacha20
{
    inline unsigned int bit_left_roll(unsigned int num, size_t n) {

        unsigned int msb = (num << n);
        unsigned int lsb = (num >> (32-n));

        return msb | lsb;
    }

    void QUARTERROUND(unsigned int *state, size_t x, size_t y, size_t z, size_t w) {
        
        state[x] += state[y]; state[w] ^= state[x]; state[w] = bit_left_roll(state[w],16);
        state[z] += state[w]; state[y] ^= state[z]; state[y] = bit_left_roll(state[y],12);
        state[x] += state[y]; state[w] ^= state[x]; state[w] = bit_left_roll(state[w],8);
        state[z] += state[w]; state[y] ^= state[z]; state[y] = bit_left_roll(state[y],7);
    }

    void init_state( // function parameters
        unsigned int *output,
        const unsigned int *key,
        unsigned int counter,
        const unsigned int *nonce) { // function body
        
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

    void apply_20rounds(unsigned int *output, const unsigned int *input) {
        
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

    void clamp(unsigned char r[HALF_KEY_BYTES]) {
        r[3] &= 15;
        r[7] &= 15;
        r[11] &= 15;
        r[15] &= 15;
        r[4] &= 252;
        r[8] &= 252;
        r[12] &= 252;
    }

    void mac(unsigned char *output, const unsigned char *key, const unsigned char* msg, size_t msg_len) {
        
        unsigned char *unclamped_r = new unsigned char[HALF_KEY_BYTES];
        memcpy(unclamped_r,key,HALF_KEY_BYTES);
        clamp(unclamped_r);
        
        uint512 r(unclamped_r,HALF_KEY_BYTES),
                s(key+HALF_KEY_BYTES,HALF_KEY_BYTES),
                a(0),
                p(0,0,0,0,0,0x3, 0xffffffffffffffff, 0xfffffffffffffffb);

        size_t blocks = msg_len/HALF_KEY_BYTES;
        size_t remain = msg_len%HALF_KEY_BYTES;

        // 16 byte blocks
        for(size_t i=0; i<blocks; ++i) {
            uint512 n(msg+(i*HALF_KEY_BYTES),HALF_KEY_BYTES);
            n.limbs[2] |= 0x01;

            a = (a + n) * r;
            a = a % p;
        }

        // remaining bytes
        if(remain) {
            unsigned char *last_block = new unsigned char[HALF_KEY_BYTES];
            memcpy(last_block,msg+(blocks*HALF_KEY_BYTES),remain);
            memset(last_block+remain+1,0x00,(HALF_KEY_BYTES-remain)-1);
            last_block[remain] = 0x01;

            uint512 n(last_block,HALF_KEY_BYTES);

            a += n;
            a = (r * a) % p;

            delete [] last_block;
        }

        a+=s;

        memcpy(output,(unsigned char*)a.limbs,UINT128BYTES);
        delete [] unclamped_r;
    }

    void key_gen(unsigned char *output, const unsigned char *key, const unsigned int *nonce, unsigned int counter) {
        
        unsigned int *initial_state = new unsigned int[__CHAx220_STATE_DWORDS__];
        unsigned int *transformed_state = new unsigned int[__CHAx220_STATE_DWORDS__];
        
        __internal_chacha20::init_state(initial_state,(unsigned int*)key,counter,nonce);
        __internal_chacha20::apply_20rounds((unsigned int*)transformed_state,initial_state);
        
        // We take the first 256 bits of the serialized state, and use those as the
        // one-time Poly1305 key:
        memcpy(output,transformed_state,32);

        delete [] transformed_state;
        delete [] initial_state;
    }
}

namespace ChaCha20_Poly1305
{
    unsigned char *encrypt( // function parameters
        const unsigned char *key,
        unsigned int         counter,
        const unsigned char *nonce,
        const unsigned char *plaintext,
        size_t len
    ) { // function body
        unsigned char *cipher_text = new unsigned char[len];
        encrypt(cipher_text,key,counter,nonce,plaintext,len);

        return cipher_text;
    }

    void encrypt( // function parameters
        unsigned char       *cipher_text,
        const unsigned char *key,
        unsigned int         counter,
        const unsigned char *nonce,
        const unsigned char *plaintext,
        size_t               len
    ) { // function body

        unsigned int *plaintext_blocked = (unsigned int*) plaintext;

        unsigned int *cipher_blocked = (unsigned int*) cipher_text;

        size_t blocks = len/__CHAx220_BLK_FUNC_OUTPUT_BYTES__;
        size_t lastblock_bytes = (len%__CHAx220_BLK_FUNC_OUTPUT_BYTES__);

        unsigned int *chacha_state = new unsigned int[__CHAx220_STATE_DWORDS__];
        unsigned int *key_stream = new unsigned int[__CHAx220_STATE_DWORDS__];

        // initialize ChaCha20 state
        __internal_chacha20::init_state(
            chacha_state,
            (unsigned int*)key,
            counter,
            (unsigned int*)nonce
        );

        for(size_t i=0; i<blocks; ++i) {

            // perform ChaCha20 Block Function and get the key_stream output
            __internal_chacha20::apply_20rounds(
                key_stream,
                chacha_state
            );

            // increment the chacha state's counter index
            ++chacha_state[__CHACHA_STATE_COUNTER_INDEX__];

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

            // perform ChaCha20 Block Function and get the key_stream output
            __internal_chacha20::apply_20rounds(
                key_stream,
                chacha_state
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

        delete [] chacha_state;
        delete [] key_stream;
    }

    size_t pad16_size(size_t len) {
        if(len%16==0)
            return 0;
        return 16-(len%16);
    }
  
    void aead_encrypt(
        unsigned char       *outputCipher,
        unsigned char       *outputTag,
        const unsigned char *inputText,
        size_t               textLen,
        const unsigned char *AAD,
        size_t               AAD_len,
        const unsigned char *key,
        const unsigned char *iv,
        const unsigned char *constant
    ) {
        unsigned int *nonce = new unsigned int[3];
        unsigned char *nonce_char = (unsigned char*) nonce;

        memcpy(nonce_char,constant,4);
        memcpy(nonce_char+4,iv,8);

        unsigned char *poly1305_key = new unsigned char[32];
        __internal_poly1305::key_gen(poly1305_key,key,(unsigned int*)nonce);

        encrypt(outputCipher,key,1,(unsigned char*)nonce,inputText,textLen);

        size_t padding1 = pad16_size(AAD_len);
        size_t padding2 = pad16_size(textLen);

        size_t mac_len = AAD_len+padding1;
        mac_len += (textLen+padding2);
        mac_len += 16;
        unsigned char *mac_data = new unsigned char[mac_len];

        size_t curr_pos = 0;
        memcpy(mac_data,AAD,AAD_len);
        memset(mac_data+(curr_pos+=AAD_len),0x00,padding1);
        memcpy(mac_data+(curr_pos+=padding1),outputCipher,textLen);
        memset(mac_data+(curr_pos+=textLen),0x00,padding2);
        memcpy(mac_data+(curr_pos+=padding2), &AAD_len,8);
        memcpy(mac_data+(curr_pos+=8), &textLen,8);

        __internal_poly1305::mac(outputTag,poly1305_key,mac_data,mac_len);

        delete [] mac_data;
        delete [] poly1305_key;
        delete [] nonce;
    }

    void aead_decrypt(
        unsigned char       *outputText,
        unsigned char       *outputTag,
        const unsigned char *inputCipher,
        size_t               textLen,
        const unsigned char *AAD,
        size_t               AAD_len,
        const unsigned char *key,
        const unsigned char *iv,
        const unsigned char *constant
    ) {
        unsigned int *nonce = new unsigned int[3];
        unsigned char *nonce_char = (unsigned char*) nonce;

        memcpy(nonce_char,constant,4);
        memcpy(nonce_char+4,iv,8);

        unsigned char *poly1305_key = new unsigned char[32];
        __internal_poly1305::key_gen(poly1305_key,key,(unsigned int*)nonce);

        encrypt(outputText,key,1,(unsigned char*)nonce,inputCipher,textLen);

        size_t padding1 = pad16_size(AAD_len);
        size_t padding2 = pad16_size(textLen);

        size_t mac_len = AAD_len+padding1;
        mac_len += (textLen+padding2);
        mac_len += 16;
        unsigned char *mac_data = new unsigned char[mac_len];

        size_t curr_pos = 0;
        memcpy(mac_data,AAD,AAD_len);
        memset(mac_data+(curr_pos+=AAD_len),0x00,padding1);
        memcpy(mac_data+(curr_pos+=padding1),inputCipher,textLen);
        memset(mac_data+(curr_pos+=textLen),0x00,padding2);
        memcpy(mac_data+(curr_pos+=padding2), &AAD_len,8);
        memcpy(mac_data+(curr_pos+=8), &textLen,8);

        __internal_poly1305::mac(outputTag,poly1305_key,mac_data,mac_len);

        delete [] mac_data;
        delete [] poly1305_key;
        delete [] nonce;
    }

    // nonce version

    void aead_encrypt(
        unsigned char       *outputCipher,
        unsigned char       *outputTag,
        const unsigned char *inputText,
        size_t               textLen,
        const unsigned char *AAD,
        size_t               AAD_len,
        const unsigned char *key,
        const unsigned char *nonce
    ) {

        unsigned char *poly1305_key = new unsigned char[32];
        __internal_poly1305::key_gen(poly1305_key,key,(unsigned int*)nonce);

        encrypt(outputCipher,key,1,(unsigned char*)nonce,inputText,textLen);

        size_t padding1 = pad16_size(AAD_len);
        size_t padding2 = pad16_size(textLen);

        size_t mac_len = AAD_len+padding1;
        mac_len += (textLen+padding2);
        mac_len += 16;
        unsigned char *mac_data = new unsigned char[mac_len];

        size_t curr_pos = 0;
        memcpy(mac_data,AAD,AAD_len);
        memset(mac_data+(curr_pos+=AAD_len),0x00,padding1);
        memcpy(mac_data+(curr_pos+=padding1),outputCipher,textLen);
        memset(mac_data+(curr_pos+=textLen),0x00,padding2);
        memcpy(mac_data+(curr_pos+=padding2), &AAD_len,8);
        memcpy(mac_data+(curr_pos+=8), &textLen,8);

        __internal_poly1305::mac(outputTag,poly1305_key,mac_data,mac_len);

        delete [] mac_data;
        delete [] poly1305_key;
    }

    void aead_decrypt(
        unsigned char       *outputText,
        unsigned char       *outputTag,
        const unsigned char *inputCipher,
        size_t               cipherLen,
        const unsigned char *AAD,
        size_t               AAD_len,
        const unsigned char *key,
        const unsigned char *nonce
    ) {

        unsigned char *poly1305_key = new unsigned char[32];
        __internal_poly1305::key_gen(poly1305_key,key,(unsigned int*)nonce);

        encrypt(outputText,key,1,(unsigned char*)nonce,inputCipher,cipherLen);

        size_t padding1 = pad16_size(AAD_len);
        size_t padding2 = pad16_size(cipherLen);

        size_t mac_len = AAD_len+padding1;
        mac_len += (cipherLen+padding2);
        mac_len += 16;
        unsigned char *mac_data = new unsigned char[mac_len];

        size_t curr_pos = 0;
        memcpy(mac_data,AAD,AAD_len);
        memset(mac_data+(curr_pos+=AAD_len),0x00,padding1);
        memcpy(mac_data+(curr_pos+=padding1),inputCipher,cipherLen);
        memset(mac_data+(curr_pos+=cipherLen),0x00,padding2);
        memcpy(mac_data+(curr_pos+=padding2), &AAD_len,8);
        memcpy(mac_data+(curr_pos+=8), &cipherLen,8);

        __internal_poly1305::mac(outputTag,poly1305_key,mac_data,mac_len);

        delete [] mac_data;
        delete [] poly1305_key;
    }
}

#endif