#include "base64.h"

int base64Encode(char **dest, unsigned char *src, unsigned int slen) {

	BIO *bio, *b64;
	BUF_MEM *bufferPtr;
	int numBytesEncoded = 0;

	b64 = BIO_new(BIO_f_base64());
	if (!b64) return 0;

	bio = BIO_new(BIO_s_mem());
	if (!bio) return 0;

	bio = BIO_push(b64, bio);
	if (!bio) return 0;

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

	if (BIO_write(bio, src, slen - 1) <= 0) {
		if (bio) BIO_free_all(bio);
		return 0;
	}

	if (1 != BIO_flush(bio)) {
		if (bio) BIO_free_all(bio);
		return 0;
	}

	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);

	if (bio) BIO_free_all(bio);

	*dest = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (*bufferPtr).length + 1);
	if (*dest == NULL) return false;

	(*bufferPtr).data[(*bufferPtr).length] = '\0';
	strncpy_s(*dest, (*bufferPtr).length + 1, (*bufferPtr).data, (*bufferPtr).length);

	numBytesEncoded = (*bufferPtr).length + 1;

	if (bufferPtr) {
		free(bufferPtr);
		bufferPtr = NULL;
	}

	return numBytesEncoded;
}

int base64Decode(unsigned char **dest, const char *src) {

	unsigned int dlen = 0;
	BIO *bio, *b64;

	unsigned int decode_length = calcDecodedLength(src);

	*dest = (unsigned char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, decode_length + 1);
	if (*dest == NULL) return false;

	bio = BIO_new_mem_buf((char*)src, -1);
	if (!bio) return 0;

	b64 = BIO_new(BIO_f_base64());
	if (!b64) return 0;

	bio = BIO_push(b64, bio);
	if (!bio) return 0;

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

	dlen = BIO_read(bio, *dest, strlen(src));

	if (dlen != decode_length) {
		if (dest) {
			HeapFree(GetProcessHeap(), 0, dest);
			dest = NULL;
		}
		if (bio) BIO_free_all(bio);
		return false;
	}

	if (bio) BIO_free_all(bio);

	*(*dest + decode_length) = '\0';

	return decode_length + 1;
}

unsigned int calcDecodedLength(const char *encoded) {

	unsigned int len = strlen(encoded), padding = 0;

	if (encoded[len - 1] == '=' && encoded[len - 2] == '=')
		padding = 2;

	else if (encoded[len - 1] == '=')
		padding = 1;

	return (len * 3) / 4 - padding;
}
