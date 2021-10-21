#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include "base64.h"
#include "Logger.h"

unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
BEGIN_HUNE_CORE

int _base64Decode(const unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len )
{
    static char inalphabet[256], decoder[256];
    int c = 0, char_count, errors = 0;
    unsigned int input_idx = 0;
    unsigned int output_idx = 0;

    size_t alphabetSize = sizeof(alphabet);
    for (size_t i = 0; i < alphabetSize; i++){
        inalphabet[alphabet[i]] = 1;
        decoder[alphabet[i]] = i;
    }

    char_count = 0;
    int bits = 0;
    for( input_idx=0; input_idx < input_len ; input_idx++ ) {
        c = input[ input_idx ];
        if (c == '=')
            break;
        if (c > 255 || ! inalphabet[c])
            continue;
        bits += decoder[c];
        char_count++;
        if (char_count == 4) {
            output[ output_idx++ ] = (bits >> 16);
            output[ output_idx++ ] = ((bits >> 8) & 0xff);
            output[ output_idx++ ] = ( bits & 0xff);
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 6;
        }
    }
    
    if( c == '=' ) {
        switch (char_count) {
            case 1:
                errors++;
                break;
            case 2:
                output[ output_idx++ ] = ( bits >> 10 );
                break;
            case 3:
                output[ output_idx++ ] = ( bits >> 16 );
                output[ output_idx++ ] = (( bits >> 8 ) & 0xff);
                break;
            default:
                HUNE_LOG("_base64Decode invalid char_count");
                break;
            }
    } else if ( input_idx < input_len ) {
        if (char_count) {
            errors++;
        }
    }
    
    *output_len = output_idx;
    return errors;
}
    
void _base64Encode( const unsigned char *input, unsigned int input_len, char *output )
{
    unsigned int char_count;
    unsigned int bits;
    unsigned int input_idx = 0;
    unsigned int output_idx = 0;
    
    char_count = 0;
    bits = 0;    
    for( input_idx=0; input_idx < input_len ; input_idx++ ) {
        bits |= input[ input_idx ];
        
        char_count++;
        if (char_count == 3) {
            output[ output_idx++ ] = alphabet[(bits >> 18) & 0x3f];
            output[ output_idx++ ] = alphabet[(bits >> 12) & 0x3f];
            output[ output_idx++ ] = alphabet[(bits >> 6) & 0x3f];
            output[ output_idx++ ] = alphabet[bits & 0x3f];
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 8;
        }
    }
    
    if (char_count) {
        if (char_count == 1) {
            bits <<= 8;
        }

        output[ output_idx++ ] = alphabet[(bits >> 18) & 0x3f];
        output[ output_idx++ ] = alphabet[(bits >> 12) & 0x3f];
        if (char_count > 1) {
            output[ output_idx++ ] = alphabet[(bits >> 6) & 0x3f];
        } else {
            output[ output_idx++ ] = '=';
        }
        output[ output_idx++ ] = '=';
    }
    
    output[ output_idx++ ] = 0;
}
    
unsigned int base64_decode_out(const unsigned char *in, unsigned int inLength, unsigned char **out)
{
    unsigned int outLength = 0;

    *out = new(std::nothrow) unsigned char[inLength / 4 * 3 + 1];
    if (! *out) {
        return outLength;
    }
    memset(*out, 0, inLength / 4 * 3 + 1);
    int ret = _base64Decode(in, inLength, *out, &outLength);
    
    if (ret > 0 )
    {
        delete[] *out;
        *out = NULL;            
        outLength = 0;
    }
    
    return outLength;
}

unsigned int base64_decode(const unsigned char *in, unsigned int in_len, unsigned char * out , unsigned int out_len){
    unsigned int rv = 0 ; 
    if (in == NULL || out == NULL || in_len == 0) return rv ; 
    if (out_len < get_base64_decode_size(in_len)) return rv ; 
    int ret = _base64Decode(in, in_len, out, &rv);
    if (ret > 0){
        rv = 0 ; 
    }
    return rv ; 
}


unsigned int get_base64_decode_size(unsigned int in_len){
    int rv = 0 ; 
    if (in_len == 0) return rv ; 
    rv = in_len / 4 * 3 + 1; 
    return rv ; 
}

unsigned int base64_encode_out(const unsigned char *in, unsigned int inLength, char **out) {
    unsigned int outLength = (inLength + 2) / 3 * 4;
    
    *out = new(std::nothrow) char[outLength + 1];
    if (! *out) {
        return outLength;
    }
    memset(*out, 0, outLength+1);
    memset(*out, 0 , outLength + 1 ); 
    _base64Encode(in, inLength, *out);
    return outLength;
}

unsigned int base64_encode(const unsigned char *in, unsigned int in_len, char * out , unsigned int out_len){
    int rv = 0 ; 
    if (in == NULL || out == NULL || in_len == 0) return rv ; 
    if (out_len < get_base64_encode_size(in_len)) return rv ; 
    _base64Encode(in, in_len, out);
    rv = out_len ; 
    return rv ; 
}

unsigned int get_base64_encode_size(unsigned int in_len){
    int rv = 0 ; 
    if (in_len == 0) return rv ; 
    rv = (in_len + 2) / 3 * 4; 
    rv += 1 ; 
    return rv ; 
}

END_HUNE_CORE
