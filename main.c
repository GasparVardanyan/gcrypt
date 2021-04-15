# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <errno.h>
# include "gcrypt.h"

# define OPT_HELP "--help"
# define OPT_VERSION "--version"
# define OPT_ENCRYPT "-e"
# define OPT_DECRYPT "-d"
# define OPT_BS "-b"

# define CHECK_ARG(a,n,o) (strcmp (a, argv [i]) == 0 && (o ? argc == 2 + n : (i + n < argc ? ++i : 0)))
# define ARG_ITER for (int i = 1; i < argc;)
# define GET_STR(x) (x = argv [i++])
# define GET_ULL(x)								\
{												\
	char * buff, * ep = NULL;					\
	GET_STR (buff);								\
	errno = 0;									\
	x = strtoull (buff, & ep, 10);				\
	if (errno || ep != strrchr (buff, 0))		\
		g_exit (err_invalid_args);				\
}												\
// GET_ULL

// # define FLAG_ITER if (* argv [i] != '-') g_exit (err_invalid_args); for (const char * flags = argv [i++] + 1; * flags; flags++)
// # define CHECK_FLAG(f) (* flags == f)
// yagni...

static const char * const header =
# ifdef __linux__
	"    \033[37m                                  _\n"
	"                                     / /\n"
	"       \033[1m____________\033[0m\033[37m      _   _      / / \n"
	"      \033[1m/ ____/ ____/____\033[0m\033[37m/ / / /\033[1m___  / /____________________\033[0m\033[37m\n"
	"     \033[1m/ / __/ /   / ___/ / / / __ \\/ _______Gaspar's cryptographic\033[0m\033[37m\n"
	"    \033[1m/ /_/ / /___/ /  / /_/ / /_/ / /_      tool.\033[0m\033[37m\n"
	"    \033[1m\\____/\\____/ /   \\__, / .___/\\__/\033[0m\033[37m\n"
	"              / /      \033[1m/ / / _____________________________________________\033[0m\033[37m\n"
	"             /_/   ___/ / /    \033[1mhttps://github.com/GasparVardanyan/gcrypt\033[0m\033[37m\n"
	"                  /____/_/               _____   \033[1m______\033[0m\033[37m___\n"
	"                                         ___  \033[1m\\ / /__ '\033[0m\033[37m___\n"
	"             \033[1m51mP13, 53cUR3,\033[0m\033[37m                 \033[1m\\ V / |_ \\\033[0m\033[37m\n"
	"             \033[1mp3rf0rM4nc3\033[0m\033[37m                      \033[1m\\ / |  _/\033[0m\033[37m\n"
	"                                             / /  | |\033[39m\n"
# else
	"                                      _\n"
	"                                     / /\n"
	"       ____________      _   _      / / \n"
	"      / ____/ ____/____/ / / /___  / /____________________\n"
	"     / / __/ /   / ___/ / / / __ \\/ _______Gaspar's cryptographic\n"
	"    / /_/ / /___/ /  / /_/ / /_/ / /_      tool.\n"
	"    \\____/\\____/ /   \\__, / .___/\\__/\n"
	"              / /      / / / _____________________________________________\n"
	"             /_/   ___/ / /    https://github.com/GasparVardanyan/gcrypt\n"
	"                  /____/_/               _____   _________\n"
	"                                         ___  \\ / /__ '___\n"
	"             51mP13, 53cUR3,                 \\ V / |_ \\\n"
	"             p3rf0rM4nc3                      \\ / |  _/\n"
	"                                             / /  | |\n"
# endif // __linux__
;

static const char * const help =
	"Options:\n"
	"\t"OPT_HELP"\t\t\tcommand: display this help message and exit\n"
	"\t"OPT_VERSION"\t\tcommand: display the version and exit\n"
	"\t"OPT_ENCRYPT" key ifile ofile\taction: encrypt data\n"
	"\t"OPT_DECRYPT" key ifile ofile\taction: decrypt data\n"
	"\t"OPT_BS" count\t\toption: read and write up to count bytes at a time (default: 512)\n"
	"\nUsage:\n"
	"\t gcrypt ([action [options]] | [command])\n"
	"\nExamples:\n"
	"\tgcrypt "OPT_ENCRYPT" my_strong_key file_to_encrypt output_file\n"
	"\tgcrypt "OPT_DECRYPT" my_strong_key file_to_decrypt output_file\n"
	"\tgcrypt "OPT_ENCRYPT" my_strong_key file_to_encrypt output_file "OPT_BS" 1024\n"
	"\tgcrypt "OPT_BS" 666 "OPT_DECRYPT" my_strong_key file_to_decrypt output_file\n"
;

# define ERR_HELP_MSG "Help: gcrypt "OPT_HELP"\n"

static const char * const err_invalid_args =
	"Error: invalid arguments list.\n"
	ERR_HELP_MSG
;

static const char * const err_empty_key =
	"Error: key can't be empty.\n"
	ERR_HELP_MSG
;

static const char * const err_same_iofile =
	"Error: input and output files can't be the same.\n"
	ERR_HELP_MSG
;

static _Noreturn void g_exit (const char * const);

static G_ACTION (* action);

static const char * input_key;
static const char * input_ifile;
static const char * input_ofile;
static size_t input_b = 512u;

# define SET_ACTION(a)					\
{										\
	GET_STR (input_key);				\
	GET_STR (input_ifile);				\
	GET_STR (input_ofile);				\
	if (!action) action = a;			\
	else g_exit (err_invalid_args);		\
}										\
// SET_ACTION
# define CALL_ACTION(action)														\
{																					\
	if (input_b == 0) g_exit (err_invalid_args);									\
	else if (strcmp (input_ifile, input_ofile) == 0) g_exit (err_same_iofile);		\
	else if (!* input_key) g_exit (err_empty_key);									\
	(* action) (input_key, input_ifile, input_ofile, input_b);						\
}																					\
// CALL_ACTION

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
		else if (CHECK_ARG (OPT_ENCRYPT, 3, 0))
		{
			SET_ACTION (gEncryptF);
		}
		else if (CHECK_ARG (OPT_DECRYPT, 3, 0))
		{
			SET_ACTION (gDecryptF);
		}
		else if (CHECK_ARG (OPT_BS, 1, 0))
		{
			GET_ULL (input_b);
		}
		else
		{
			g_exit (err_invalid_args);
		}
	}

	if (action) CALL_ACTION (action);
//	else g_exit (err_invalid_args);
}

void g_exit (const char * const message)
{
	if (message)
	{
		fputs (message, stderr);
		exit (EXIT_FAILURE);
	}
	else exit (0);
}
