#include "shell.h"

char **_getenv(const char *var);
void free_env(void);
char **_copyenv(void);

/**
 * Create a copy of the current environment variables.
 *
 * @return A new array containing a copy of the environment
 * variables.
 * NULL on failure.
 */
char **_copyenv(void)
{
	int index;
	size_t size;
	char **new_environ;


	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * (size + 1));
	if (!new_environ)
		return (NULL);
	/*Copy each environment variable to the new array*/
	for (index = 0; environ[index]; index++)
	{
		new_environ[index] = malloc(_strlen(environ[index]) + 1);
		/*Free allocated memory and return NULL on failure*/
		if (!new_environ[index])
		{
			for (index--; index >= 0; index--)
				free(new_environ[index]);
			free(new_environ);
			return (NULL);
		}
		_strcpy(new_environ[index], environ[index]);
	}
	new_environ[index] = NULL;

	return (new_environ);
}

/**
 * Free the memory allocated for the environment variables.
 */
void free_env(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * Get the address of the environment variable with the
 * specified name.
 * @param var The name of the environment variable to find.
 * @return    The address of the environment variable.
 *            NULL if the variable is not found.
 *
 */
char **_getenv(const char *var)
{
	int index, len;

	len = _strlen(var);
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], len) == 0)
			return (&environ[index]);
	}

	return (NULL);
}

