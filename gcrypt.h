# ifndef GCRYPT_GCRYPT_H__
# define GCRYPT_GCRYPT_H__

# include <stdint.h>
# include <stddef.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# define GCRYPT_VERSION "v3.2"

//# ifdef __cplusplus
//extern "C" {
//# endif // __cplusplus

extern void gEncryptF (const char * const key, const char * const ifile, const char * const ofile, const size_t b);
extern void gDecryptF (const char * const key, const char * const ifile, const char * const ofile, const size_t b);

//# ifdef __cplusplus
//}
//# endif // __cplusplus

# endif // GCRYPT_GCRYPT_H__
