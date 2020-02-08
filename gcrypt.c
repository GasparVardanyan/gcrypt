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
	gHash ((const ubyte * const) key, hash); 											\
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

# define GCRYPT_F_PROCESS_DECHASH																\
	ubyte hashDec [256];																\
	for (i = 0; i < 256; i++)															\
		hashDec [hash [i]] = i;															\
// GCRYPT_F_PROCESS_DECHASH

# define GCRYPT_P_ENCRYPT inp [i] = hash [rot += inp [i]]
# define GCRYPT_P_DECRYPT rot += inp [i] = hashDec [inp [i]] - rot

typedef unsigned char ubyte;

static void gHash (const ubyte * const key, ubyte hash [256])
{
	size_t i, j;
	ubyte h1 [256], s, sm, b, bi, si, wi;
	unsigned short r, ic;
	for (i = 0; i < 256; i++) h1 [i] = i;
	for (i = 0; key [i] != 0; i++)
	{
		b = key [i];
		ic = b * b;
		for (r = 0; r < ic; r++)
		{
			s = 256 / b;
			sm = 256 % b;
			if (sm != 0) s++;
			wi = b;
			for (bi = 0; bi < b; bi++)
			{
				if (bi == sm) s--;
				for (si = 0; si < s; si++)
					hash [wi++] = h1 [(ubyte) (b * si + bi)];
			}
			for (j = 0; j < 256; j++) h1 [j] = hash [j];
		}
	}
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
