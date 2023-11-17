#include "shell.h"

int num_len(int num);
char *_itoa(int num);
int create_error(int err, char **args);

/**
 * num_len - Calculates the number of digits in an integer.
 * @num: The integer to calculate the length of.
 *
 * Return: The length of the integer.
 *
 */
int num_len(int num)
{
	unsigned int num1;
	int len = 1;
	/* Check if the number is negative */
	if (num < 0)
	{
		len++;
		num1 = num * -1;
	}
	else
	{
		num1 = num;
	}
	while (num1 > 9)
	{
		len++;
		num1 /= 10;
	}

	return (len);
}

/**
 *_itoa - Converts an integer to a string.
 * @num: The integer to convert.
 *
 * Return: A dynamically allocated string representing the integer.
 */
char *_itoa(int num)
{
	int len = num_len(num);
	char *buffer;
	unsigned int num1;
	/*Allocate memory for the string representation of the integer*/
	buffer = malloc(sizeof(char) * (len + 1));
	if (!buffer)
		return (NULL);

	buffer[len] = '\0';

	if (num < 0)
	{
		num1 = num * -1;
		buffer[0] = '-';
	}
	else
	{
		num1 = num;
	}

	len--;
	do {
		buffer[len] = (num1 % 10) + '0';
		num1 /= 10;
		len--;
	} while (num1 > 0);

	return (buffer);
}


/**
 * create_error - Creates an error message based on the error code.
 * @args: Array of strings representing command arguments.
 * @err: The error code.
 *
 * Return: The error code.
 */
int create_error(char **args, int err)
{
	char *error;
	/*Generate error message based on the error code*/
	switch (err)
	{
	case -1:
		error = error_env(args);
		break;
	case 1:
		error = error_1(args);
		break;
	case 2:
		if (*(args[0]) == 'e')
			error = error_2_exit(++args);
		else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
			error = error_2_syntax(args);
		else
			error = error_2_cd(args);
		break;
	case 126:
		error = error_126(args);
		break;
	case 127:
		error = error_127(args);
		break;
	}
	write(STDERR_FILENO, error, _strlen(error));

	if (error)
		free(error);
	return (err);

}

