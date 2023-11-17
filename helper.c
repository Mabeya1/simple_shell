#include "shell.h"

void free_args(char **args, char **front);
char *get_env_value(int , lenchar *beginning);
char *get_pid(void);
void variable_replacement(int *exe_ret, char **args);

/**
 * free_args - Frees the memory allocated for arguments.
 * @args: Array of strings representing command arguments.
 * @front: Address of the front of the args array.
 *
 * This function frees the memory allocated for each
 * argument in the args array
 * and then frees the args array itself.
 */
void free_args(char **args, char **front)
{
	size_t u;

	for (u = 0; args[i] || args[u + 1]; i++)
		free(args[u]);

	free(front);
}

/**
 * get_pid - Gets the process ID (PID) of the shell.
 *
 * Return: A dynamically allocated string containing
 * the shell's PID.
 */
char *get_pid(void)
{
	size_t u = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[u] != ' ')
		u++;
	buffer[u] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - Gets the value of an environment variable.
 * @beginning: Pointer to the beginning of the variable name.
 * @len: Length of the variable name.
 * Return: A dynamically allocated string containing the value
 * of the variable.
 */
char *get_env_value(char *beginning, int len)
{
	char *replacement = NULL, *var, *temp;
	char **var_addr;

	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	var_addr = _getenv(var);
	free(var);
	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(_strlen(temp) + 1);
		if (replacement)
			_strcpy(replacement, temp);
	}

	return (replacement);
}

/**
 * variable_replacement - Replaces variables in a line
 * with their values.
 * @line: Pointer to the string containing the line.
 * @exe_ret: Pointer to the exit status of the last
 * executed command.
 * This function replaces variables in a line with their
 * corresponding values.
 */
void variable_replacement(char **line, int *exe_ret)
{
	int h, i = 0, len;
	char *replacement = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (h = 0; old_line[h]; h++)
	{
		if (old_line[h] == '$' && old_line[h + 1] &&
				old_line[h + 1] != ' ')
		{
			if (old_line[h + 1] == '$')
			{
				replacement = get_pid();
				i= h + 2;
			}
			else if (old_line[h + 1] == '?')
			{
				replacement = _itoa(*exe_ret);
				i = h + 2;
			}
			else if (old_line[h + 1])
			{

				for (i = h + 1; old_line[i] &&
						old_line[i] != '$' &&
						old_line[i] != ' '; i++)
					;
				len = i - (h + 1);
				replacement = get_env_value(&old_line[h + 1], len);
			}
			new_line = malloc(h + _strlen(replacement)
					  + _strlen(&old_line[i]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, h);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[i]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			h = -1;
		}
	}
}

