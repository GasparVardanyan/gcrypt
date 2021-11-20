# ifndef GCRYPT_READOPTS_H__
# define GCRYPT_READOPTS_H__

# include <errno.h>
# include <stdlib.h>
# include <string.h>

# define CHECK_ARG(a,n,o) (strcmp (a, argv [i]) == 0 && (o ? argc == 2 + n : (i + n < argc ? ++i : 0)))
# define ARG_ITER for (int i = 1; i < argc;)
# define GET_STR(x) (x = argv [i++])
# define GET_ULL(x, error_callback)				\
{												\
	char * buff, * ep = NULL;					\
	GET_STR (buff);								\
	errno = 0;									\
	x = strtoull (buff, & ep, 10);				\
	if (errno || ep != strrchr (buff, 0))		\
		error_callback;							\
}												\
// GET_ULL

# define FLAG_ITER if (* argv [i] != '-') g_exit (err_invalid_usage); for (const char * flags = argv [i++] + 1; * flags; flags++)
# define CHECK_FLAG(f) (* flags == * (f + 1))

# endif // GCRYPT_READOPTS_H__
