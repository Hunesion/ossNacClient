#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include "crypto.h"
#include "base64.h"


#define AES_128 0 
#define AES_256 1 


#define HMAC_ITERATIONS 10 
#define IV_LEN 16 
#define SALT_LEN 32
#define AES128_KEY_LEN 16 

BEGIN_HUNE_CORE

static const unsigned char AES_IV[IV_LEN] = {0xF2, 0x7D, 0x5C, 0x99, 0x27, 0x72, 0x6B, 0xCE, 0xFE, 0x75, 0x10, 0xB1, 0xBD, 0xD3, 0xD1, 0x37};
static unsigned char enc_aes_128_key[256] ;
static std::string s_enc_key_integrity ;
static bool s_valid_enc_key_integrity = false ; 

void crypto_dispose(){
    memset(enc_aes_128_key, 0, 256); 
    s_enc_key_integrity.clear() ;
    s_valid_enc_key_integrity = false ;  
}

bool convert_aes_128_key(const char *key, unsigned char *out, int outlen){
    bool rv = false ;
    int len = 0 ;  
    memset(out, 0, outlen); 

    if (key == NULL) return rv ;
    len = strlen(key); 

    unsigned char hmac_salt[32] = {0x3F, 0xF2, 0xEC, 0x01, 0x9C, 0x62, 0x7B, 0x94, 0x52, 0x25, 0xDE, 0xBA, 0xD7, 0x1A, 0x01, 0xB6, 0x98, 0x5F, 0xE8, 0x4C, 0x95, 0xA7, 0x0E, 0xB1, 0x32, 0x88, 0x2F, 0x88, 0xC0, 0xA5, 0x9A, 0x55}; 
    if (!PKCS5_PBKDF2_HMAC_SHA1(key, len , hmac_salt , 32 , HMAC_ITERATIONS, AES128_KEY_LEN, out)) return rv ;     
    rv = true ;
    return rv ;  
}

bool set_aes_128_global_key(const char *key){
    bool rv = false ;
    memset(enc_aes_128_key, 0, 256); 
    s_enc_key_integrity.clear() ;
    s_valid_enc_key_integrity = false ;  

    if (!convert_aes_128_key(key, enc_aes_128_key, 256)) return rv;
    
    rv = true ;
    s_enc_key_integrity = key ;  
    s_valid_enc_key_integrity = true ; 
    return rv ;  
}

int aes_cbc_encrypt(int bit, const unsigned char *key , const unsigned char *iv , const unsigned char *in , int in_len, unsigned char **out ){
    int rv = 0 ;
    EVP_CIPHER_CTX *ctx = NULL ;
    const EVP_CIPHER *cipher = NULL ; 
    int padding = 0 ; 
    unsigned char *result = NULL ; 
    int result_len = 0 , out_len = 0 , cipher_text_len = 0 ; 


    if (in == NULL || out == NULL || in_len <= 0) return rv ; 
    *out = NULL ; 

     
    padding = AES_BLOCK_SIZE - (in_len % AES_BLOCK_SIZE); 
    result_len = in_len + padding ;


    switch (bit){
        case AES_128:
            cipher = EVP_aes_128_cbc(); 
            break ; 
        case AES_256: 
            cipher = EVP_aes_256_cbc(); 
            break ; 
        default :
            cipher = NULL ; 
            break ; 
    }

    if (!cipher) return rv ; 

    if (!(ctx = EVP_CIPHER_CTX_new())) return rv ; 

    if (EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv)!= 1) goto end ; 

    result_len = in_len + padding  + AES_BLOCK_SIZE;
    result = (unsigned char *) malloc(sizeof(unsigned char) * result_len); 
    if (!result) goto end;
    memset(result , 0, result_len); 

    if (EVP_EncryptUpdate(ctx, result, &out_len, in, in_len) != 1) goto end ; 
    cipher_text_len = out_len ; 

    if (cipher_text_len > result_len) goto end ; 

    if (EVP_EncryptFinal_ex(ctx, result + out_len, &out_len) != 1) goto end ; 
    cipher_text_len += out_len ; 
    rv = 1 ; 
    
end : 
    if (ctx){
        EVP_CIPHER_CTX_free(ctx); 
    }

    if (!rv && result != NULL){
        free(result); 
        result = NULL ; 
        cipher_text_len = 0 ; 
    }

    *out = result ; 
    rv = cipher_text_len ;
    
    return rv ; 
}


int aes_cbc_decrypt(int bit, const unsigned char *key , const unsigned char *iv , const unsigned char *in , int in_len, unsigned char **out ){
    int rv = 0 ;
    EVP_CIPHER_CTX *ctx = NULL ;
    const EVP_CIPHER *cipher = NULL ; 
    unsigned char *result = NULL ; 
    int result_len = 0 , out_len = 0 , plain_text_len = 0 ; 


    if (in == NULL || out == NULL || in_len <= 0) return rv ; 
    *out = NULL ;

    switch (bit){
        case AES_128:
            cipher = EVP_aes_128_cbc(); 
            break ; 
        case AES_256: 
            cipher = EVP_aes_256_cbc(); 
            break ; 
        default :
            cipher = NULL ; 
            break ; 
    }

    if (!cipher) return rv ; 

    if (!(ctx = EVP_CIPHER_CTX_new())) return rv ; 

    if (EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv)!= 1) goto end ; 

    result_len = in_len  + AES_BLOCK_SIZE; 
    result = (unsigned char *) malloc(sizeof(unsigned char) * result_len); 
    if (!result) goto end;
    memset(result , 0, result_len); 

    if (EVP_DecryptUpdate(ctx, result, &out_len, in, in_len) != 1) goto end ; 
    plain_text_len = out_len ; 

    if (plain_text_len > result_len) goto end ; 

    if (EVP_DecryptFinal_ex(ctx, result + out_len, &out_len) != 1) goto end ; 
    plain_text_len += out_len ; 
    rv = 1 ; 
    
end : 
    if (ctx){
        EVP_CIPHER_CTX_free(ctx); 
    }

    if (!rv && result != NULL){
        free(result); 
        result = NULL ; 
        plain_text_len = 0 ; 
    }

    *out = result ; 
    rv = plain_text_len ;
    
    return rv ; 
}

int aes_128_cbc_decrypt(const unsigned char* key, int key_len, const unsigned char *in , int in_len, unsigned char **out){
    int rv = 0 ;
    
    unsigned char iv[IV_LEN];  
    
    
    if (in == NULL || out == NULL || in_len <= 0) return rv ; 
    *out = NULL ; 

    memcpy (iv, AES_IV, 16); 
    rv = aes_cbc_decrypt(AES_128, key, iv , in , in_len , out); 

    return rv ; 
}

int aes_128_cbc_encrypt(const unsigned char* key, int key_len, const unsigned char *in , int in_len, unsigned char **out){
    int rv = 0 ;
    
    unsigned char iv[IV_LEN];  
    
    
    if (in == NULL || out == NULL || in_len <= 0) return rv ; 
    *out = NULL ; 

    memcpy (iv, AES_IV, 16); 
    rv = aes_cbc_encrypt(AES_128, key, iv , in , in_len , out); 

    return rv ; 
}

std::string aes_128_encrypt_and_base64(const char* in , int in_len){
    std::string rv ; 
    unsigned char *enc = NULL ; 
    int enc_len = 0 ; 
    char *base64 = NULL ;
    int base64_len = 0 ;  
    if (in == NULL || in_len <= 0) return rv ; 
    enc_len = aes_128_cbc_encrypt(enc_aes_128_key, AES_DEFAULT_KEY_SIZE , (const unsigned char*)in, in_len, &enc); 
    if (enc_len <= 0 || enc == NULL) goto END ; 

    base64_len = base64_encode_out(enc, enc_len, &base64); 
    if (base64_len <= 0 || base64 == NULL) goto END ; 
    rv = base64; 
    
END : 
    if (enc != NULL){
        free(enc); 
        enc = NULL ; 
    }

    if (base64 != NULL){
        free(base64); 
        base64 = NULL ; 
    }
    return rv ; 
}

std::string aes_128_encrypt_and_base64(const char *key, const char *in , int in_len){
    std::string rv ; 
    unsigned char *enc = NULL ; 
    unsigned char hkey[256] = { 0, };
    int enc_len = 0 ; 
    char *base64 = NULL ;
    int base64_len = 0 ;  
    if (in == NULL || in_len <= 0) return rv ; 

    convert_aes_128_key(key, hkey, 256);
    enc_len = aes_128_cbc_encrypt(hkey, AES_DEFAULT_KEY_SIZE , (const unsigned char*)in, in_len, &enc); 

    if (enc_len <= 0 || enc == NULL) goto END ; 

    base64_len = base64_encode_out(enc, enc_len, &base64); 
    if (base64_len <= 0 || base64 == NULL) goto END ; 
    rv = base64; 
    
END : 
    if (enc != NULL){
        free(enc); 
        enc = NULL ; 
    }

    if (base64 != NULL){
        free(base64); 
        base64 = NULL ; 
    }
    return rv ; 
}

std::string aes_128_decrypt_and_base64(const char *key, const char *in , int in_len)
{
    std::string rv, plain ;
    unsigned char *base64 = NULL ;
    unsigned char *dec = NULL ;
    unsigned char hkey[256] = { 0, };
    int base64_len = 0 , dec_len = 0 ; 

    if (key == NULL || in == NULL || in_len <= 0) return rv ; 

    base64_len = base64_decode_out((unsigned char *)in, in_len, &base64); 
    if (base64_len <= 0 || base64 == NULL) goto END ; 

    convert_aes_128_key(key, hkey, 256);
    dec_len = aes_128_cbc_decrypt(hkey, AES_DEFAULT_KEY_SIZE, (const unsigned char*)base64, base64_len, &dec);

    if (dec_len <= 0 || dec == NULL) goto END ; 
    plain = (char*)dec ; 
    if (plain.length() != dec_len) {
        rv = plain.substr(0, dec_len);
    } else {
        rv = plain;
    }
    
END : 
    if (base64 != NULL){
        delete[] base64; 
        base64 = NULL ; 
    }

    if (dec != NULL){
        free(dec); 
        dec = NULL ; 
    }
    return rv ; 
}

std::string aes_256_encrypt_and_base64(const char *key , const char *in , int in_len){
        std::string rv ; 
    unsigned char *enc = NULL ; 
    unsigned char iv[IV_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
    int enc_len = 0 ; 
    char *base64 = NULL ;
    int base64_len = 0 ;  
    if (in == NULL || in_len <= 0 || key == NULL) return rv ; 

    enc_len = aes_cbc_encrypt(AES_256, (const unsigned char*)key , iv, (const unsigned char*)in, in_len, &enc); 

    if (enc_len <= 0 || enc == NULL) goto END ; 

    base64_len = base64_encode_out(enc, enc_len, &base64); 
    if (base64_len <= 0 || base64 == NULL) goto END ; 
    rv = base64; 
    
END : 
    if (enc != NULL){
        free(enc); 
        enc = NULL ; 
    }

    if (base64 != NULL){
        free(base64); 
        base64 = NULL ; 
    }
    return rv ; 
}


std::string aes_256_decrypt_and_base64(const char *key, const char *in , int in_len){
    std::string rv, plain ; 
    unsigned char iv[IV_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
    unsigned char *base64 = NULL ;
    unsigned char *dec = NULL ;
    int base64_len = 0 , dec_len = 0 ; 

    if (key == NULL || in == NULL || in_len <= 0) return rv ; 

    base64_len = base64_decode_out((unsigned char *)in, in_len, &base64); 
    if (base64_len <= 0 || base64 == NULL) goto END ; 
    dec_len = aes_cbc_decrypt(AES_256 , (const unsigned char *)key, iv, base64, base64_len, &dec); 

    if (dec_len <= 0 || dec == NULL) goto END ; 
    plain = (char*)dec ; 
    if (plain.length() != dec_len) {
        rv = plain.substr(0, dec_len);
    } else {
        rv = plain;
    }
    
    
END : 
    if (base64 != NULL){
        delete[] base64; 
        base64 = NULL ; 
    }

    if (dec != NULL){
        free(dec); 
        dec = NULL ; 
    }
    return rv ; 
}

char* aes_256_encrypt_out(const char *key, const char *in, int in_len, int *out_len)
{
    unsigned char iv[IV_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
    unsigned char *enc = NULL ;
    char *rv = NULL;
    int enc_len = 0 ; 

    if (key == NULL || in == NULL || in_len <= 0) return rv ; 

    enc_len = aes_cbc_encrypt(AES_256, (const unsigned char *)key, iv, (unsigned char*)in, in_len, &enc); 

    if (enc_len > 0) {
        if (out_len) {
            *out_len = enc_len;
        }

        rv = (char*)enc;    
    }
    
    return rv ; 
}

char* aes_256_decrypt_out(const char *key, const char *in, int in_len, int *out_len)
{
    unsigned char iv[IV_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
    unsigned char *dec = NULL ;
    char *rv = NULL;
    int dec_len = 0 ; 

    if (key == NULL || in == NULL || in_len <= 0) return rv ; 

    dec_len = aes_cbc_decrypt(AES_256, (const unsigned char *)key, iv, (unsigned char*)in, in_len, &dec); 

    if (dec_len > 0) {
        if (out_len) {
            *out_len = dec_len;
        }

        rv = (char*)dec;    
    }
    
    return rv ; 
}


int bin2hex(const unsigned char *in, const int in_len , char *out , const int out_len){
    int rv = 0 , i = 0 , j = 0 ; 
    if (in == NULL || out == NULL || in_len <= 0 || out_len < (in_len * 2) ) return rv ; 
    memset(out, 0 , out_len); 
    for (i = j = 0; i < in_len; i++) {
        int b = 0;
        b = in[i] >> 4;
        out[j++] = (char) (87 + b + (((b - 10) >> 31) & -39));
        b = in[i] & 0xf;
        out[j++] = (char) (87 + b + (((b - 10) >> 31) & -39));
    }
    rv = 1 ; 
    return rv ; 
}
char* bin2hex_out(const unsigned char *in, const int in_len, int *out_len){
    char* rv = NULL ; 
    int len = 0 ; 

    if (in == NULL || out_len == NULL || in_len <= 0) return rv ; 
    *out_len = len ; 

    len = (in_len *2 ) + 1 ; 
    *out_len = len ; 
    rv = (char*) malloc(len) ; 
    bin2hex(in, in_len , rv, len); 
    return rv ; 
}

int hex2bin(const unsigned char *in, const int in_len , char *out , const int out_len){
    int rv = 0 , i = 0  ; 
    int target_length = in_len >> 1;
    if (in == NULL || out == NULL || in_len <= 0 || out_len < target_length) return rv ; 

    memset(out, 0 , out_len); 

     for (i = 0; i < target_length; i++) {
        int temp = -1;
        int t2 = 0;
        int ch = (unsigned char) in[2 * i];
        temp += (((0x2f - ch) & (ch - 0x3a)) >> 8) & (ch - 47);
        ch |= 0x20;
        temp += (((0x60 - ch) & (ch - 0x67)) >> 8) & (ch - 86);
        t2 = temp << 4;
        temp = -1;
        ch = (unsigned char) in[2 * i + 1];
        temp += (((0x2f - ch) & (ch - 0x3a)) >> 8) & (ch - 47);
        ch |= 0x20;
        temp += (((0x60 - ch) & (ch - 0x67)) >> 8) & (ch - 86);
        t2 |= temp;

        if (0 != (t2 >> 8)) {
            return rv;
        }
        out[i] = (char) t2 & 0xFF;
    }
    rv = 1 ; 
    return rv ; 

}
char* hex2bin_out(const unsigned char *in, const int in_len, int *out_len){
    int target_length = in_len >> 1;
    char *rv = NULL ; 

    if (in == NULL || out_len == NULL || in_len <= 0) return rv ; 
    *out_len = 0 ;

    rv = (char*) malloc(target_length); 

    if (!hex2bin(in, in_len, rv, target_length)){
        free(rv); 
        rv = NULL ; 
    }else {
        *out_len = target_length; 
    }

    return rv ; 
}


int get_sha256(const char *in, const int in_len, char *out , const int out_len){
    int rv = 0 ; 
    unsigned char hash[SHA256_DIGEST_LENGTH] = { 0, } ; 
    SHA256_CTX sha ; 
    if (in == NULL || out == NULL || in_len <= 0 || out_len < SHA256_HEX_LENGTH) return rv ; 
    memset(out , 0, out_len); 
    SHA256_Init(&sha); 
    SHA256_Update(&sha, in , in_len); 
    SHA256_Final(hash, &sha); 

    bin2hex(hash, SHA256_DIGEST_LENGTH, out , out_len); 
    rv = 1 ; 
    return rv ; 
}


int get_sha512(const char *in, const int in_len, char *out, const int out_len){
    int rv = 0 ; 
    unsigned char hash[SHA512_DIGEST_LENGTH] = { 0, }; 
    SHA512_CTX sha ; 
    if (in == NULL || out == NULL || in_len <= 0 || out_len < SHA512_HEX_LENGTH ) return rv ; 
    memset(out , 0, out_len); 
    SHA512_Init(&sha); 
    SHA512_Update(&sha, in , in_len); 
    SHA512_Final(hash, &sha); 
    bin2hex(hash, SHA512_DIGEST_LENGTH, out , out_len); 
    rv = 1 ; 
    return rv ;
}

END_HUNE_CORE