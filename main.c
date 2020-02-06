# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <errno.h>
# include "gcrypt.h"

static const char * const header =
# ifdef __linux__
	"    \033[37m                                  _\n"
	"                                     / /\n"
	"       \033[1m____________\033[0m\033[37m      _   _      / / \n"
	"      \033[1m/ ____/ ____/____\033[0m\033[37m/ / / /\033[1m___  / /____________________\033[0m\033[37m\n"
	"     \033[1m/ / __/ /   / ___/ / / / __ \\/ _______Gaspar's cryptographic\033[0m\033[37m\n"
	"    \033[1m/ /_/ / /___/ /  / /_/ / /_/ / /_      algorithm.\033[0m\033[37m\n"
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
	"    / /_/ / /___/ /  / /_/ / /_/ / /_      algorithm.\n"
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

# define OPT_HELP "--help"
# define OPT_VERSION "--version"
# define OPT_ENCRYPT "-e"
# define OPT_DECRYPT "-d"
# define OPT_BS "-b"

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

static void (* action) ();

static const char * input_key;
static const char * input_ifile;
static const char * input_ofile;
static size_t input_b = 512u;

# define CHECK_ARG_PARAMS(n) (i + n >= argc ? g_exit (err_invalid_args), 0 : ++i)
# define CHECK_ARG(a,n) strcmp (a, argv [i]) == 0 && CHECK_ARG_PARAMS (n)
# define GET_ARG argv [i++]
# define SET_ACTION(a)					\
	input_key = GET_ARG;				\
	input_ifile = GET_ARG;				\
	input_ofile = GET_ARG;				\
	if (!action) action = a;			\
	else g_exit (err_invalid_args);		\
// SET_ACTION

int main (int argc, char * argv [])
{
	if (argc == 1)
	{
		puts (header);
		return 0;
	}
	else if (argc == 2)
	{
		if (strcmp (argv [1], OPT_HELP) == 0)
		{
			puts (header), puts (help);
			return 0;
		}
		else if (strcmp (argv [1], OPT_VERSION) == 0)
		{
			puts ("GCrypt " GCRYPT_VERSION);
			return 0;
		}
		else g_exit (err_invalid_args);
	}
	else for (int i = 1; i < argc;)
	{
		if (CHECK_ARG (OPT_ENCRYPT, 3))
		{
			SET_ACTION (gEncryptF);
			continue;
		}
		else if (CHECK_ARG (OPT_DECRYPT, 3))
		{
			SET_ACTION (gDecryptF);
			continue;
		}
		else if (CHECK_ARG (OPT_BS, 1))
		{
			char * buff = GET_ARG, * ep = NULL;
			errno = 0;
			input_b = strtoull (buff, & ep, 10);
			if (errno || ep != strrchr (buff, 0) || input_b == 0)
				g_exit (err_invalid_args);
			continue;
		}
		g_exit (err_invalid_args);
	}

	if (action)
	{
		if (strcmp (input_ifile, input_ofile) == 0) g_exit (err_same_iofile);
		else if (!* input_key) g_exit (err_empty_key);
		(* action) (input_key, input_ifile, input_ofile, input_b);
	}
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
