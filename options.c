# include <stdio.h>
# include <stdlib.h>

# include "options.h"

void g_exit (const char * const message)
{
	if (message)
	{
		fputs (message, stderr);
		exit (EXIT_FAILURE);
	}
	else exit (0);
}
