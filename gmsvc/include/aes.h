/*********************************************************************
* Filename:   aes.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding AES implementation.
*********************************************************************/

#ifndef AES_H
#define AES_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define AES_BLOCK_SIZE 16               // AES operates on 16 unsigned chars at a time

/**************************** DATA TYPES ****************************/
//typedef unsigned char unsigned char;            // 8-bit unsigned char
//typedef unsigned int unsigned int;             // 32-bit word, change to "long" for 16-bit machines

/*********************** FUNCTION DECLARATIONS **********************/
///////////////////
// AES
///////////////////
// Key setup must be done before any AES en/de-cryption functions can be used.
void aes_key_setup(const unsigned char key[],          // The key, must be 128, 192, or 256 bits
                   unsigned int w[],                  // Output key schedule to be used later
                   int keysize);              // Bit length of the key, 128, 192, or 256

void aes_encrypt(const unsigned char in[],             // 16 unsigned chars of plaintext
                 unsigned char out[],                  // 16 unsigned chars of ciphertext
                 const unsigned int key[],            // From the key setup
                 int keysize);                // Bit length of the key, 128, 192, or 256

void aes_decrypt(const unsigned char in[],             // 16 unsigned chars of ciphertext
                 unsigned char out[],                  // 16 unsigned chars of plaintext
                 const unsigned int key[],            // From the key setup
                 int keysize);                // Bit length of the key, 128, 192, or 256

///////////////////
// AES - CBC
///////////////////
int aes_encrypt_cbc(const unsigned char in[],          // Plaintext
                    size_t in_len,            // Must be a multiple of AES_BLOCK_SIZE
                    unsigned char out[],               // Ciphertext, same length as plaintext
                    const unsigned int key[],         // From the key setup
                    int keysize,              // Bit length of the key, 128, 192, or 256
                    const unsigned char iv[]);         // IV, must be AES_BLOCK_SIZE unsigned chars long

// Only output the CBC-MAC of the input.
int aes_encrypt_cbc_mac(const unsigned char in[],      // plaintext
                        size_t in_len,        // Must be a multiple of AES_BLOCK_SIZE
                        unsigned char out[],           // Output MAC
                        const unsigned int key[],     // From the key setup
                        int keysize,          // Bit length of the key, 128, 192, or 256
                        const unsigned char iv[]);     // IV, must be AES_BLOCK_SIZE unsigned chars long

///////////////////
// AES - CTR
///////////////////
void increment_iv(unsigned char iv[],                  // Must be a multiple of AES_BLOCK_SIZE
                  int counter_size);          // unsigned chars of the IV used for counting (low end)

void aes_encrypt_ctr(const unsigned char in[],         // Plaintext
                     size_t in_len,           // Any unsigned char length
                     unsigned char out[],              // Ciphertext, same length as plaintext
                     const unsigned int key[],        // From the key setup
                     int keysize,             // Bit length of the key, 128, 192, or 256
                     const unsigned char iv[]);        // IV, must be AES_BLOCK_SIZE unsigned chars long

void aes_decrypt_ctr(const unsigned char in[],         // Ciphertext
                     size_t in_len,           // Any unsigned char length
                     unsigned char out[],              // Plaintext, same length as ciphertext
                     const unsigned int key[],        // From the key setup
                     int keysize,             // Bit length of the key, 128, 192, or 256
                     const unsigned char iv[]);        // IV, must be AES_BLOCK_SIZE unsigned chars long

///////////////////
// AES - CCM
///////////////////
// Returns True if the input parameters do not violate any constraint.
int aes_encrypt_ccm(const unsigned char plaintext[],              // IN  - Plaintext.
                    unsigned int plaintext_len,                  // IN  - Plaintext length.
                    const unsigned char associated_data[],        // IN  - Associated Data included in authentication, but not encryption.
                    unsigned short associated_data_len,  // IN  - Associated Data length in unsigned chars.
                    const unsigned char nonce[],                  // IN  - The Nonce to be used for encryption.
                    unsigned short nonce_len,            // IN  - Nonce length in unsigned chars.
                    unsigned char ciphertext[],                   // OUT - Ciphertext, a concatination of the plaintext and the MAC.
                    unsigned int *ciphertext_len,                // OUT - The length of the ciphertext, always plaintext_len + mac_len.
                    unsigned int mac_len,                        // IN  - The desired length of the MAC, must be 4, 6, 8, 10, 12, 14, or 16.
                    const unsigned char key[],                    // IN  - The AES key for encryption.
                    int keysize);                        // IN  - The length of the key in bits. Valid values are 128, 192, 256.

// Returns True if the input parameters do not violate any constraint.
// Use mac_auth to ensure decryption/validation was preformed correctly.
// If authentication does not succeed, the plaintext is zeroed out. To overwride
// this, call with mac_auth = NULL. The proper proceedure is to decrypt with
// authentication enabled (mac_auth != NULL) and make a second call to that
// ignores authentication explicitly if the first call failes.
int aes_decrypt_ccm(const unsigned char ciphertext[],             // IN  - Ciphertext, the concatination of encrypted plaintext and MAC.
                    unsigned int ciphertext_len,                 // IN  - Ciphertext length in unsigned chars.
                    const unsigned char assoc[],                  // IN  - The Associated Data, required for authentication.
                    unsigned short assoc_len,            // IN  - Associated Data length in unsigned chars.
                    const unsigned char nonce[],                  // IN  - The Nonce to use for decryption, same one as for encryption.
                    unsigned short nonce_len,            // IN  - Nonce length in unsigned chars.
                    unsigned char plaintext[],                    // OUT - The plaintext that was decrypted. Will need to be large enough to hold ciphertext_len - mac_len.
                    unsigned int *plaintext_len,                 // OUT - Length in unsigned chars of the output plaintext, always ciphertext_len - mac_len .
                    unsigned int mac_len,                        // IN  - The length of the MAC that was calculated.
                    int *mac_auth,                       // OUT - TRUE if authentication succeeded, FALSE if it did not. NULL pointer will ignore the authentication.
                    const unsigned char key[],                    // IN  - The AES key for decryption.
                    int keysize);                        // IN  - The length of the key in BITS. Valid values are 128, 192, 256.




int aesEncrypt(char *src, int len, char *enc, char *key);
int aesDecrypt(char *src, int len, char *dec, char *key);
#endif   // AES_H
