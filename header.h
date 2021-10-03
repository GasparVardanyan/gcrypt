# ifndef GCRYPT_HEADER_H__
# define GCRYPT_HEADER_H__

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

# endif // GCRYPT_HEADER_H__
