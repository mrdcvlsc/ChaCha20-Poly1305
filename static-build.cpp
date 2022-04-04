/* Compile with:

    g++ static-build.cpp -o static-build.out -lchacha20
*/

#include <ChaCha20-Poly1305.hpp>

int main() {
    std::string lorem =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit,"
        "sed do eiusmod tempor incididunt ut labore et dolore mag"
        "na aliqua. Ut enim ad minim veniam, quis nostrud exercit"
        "ation ullamco laboris nisi ut aliquip ex ea commodo cons"
        "equat. Duis aute irure dolor in reprehenderit in volupta"
        "te velit esse cillum dolore eu fugiat nulla pariatur. Ex"
        "cepteur sint occaecat cupidatat non proident, sunt in cu"
        "lpa qui officia deserunt mollit anim id est laborum.";

    std::string key = "MyRandomKeyThatShouldBe32char...";

    unsigned char AAD[12] = {
        0x50, 0x51, 0x52, 0x53, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
    };

    unsigned char nonce[] = {
        0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
    };

    unsigned char *iv = nonce+4;
    unsigned char *constant = nonce;

    // encryption (with C++ style casting on parameters).
    unsigned char *cipher_text = new unsigned char[lorem.size()];
    unsigned char *encrypt_mac = new unsigned char[POLY1305_MAC_BYTES];
    ChaCha20_Poly1305::aead_encrypt(
        cipher_text,encrypt_mac,
        reinterpret_cast<const unsigned char*>(lorem.data()),lorem.size(),
        AAD,sizeof(AAD),reinterpret_cast<const unsigned char*>(key.data()),iv,constant
    );

    // decryption (with C style casting on parameters).
    unsigned char *recover_text = new unsigned char[lorem.size()];
    unsigned char *decrypt_mac  = new unsigned char[POLY1305_MAC_BYTES];
    ChaCha20_Poly1305::aead_decrypt(
        recover_text,decrypt_mac,
        cipher_text,lorem.size(),
        AAD,sizeof(AAD),(const unsigned char*)key.data(),iv,constant
    );

    // you can use whatever cast you want C or C++.
    
    // compare plain text and recovered text
    bool correct = true;
    for(size_t i=0; i<lorem.size(); ++i) {
        if(recover_text[i]!=(unsigned char)(lorem[i])) {
            correct = false;
            break;
        }
    }

    bool authentic = true;
    for(size_t i=0; i<POLY1305_MAC_BYTES; ++i) {
        if(encrypt_mac[i]!=decrypt_mac[i]) {
            authentic = false;
            break;
        }
    }

    delete [] cipher_text;
    delete [] encrypt_mac;
    delete [] recover_text;
    delete [] decrypt_mac;
    
    if(correct) {
        std::cout << "The Decrypted Message is Correct\n";
    }
    else {
        std::cout << "The Decrypted Message is Wrong!!!\n";
    }

    if(authentic) {
        std::cout << "Message is Authentic\n";
    }
    else
        std::cout << "The Message was Altered\n";

    if(correct && authentic)
        return 0;
    return 1;
}
