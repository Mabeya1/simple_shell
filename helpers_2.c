#include "shell.h"

void logical_ops(char *line, ssize_t *new_len);
ssize_t get_new_len(char *line);
void handle_line(char **line, ssize_t read);

/**
 * Modify the input command line by inserting spaces
 * around certain logical operators.
 * @param line  Pointer to the input command line
 * @param read  Number of characters read from input
 */
void handle_line(char **line, ssize_t read)
{
	char previous, current, next;
	char *old_line, *new_line;
	size_t c, d;
	ssize_t new_len;

	new_len = get_new_len(*line);
	if (new_len == read - 1)
		return;
	new_line = malloc(new_len + 1);
	if (!new_line)
		return;
	j = 0;
	old_line = *line;
	for (d = 0; old_line[c]; c++)
	{
		current = old_line[c];
		next = old_line[c + 1];
		if (c != 0)
		{
			previous = old_line[c - 1];
			if (current == ';')
			{
				if (next == ';' && previous != ' ' && previous != ';')
				{
					new_line[d++] = ' ';
					new_line[d++] = ';';
					continue;
				}
				else if (previous == ';' && next != ' ')
				{
					new_line[d++] = ';';
					new_line[d++] = ' ';
					continue;
				}
				if (previous != ' ')
					new_line[d++] = ' ';
				new_line[d++] = ';';
				if (next != ' ')
					new_line[d++] = ' ';
				continue;
			}
			else if (current == '&')
			{
				if (next == '&' && previous != ' ')
					new_line[d++] = ' ';
				else if (previous == '&' && next != ' ')
				{
					new_line[d++] = '&';
					new_line[d++] = ' ';
					continue;
				}
			}
			else if (current == '|')
			{
				if (next == '|' && previous != ' ')
					new_line[d++]  = ' ';
				else if (previous == '|' && next != ' ')
				{
					new_line[d++] = '|';
					new_line[d++] = ' ';
					continue;
				}
			}
		}
		else if (current == ';')
		{
			if (i != 0 && old_line[c - 1] != ' ')
				new_line[d++] = ' ';
			new_line[d++] = ';';
			if (next != ' ' && next != ';')
				new_line[d++] = ' ';
			continue;
		}
		new_line[d++] = old_line[c];
	}
	new_line[d] = '\0';

	free(*line);
	*line = new_line;
}

/**
 * Calculate the new length of the line after modifications.
 *
 * @param line  Input command line
 * @return      New length of the lin
 */

ssize_t get_new_len(char *line)
{
	size_t c;
	ssize_t new_len = 0;
	char current, next;

	for (c = 0; line[c]; c++)
	{
		current = line[c];
		next = line[c + 1];
		if (current == '#')
		{
			if (c == 0 || line[c - 1] == ' ')
			{
				line[c] = '\0';
				break;
			}
		}
		else if (c != 0)
		{
			if (current == ';')
			{
				if (next == ';' && line[c - 1] != ' ' && line[c - 1] != ';')
				{
					new_len += 2;
					continue;
				}
				else if (line[c - 1] == ';' && next != ' ')
				{
					new_len += 2;
					continue;
				}
				if (line[c - 1] != ' ')
					new_len++;
				if (next != ' ')
					new_len++;
			}
			else
				logical_ops(&line[c], &new_len);
		}
		else if (current == ';')
		{
			if (c != 0 && line[c - 1] != ' ')
				new_len++;
			if (next != ' ' && next != ';')
				new_len++;
		}
		new_len++;
	}
	return (new_len);
}
/**
 *Adjust the new length based on logical operators.
 *
 * @param line inter to the logical operator in the line
 * @param new_len   Pointer to the new length variable
 */
void logical_ops(char *line, ssize_t *new_len)
{
	char previous, current, next;

	previous = *(line - 1);
	current = *line;
	next = *(line + 1);
	/*Adjust the new length based on the logical operator*/
	if (current == '&')
	{
		if (next == '&' && previous != ' ')
			(*new_len)++;
		else if (previous == '&' && next != ' ')
			(*new_len)++;
	}
	else if (current == '|')
	{
		if (next == '|' && previous != ' ')
			(*new_len)++;
		else if (previous == '|' && next != ' ')
			(*new_len)++;
	}
}

