# include <stddef.h>
# include <stdio.h>
# include <string.h>

# include "gcrypt.h"
# include "header.h"
# include "options.h"
# include "readopts.h"

static G_ACTION (* action);

static const char * input_key;
static const char * input_ifile;
static const char * input_ofile;
static _Bool input_stdio;
static _Bool input_nornd;

int main (int argc, char * argv [])
{
	if (argc == 1)
	{
		puts (header);
		g_exit (NULL);
	}
	else ARG_ITER
	{
		if (CHECK_ARG (OPT_HELP, 0, 1))
		{
			puts (header), puts (help);
			g_exit (NULL);
		}
		else if (CHECK_ARG (OPT_VERSION, 0, 1))
		{
			puts ("GCrypt " GCRYPT_VERSION);
			g_exit (NULL);
		}
# define SET_ACTION(a)						\
{											\
	if (action != NULL)						\
		g_exit (err_invalid_usage);			\
											\
	GET_STR (input_key);					\
											\
	if (input_stdio == 0)					\
	{										\
		GET_STR (input_ifile);				\
		GET_STR (input_ofile);				\
	}										\
											\
	action = a;								\
}											\
// SET_ACTION
		else if (
			CHECK_ARG (OPT_ENCRYPT, 3, 0) && input_stdio == 0 ||
			CHECK_ARG (OPT_ENCRYPT, 1, 0) && input_stdio == 1
		){
			SET_ACTION (gEncryptF);
		}
		else if (
			CHECK_ARG (OPT_DECRYPT, 3, 0) && input_stdio == 0 ||
			CHECK_ARG (OPT_DECRYPT, 1, 0) && input_stdio == 1
		){
			SET_ACTION (gDecryptF);
		}
# undef SET_ACTION
		/* else if (CHECK_ARG (OPT_BS, 1, 0)) */
		/* { */
		/*     GET_ULL (input_buffsize, g_exit (err_invalid_usage)); */
		/* } */
		else
		{
			FLAG_ITER
			{
				if (CHECK_FLAG (FLG_STDIO))
				{
					if (action != NULL)
						g_exit (err_invalid_usage);

					input_stdio = 1;
				}
				else if (CHECK_FLAG (FLG_NORND))
				{
					input_nornd = 1;
				}
				else
				{
					g_exit (err_invalid_usage);
				}
			}
		}
	}

	if (action)
	{
		if (input_stdio == 0 && strcmp (input_ifile, input_ofile) == 0)
			g_exit (err_same_iofile);
		else if (* input_key == 0) g_exit (err_empty_key);
		/* else if (input_buffsize == 0) g_exit (err_invalid_usage); */
		/* (* action) (input_key, input_ifile, input_ofile, input_buffsize, input_nornd); */
		(* action) (input_key, input_ifile, input_ofile, input_nornd);
	}
	else
		g_exit (err_invalid_usage);
}
