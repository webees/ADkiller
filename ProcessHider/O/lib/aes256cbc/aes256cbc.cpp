
#include "aes256cbc.h"

bool GenerateKeys(const char *password, int plen, unsigned char *aesSalt, unsigned char *aesKey, unsigned char *aesIV) {

	if (RAND_bytes(aesSalt, SALT_LEN) == 0)
		return false;
	aesSalt[SALT_LEN] = '\0';

	if (RAND_bytes(aesIV, EVP_MAX_IV_LENGTH) == 0)
		return false;

	aesIV[EVP_MAX_IV_LENGTH] = '\0';

	if (PKCS5_PBKDF2_HMAC_SHA1(password, AES_KEY_LEN, aesSalt, PKCS5_SALT_LEN, ITERATIONS, AES_KEY_LEN, aesKey) == 0)
		return false;

	return true;
}


int Encrypt(char **cipher, const char *plain, int plen, unsigned char *aesKey, unsigned char *aesIV) {

	EVP_CIPHER_CTX *ctx;
	unsigned char *cipher_tmp = { 0 };
	int len = 0, cipherTextLen = 0;

	if (!(ctx = EVP_CIPHER_CTX_new())) {
		return 0;
	}

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aesKey, aesIV)) {
		if (ctx) EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	cipher_tmp = (unsigned char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, plen + 16);
	if (cipher_tmp == NULL) {
		if (ctx) EVP_CIPHER_CTX_free(ctx);
		return 0;
	}

	if (1 != EVP_EncryptUpdate(ctx, cipher_tmp, &len, (const unsigned char *)plain, plen - 1)) {
		if (ctx) EVP_CIPHER_CTX_free(ctx);
		if (cipher_tmp) {
			HeapFree(GetProcessHeap(), 0, cipher_tmp);
			cipher_tmp = NULL;
		}
		return 0;
	}

	cipherTextLen = len;

	if (1 != EVP_EncryptFinal_ex(ctx, cipher_tmp + len, &len)) {
		if (ctx) EVP_CIPHER_CTX_free(ctx);
		if (cipher_tmp) {
			HeapFree(GetProcessHeap(), 0, cipher_tmp);
			cipher_tmp = NULL;
		}
		return 0;
	}

	cipherTextLen += len;

	if (ctx) EVP_CIPHER_CTX_free(ctx);

	if (cipherTextLen <= 0) {
		if (cipher_tmp) {
			HeapFree(GetProcessHeap(), 0, cipher_tmp);
			cipher_tmp = NULL;
		}
		return 0;
	}

	cipher_tmp[cipherTextLen] = '\0';

	if ((cipherTextLen = base64Encode(cipher, cipher_tmp, cipherTextLen + 1)) <= 0) {
		if (cipher_tmp) {
			HeapFree(GetProcessHeap(), 0, cipher_tmp);
			cipher_tmp = NULL;
		}
		return 0;
	}

	if (cipher_tmp) {
		HeapFree(GetProcessHeap(), 0, cipher_tmp);
		cipher_tmp = NULL;
	}

	return cipherTextLen;
}


int Decrypt(char **plain, const char *cipher, unsigned char *aesKey, unsigned char *aesIV) {

	EVP_CIPHER_CTX *ctx;
	int len = 0, b64DecodedLen = 0, plainTextLen = 0, retValue = 0;
	unsigned char *plain_tmp = { 0 };

	b64DecodedLen = base64Decode(&plain_tmp, cipher);
	if (b64DecodedLen == 0) return 0;

	if (!(ctx = EVP_CIPHER_CTX_new())) {
		if (plain_tmp) {
			HeapFree(GetProcessHeap(), 0, plain_tmp);
			plain_tmp = NULL;
		}
		return 0;
	}

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aesKey, aesIV)) {
		if (ctx) EVP_CIPHER_CTX_free(ctx);
		if (plain_tmp) {
			HeapFree(GetProcessHeap(), 0, plain_tmp);
			plain_tmp = NULL;
		}
		return 0;
	}

	*plain = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, b64DecodedLen);
	if (*plain == NULL) return 0;

	if (1 != EVP_DecryptUpdate(ctx, (unsigned char *)*plain, &len, plain_tmp, b64DecodedLen - 1)) {
		if (ctx) EVP_CIPHER_CTX_free(ctx);
		if (plain_tmp) {
			HeapFree(GetProcessHeap(), 0, plain_tmp);
			plain_tmp = NULL;
		}
		if (plain) {
			HeapFree(GetProcessHeap(), 0, plain);
			plain = NULL;
		}
		return 0;
	}

	if (plain_tmp) {
		HeapFree(GetProcessHeap(), 0, plain_tmp);
		plain_tmp = NULL;
	}

	plainTextLen = len;

	if (1 != EVP_DecryptFinal_ex(ctx, (unsigned char *)*plain + len, &len)) {
		if (ctx) EVP_CIPHER_CTX_free(ctx);
		if (plain) {
			HeapFree(GetProcessHeap(), 0, plain);
			plain = NULL;
		}
		return 0;
	}

	plainTextLen += len;
	retValue = plainTextLen;

	*(*plain + plainTextLen) = '\0';

	if (ctx) EVP_CIPHER_CTX_free(ctx);

	return retValue;
}