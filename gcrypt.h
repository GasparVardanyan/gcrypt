# ifndef GCRYPT_GCRYPT_H__
# define GCRYPT_GCRYPT_H__

# include <stddef.h>

# define GCRYPT_VERSION "v3.3"
# define G_ACTION(A) void (A) (const char * const key, const char * const restrict ifile, const char * const restrict ofile, const size_t b)

# ifdef __cplusplus
extern "C" {
# endif // __cplusplus

extern G_ACTION (gEncryptF);
extern G_ACTION (gDecryptF);

# ifdef __cplusplus
}
# endif // __cplusplus

# endif // GCRYPT_GCRYPT_H__
