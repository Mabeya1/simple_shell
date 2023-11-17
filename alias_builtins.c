#include "shell.h"

int shellby_alias(argschar __attribute__((__unused__)) **front, char **);
void print_alias(alias_t *alias);
void set_alias(char *value, char *var_name);

/**
 * Execute alias-related commands.
 * If no arguments provided, print all aliases.
 * If arguments are provided, either print the alias or set a new alias.
 *
 * @param args   Array of command arguments
 * @param front  Unused double pointer to the front of the command
 * @return       0 on success, 1 on error
 */
int shellby_alias(char __attribute__((__unused__)) **front, char **args)
{
	alias_t *temp = aliases;
	int b, ret = 0;
	char *value;

	if (!args[0])
	{
		while (temp)
		{
			print_alias(temp);
			temp = temp->next;
		}
		return (ret);
	}
	for (b = 0; args[b]; b++)
	{
		temp = aliases;
		value = _strchr(args[b], '=');
		if (!value)
		{
			while (temp)
			{
				if (_strcmp(args[b], temp->name) == 0)
				{
					print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				ret = create_error(args + b, 1);
		}
		else
			set_alias(args[b], value);
	}
	return (ret);
}

/**
 * Set a new alias or update the value of an existing alias.
 *
 * @param var_name  Name of the alias
 * @param value     Value associated with the alias
 */
void set_alias(char *value, char *var_name)
{
	alias_t *temp = aliases;
	int len, h, i;
	char *new_value;

	*value = '\0';
	value++;
	len = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return;
	for (h = 0, i = 0; value[h]; h++)
	{
		if (value[h] != '\'' && value[h] != '"')
			new_value[i++] = value[h];
	}
	new_value[i] = '\0';
	while (temp)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		add_alias_end(&aliases, var_name, new_value);
}

/**
 * Print the alias in the format 'name='value''.
 *
 * @param alias  Pointer to the alias structure
 */
void print_alias(alias_t *alias)
{
	char *alias_string;
	int len = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (len + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, len);
	free(alias_string);
}
/**
 * eplace alias names in the command arguments with their values.
 *
 * @param args  Array of command arguments
 * @return  Updated array of command arguments with aliases replaced
 */
char **replace_aliases(char **args)
{
	alias_t *temp;
	int b;
	char *new_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);
	for (b = 0; args[b]; b++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(args[b], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (_strlen(temp->value) + 1));
				if (!new_value)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(new_value, temp->value);
				free(args[b]);
				args[b] = new_value;
				b--;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);
}

