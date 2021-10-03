# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "gcrypt.h"

# define GCRYPT_F_PROCESS_BEGIN										\
	FILE															\
		* istream = fopen (ifile, "rb"),							\
		* ostream = fopen (ofile, "wb")								\
	;																\
	if (!istream)													\
	{																\
		fputs ("Error opening ifile. Aborting.\n", stderr);			\
		return;														\
	}																\
	if (!ostream)													\
	{																\
		fputs ("Error opening ofile. Aborting.\n", stderr);			\
		return;														\
	}																\
																	\
	unsigned char													\
		* inp,														\
		hash [256]													\
	;																\
																	\
	if ((inp = malloc (b)) == NULL)									\
	{																\
		fputs ("Error allocating buffer. Aborting.\n", stderr);		\
		goto close_streams;											\
	}																\
																	\
	gHash ((const unsigned char * const) key, hash);				\
																	\
	register size_t bufsize, i;										\
	register unsigned char rot = 0;									\
// GCRYPT_F_PROCESS_BEGIN

# define GCRYPT_F_PROCESS_END										\
	if (ferror (istream) || !feof (istream))						\
		fputs ("Error reading ifile. Aborting.\n", stderr);			\
	if (ferror (ostream))											\
		fputs ("Error writing ofile. Aborting.\n", stderr);			\
																	\
	free (inp);														\
close_streams:														\
	fclose (istream);												\
	fclose (ostream);												\
// GCRYPT_F_PROCESS_END

# define GCRYPT_F_PROCESS_WRITE(pattern)							\
	while ((bufsize = fread (inp, 1, b, istream)))					\
	{																\
		for (i = 0; i < bufsize; i++)								\
			pattern;												\
		fwrite (inp, 1, bufsize, ostream);							\
	}																\
// GCRYPT_F_PROCESS_WRITE

# define GCRYPT_F_PROCESS_DECHASH									\
	unsigned char hashDec [256];									\
	for (i = 0; i < 256; i++)										\
		hashDec [hash [i]] = i;										\
// GCRYPT_F_PROCESS_DECHASH

# define GCRYPT_P_ENCRYPT inp [i] = hash [rot += inp [i]]
# define GCRYPT_P_DECRYPT rot += inp [i] = hashDec [inp [i]] - rot

static void gHash (const unsigned char * key, unsigned char out [256])
{
	unsigned char arr256 [256];

	unsigned char * hash = out;
	unsigned char * hash_p = arr256;
	unsigned char * swp;

	for (size_t i = 0; i < 256; i++)
		hash [i] = i;

	while (* key)
	{
		unsigned short c = * key * * key;

		for (unsigned short ci = 0; ci < c; ci++)
		{
			unsigned char d = 256 / * key;
			unsigned char m = 256 % * key;
			unsigned char hi = * key;

			d += 1;

			for (unsigned short mi = 0; mi < m; mi++)
				for (unsigned short di = 0; di < d; di++)
					hash_p [hi++] = hash [di * * key + mi];

			d -= 1;

			for (unsigned short mi = m; mi < * key; mi++)
				for (unsigned short di = 0; di < d; di++)
					hash_p [hi++] = hash [di * * key + mi];

			swp = hash;
			hash = hash_p;
			hash_p = swp;
		}

		key++;
	}

	if (hash != out)
		memcpy (out, hash, 256);
}

G_ACTION (gEncryptF)
{
	GCRYPT_F_PROCESS_BEGIN
	GCRYPT_F_PROCESS_WRITE (GCRYPT_P_ENCRYPT)
	GCRYPT_F_PROCESS_END
}

G_ACTION (gDecryptF)
{
	GCRYPT_F_PROCESS_BEGIN
	GCRYPT_F_PROCESS_DECHASH
	GCRYPT_F_PROCESS_WRITE (GCRYPT_P_DECRYPT)
	GCRYPT_F_PROCESS_END
}
