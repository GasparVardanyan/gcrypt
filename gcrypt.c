# include "gcrypt.h"

# define GCRYPT_F_PROCESS_BEGIN															\
	FILE																				\
		* istream = fopen (ifile, "rb"),												\
		* ostream = fopen (ofile, "wb")													\
	;																					\
	if (!istream)																		\
	{																					\
		fputs ("Error opening ifile. Aborting.\n", stderr);								\
		return;																			\
	}																					\
	if (!ostream)																		\
	{																					\
		fputs ("Error opening ofile. Aborting.\n", stderr);								\
		return;																			\
	}																					\
																						\
	ubyte																				\
		* inp,																			\
		hash [256]																		\
	;																					\
																						\
	if ((inp = malloc (b)) == NULL)														\
	{																					\
		fputs ("Error allocating buffer. Aborting.\n", stderr);							\
		goto close_streams;																\
	}																					\
																						\
	gHash ((const ubyte * const) key, hash);											\
																						\
	register size_t bufsize, i;															\
	register ubyte rot = 0;																\
// GCRYPT_F_PROCESS_BEGIN

# define GCRYPT_F_PROCESS_END															\
	if (ferror (istream) || !feof (istream))											\
		fputs ("Error reading ifile. Aborting.\n", stderr);								\
	if (ferror (ostream))																\
		fputs ("Error writing ofile. Aborting.\n", stderr);								\
																						\
	free (inp);																			\
close_streams:																			\
	fclose (istream);																	\
	fclose (ostream);																	\
// GCRYPT_F_PROCESS_END

# define GCRYPT_F_PROCESS_WRITE(pattern)												\
	while ((bufsize = fread (inp, 1, b, istream)))										\
	{																					\
		for (i = 0; i < bufsize; i++)													\
			pattern;																	\
		fwrite (inp, 1, bufsize, ostream);												\
	}																					\
// GCRYPT_F_PROCESS_WRITE

# define GCRYPT_F_PROCESS_DECHASH														\
	ubyte hashDec [256];																\
	for (i = 0; i < 256; i++)															\
		hashDec [hash [i]] = i;															\
// GCRYPT_F_PROCESS_DECHASH

# define GCRYPT_P_ENCRYPT inp [i] = hash [rot += inp [i]]
# define GCRYPT_P_DECRYPT rot += inp [i] = hashDec [inp [i]] - rot

typedef unsigned char ubyte;

static void gHash (const ubyte * key, ubyte out [256])
{
	ubyte arr256 [256];

	ubyte * hash = out;
	ubyte * hash_p = arr256;
	ubyte * swp;

	for (size_t i = 0; i < 256; i++)
		hash [i] = i;

	while (* key)
	{
		unsigned short c = * key * * key;

		for (unsigned short ci = 0; ci < c; ci++)
		{
			ubyte d = 256 / * key;
			ubyte m = 256 % * key;
			ubyte hi = * key;

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

void gEncryptF (const char * const key, const char * const ifile, const char * const ofile, const size_t b)
{
	GCRYPT_F_PROCESS_BEGIN
	GCRYPT_F_PROCESS_WRITE (GCRYPT_P_ENCRYPT)
	GCRYPT_F_PROCESS_END
}

void gDecryptF (const char * const key, const char * const ifile, const char * const ofile, const size_t b)
{
	GCRYPT_F_PROCESS_BEGIN
	GCRYPT_F_PROCESS_DECHASH
	GCRYPT_F_PROCESS_WRITE (GCRYPT_P_DECRYPT)
	GCRYPT_F_PROCESS_END
}
