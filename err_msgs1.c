#include "shell.h"

char *error_env(char **args);
char *error_2_syntax(char **args);
char *error_2_cd(char **args);
char *error_2_exit(char **args);
char *error_1(char **args);
/**
 * Generate an error message for environment-related issues.
 *
 * @param args  Command arguments
 * @return      Error message string on success, NULL on failure
 */
char *error_env(char **args)
{
	char *hist_str, *error;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);
	/*Calculate the length of the error message*/
	args--;
	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 45;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": Unable to add/remove from environment\n");

	free(hist_str);
	return (error);
}

/**
 * Generate an error message for an alias not found.
 *
 * @param args  Command arguments
 * @return     Error message string on success, NULL on failure
 */
char *error_1(char **args)
{
	int len;
	char *error;
	/*Calculate the length of the error message*/
	len = _strlen(name) + _strlen(args[0]) + 13;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
		return (NULL);

	_strcpy(error, "alias: ");
	_strcat(error, args[0]);
	_strcat(error, " not found\n");

	return (error);
}

/**
 * Generate an error message for an illegal exit number.
 *
 * @param args  Command arguments
 * @return      Error message string on success, NULL on failure
 */
char *error_2_exit(char **args)
{
	char *hist_str, *error;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 27;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": exit: Illegal number: ");
	_strcat(error, args[0]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * Generate an error message for an illegal cd command.
 *
 * @param args  Command arguments
 * @return Error message string on success, NULL on failure
 */
char *error_2_cd(char **args)
{
	char *hist_str, *error;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	if (args[0][0] == '-')
		args[0][2] = '\0';
	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 24;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	if (args[0][0] == '-')
		_strcat(error, ": cd: Illegal option ");
	else
		_strcat(error, ": cd: can't cd to ");
	_strcat(error, args[0]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * Generate an error message for syntax-related issues.
 *
 * @param args  Command arguments
 * @return      Error message string on success, NULL on failure
 */
char *error_2_syntax(char **args)
{
	char *hist_str, *error;
	int len;
	/*Convert history counter to string*/
	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(hist_str)+ _strlen(name) + _strlen(args[0]) + 33;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Syntax error: \"");
	_strcat(error, args[0]);
	_strcat(error, "\" unexpected\n");
	/*Free allocated memory and return the error message*/
	free(hist_str);
	return (error);
}

