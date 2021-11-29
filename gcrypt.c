# include <stddef.h>
# include <stdio.h>
# include <string.h>

# ifndef __linux__ // TODO: find a portable way to check if BSD env or no
#     include <stdlib.h>
# else
#     include <bsd/stdlib.h>
# endif // __linux__

# include "gcrypt.h"

# define GCRYPT_F_PROCESS_BEGIN											\
	FILE																\
		* istream = ifile ? fopen (ifile, "rb") : stdin,				\
		* ostream = ofile ? fopen (ofile, "wb") : stdout;				\
																		\
	if (!istream)														\
	{																	\
		fputs ("Error opening ifile. Aborting.\n", stderr);				\
		return;															\
	}																	\
	if (!ostream)														\
	{																	\
		fputs ("Error opening ofile. Aborting.\n", stderr);				\
		return;															\
	}																	\
																		\
	unsigned char hash [256];											\
																		\
	gHash ((const unsigned char * const) key, hash);					\
																		\
	register unsigned char rot = 0;										\
// GCRYPT_F_PROCESS_BEGIN

# define GCRYPT_F_PROCESS_END											\
	if (ferror (istream) || !feof (istream))							\
		fputs ("Error reading ifile. Aborting.\n", stderr);				\
	if (ferror (ostream))												\
		fputs ("Error writing ofile. Aborting.\n", stderr);				\
																		\
	if (ifile) fclose (istream);										\
	if (ofile) fclose (ostream);										\
// GCRYPT_F_PROCESS_END

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

	register int b;

	if (nornd == 0)
	{
		register int rnexti = 0;
		unsigned char rnext = 0, rr;

		while ((b = fgetc (istream)) != EOF)
		{
			if (rnexti == rnext)
			{
				rnexti = 0;
				rnext = arc4random () % 256;
				rr = arc4random () % 256;
				fputc (hash [rot += rnext], ostream);
				fputc (hash [rot += rr], ostream);
			}
			else
				rnexti++;

			fputc (hash [rot += b], ostream);
		}
	}
	else
	{
		while ((b = fgetc (istream)) != EOF)
			fputc (hash [rot += b], ostream);
	}

	GCRYPT_F_PROCESS_END
}

G_ACTION (gDecryptF)
{
	GCRYPT_F_PROCESS_BEGIN

	unsigned char hashDec [256];
	for (int i = 0; i < 256; i++)
		hashDec [hash [i]] = i;

	register int b;

	if (nornd == 0)
	{
		register int rnexti = 0;
		unsigned char rnext = 0;

		while ((b = fgetc (istream)) != EOF)
		{
			if (rnexti == rnext)
			{
				rnexti = -1;
				rot += rnext = hashDec [b] - rot;
				b = fgetc (istream);
				rot += hashDec [b] - rot; // rr
			}
			else
			{
				rnexti++;
				rot += b = hashDec [b] - rot;
				fputc (b, ostream);
			}
		}
	}
	else
	{
		while ((b = fgetc (istream)) != EOF)
			fputc ((rot += b = hashDec [b] - rot, b), ostream);
	}

	GCRYPT_F_PROCESS_END
}
