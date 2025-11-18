/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 20:29:03 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	count_file_lines(const char *filename)
{
	FILE	*file;
	int		count;
	char	buffer[MAX_LINE_LEN];

	count = 0;
	file = fopen(filename, "r");
	if (!file)
		return (-1);
	while (fgets(buffer, MAX_LINE_LEN, file))
		count++;
	fclose(file);
	return (count);
}

static char	**allocate_lines(int line_count)
{
	char	**lines;

	lines = malloc(sizeof(char *) * (line_count + 1));
	return (lines);
}

static void	free_partial_lines(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

static int	read_lines_from_file(FILE *file, char **lines, int line_count)
{
	char	buffer[MAX_LINE_LEN];
	int		count;

	count = 0;
	while (fgets(buffer, MAX_LINE_LEN, file) && count < line_count)
	{
		lines[count] = malloc(ft_strlen(buffer) + 1);
		if (!lines[count])
		{
			free_partial_lines(lines, count);
			return (0);
		}
		ft_strlcpy(lines[count], buffer, ft_strlen(buffer) + 1);
		count++;
	}
	lines[count] = NULL;
	return (1);
}

char	**read_file_lines(const char *filename, int *line_count)
{
	FILE	*file;
	char	**lines;

	*line_count = count_file_lines(filename);
	if (*line_count <= 0)
		return (NULL);
	lines = allocate_lines(*line_count);
	if (!lines)
		return (NULL);
	file = fopen(filename, "r");
	if (!file)
	{
		free(lines);
		return (NULL);
	}
	if (!read_lines_from_file(file, lines, *line_count))
	{
		fclose(file);
		return (NULL);
	}
	fclose(file);
	return (lines);
}
