#ifndef AES256CBC_H_
#define AES256CBC_H_

#include <stdbool.h>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>
#include "Base64.h"

#define ITERATIONS		10000
#define AES_KEY_LEN		32
#define SALT_LEN		16

bool GenerateKeys(const char *password, int plen, unsigned char *aesSalt, unsigned char *aesKey, unsigned char *aesIV);
int Encrypt(char **cipher, const char *plain, int plen, unsigned char *aesKey, unsigned char *aesIV);
int Decrypt(char **plain, const char *cipher, unsigned char *aesKey, unsigned char *aesIV);


#endif