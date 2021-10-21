#ifndef _HUNE_CORE_CRYPTO_H_
#define _HUNE_CORE_CRYPTO_H_

#include "macro.h"
#include <string>
#include <openssl/sha.h>

#define SHA256_HEX_LENGTH (SHA256_DIGEST_LENGTH * 2) + 1 
#define SHA512_HEX_LENGTH (SHA512_DIGEST_LENGTH * 2) + 1 

#define AES_DEFAULT_KEY "Hunesion.Corp.2009.11.09richmail"

#define AES_DEFAULT_KEY_SIZE 32

BEGIN_HUNE_CORE
int bin2hex(const unsigned char *in, const int in_len , char *out , const int out_len); 
char* bin2hex_out(const unsigned char *in, const int in_len, int *out_len); 

int hex2bin(const unsigned char *in, const int in_len , char *out , const int out_len); 
char* hex2bin_out(const unsigned char *in, const int in_len, int *out_len); 

int get_sha256(const char *in, const int in_len, char *out, const int out_len); 
int get_sha512(const char *in, const int in_len, char *out, const int out_len);  

void crypto_dispose(); 
bool set_aes_128_global_key(const char *key);  
std::string aes_128_encrypt_and_base64(const char *in , int in_len); 
std::string aes_128_encrypt_and_base64(const char *key, const char *in , int in_len); 
std::string aes_256_encrypt_and_base64(const char *key, const char *in , int in_len);

char* aes_256_encrypt_out(const char *key, const char *in, int in_len, int *out_len);
char* aes_256_decrypt_out(const char *key, const char *in, int in_len, int *out_len);
/*
    in : base64 String 
*/
std::string aes_256_decrypt_and_base64(const char *key, const char *in , int in_len); 
std::string aes_128_decrypt_and_base64(const char *key, const char *in , int in_len); 
END_HUNE_CORE 

#endif /* _HUNE_CORE_CRYPTO_H_ */