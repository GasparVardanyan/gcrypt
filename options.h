# ifndef GCRYPT_OPTIONS_H__
# define GCRYPT_OPTIONS_H__

# define OPT_HELP "--help"
# define OPT_VERSION "--version"
# define OPT_ENCRYPT "-e"
# define OPT_DECRYPT "-d"
# define FLG_STDIO "-S"
# define FLG_NORND "-R"

// # ifdef __cplusplus
// extern "C" {
// # endif // __cplusplus

extern _Noreturn void g_exit (const char * const);

// # ifdef __cplusplus
// }
// # endif // __cplusplus

static const char * const help =
	"Options:\n"
	"\t"OPT_HELP"\t\t\tcommand: display this help message and exit\n"
	"\t"OPT_VERSION"\t\tcommand: display the version and exit\n"
	"\t"OPT_ENCRYPT" key [ifile ofile]\taction: encrypt data\n"
	"\t"OPT_DECRYPT" key [ifile ofile]\taction: decrypt data\n"
	"\t"FLG_STDIO" \t\t\tflag: use the standart io (must become before action)\n"
	"\t"FLG_NORND" \t\t\tflag: don't use randomisation (for backwards compatibility)\n"
	"\t\t\t\t      randomisation generates different files every\n"
	"\t\t\t\t      time you encrypt same file with same key\n"
	"\nUsage examples:\n"
	"\tgcrypt "OPT_ENCRYPT" my_strong_key file_to_encrypt output_file\n"
	"\tgcrypt "OPT_DECRYPT" my_strong_key file_to_decrypt output_file\n"
	"\tcat file_to_encrypt | gcrypt "FLG_STDIO" "OPT_ENCRYPT" my_strong_key > output_file \n"
;

# define ERR_HELP_MSG "Help: gcrypt "OPT_HELP"\n"

static const char * const err_invalid_usage =
	"Error: invalid usage.\n"
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

# undef ERR_HELP_MSG

# endif // GCRYPT_OPTIONS_H__
