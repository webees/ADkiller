#ifndef BASE64_H_
#define BASE64_H_

#include <windows.h>
#include <stdbool.h>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

int base64Encode(char **dest, unsigned char *src, unsigned int slen);
int base64Decode(unsigned char **dest, const char *src);
unsigned int calcDecodedLength(const char *encoded);

#endif