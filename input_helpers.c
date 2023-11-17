#include "shell.h"

char *get_args(char *line, int *exe_ret);
int call_args(char **front, char **args, int *exe_ret);
int run_args(char **front, char **args, int *exe_ret);
int check_args(char **args);
int handle_args(int *exe_ret);


/**
 * Get input arguments from the user and perform variable
 *replacement and line handling.
 * @param line Pointer to the input command line
 * @param exe_ret Pointer to the execution return value
 * @return Modified input command line on success, NULL on
 * failure
 */
char *get_args(char *line, int *exe_ret)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	handle_line(&line, read);

	return (line);
}

/**
 * Call the appropriate function based on the logical
 * operators encountered in the command.
 * @param front     Pointer to the front of the command
 * @param args      Pointer to the command arguments
 * @param exe_ret   Pointer to the execution return value
 * @return          Execution return value
 */
int call_args(char **front, char **args, int *exe_ret)
{
	int index, ret;
	/*If no arguments, return the current execution return value*/
	if (!args[0])
		return (*exe_ret);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}
	args = replace_aliases(args);
	ret = run_args(args, front, exe_ret);
	return (ret);
}

/**
 * Run the given command or built-in function and
 * update the execution return value.
 * @param front     Pointer to the front of the command
 * @param args      Pointer to the command arguments
 * @param exe_ret   Pointer to the execution return value
 * @return          Execution return value
 */
int run_args(char **front, char **args, int *exe_ret)
{
	int ret, c;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*exe_ret = ret;
	}
	else
	{
		*exe_ret = execute(args, front);
		ret = *exe_ret;
	}

	hist++;

	for (i = 0; args[c]; c++)
		free(args[c]);

	return (ret);
}

/**
 * Check the command arguments for syntax errors
 * involving logical operators.
 * @param args      Pointer to the command arguments
 * @return          0 on success, non-zero on syntax error
 */
int handle_args(int *exe_ret)
{
	int ret = 0, index;
	char **args, *line = NULL, **front;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (ret);
	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = call_args(args, front, exe_ret);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		ret = call_args(args, front, exe_ret);

	free(front);
	return (ret);
}

/**
 * Handle the execution of the command line, including
 * logical operators and error checking.
 * @param exe_ret   Pointer to the execution return value
 * @return          Execution return value
 */
int check_args(char **args)
{
	size_t c;
	char *cur, *nex;

	for (c = 0; args[c]; c++)
	{
		cur = args[c];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (c == 0 || cur[1] == ';')
				return (create_error(&args[c], 2));
			nex = args[c + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (create_error(&args[c + 1], 2));
		}
	}
	return (0);
}

