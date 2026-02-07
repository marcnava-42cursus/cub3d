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
#include "structs.h"

int	count_file_lines(const char *filename)
{
	int		fd;
	int		count;
	char	*line;

	count = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
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

static int	read_lines_from_file(int fd, char **lines, int line_count)
{
	int		count;
	char	*line;

	count = 0;
	line = get_next_line(fd);
	while (line && count < line_count)
	{
		lines[count] = line;
		count++;
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	if (count != line_count)
	{
		free_partial_lines(lines, count);
		return (0);
	}
	lines[count] = NULL;
	return (1);
}

char	**read_file_lines(const char *filename, int *line_count)
{
	int		fd;
	char	**lines;

	*line_count = count_file_lines(filename);
	if (*line_count <= 0)
		return (NULL);
	lines = allocate_lines(*line_count);
	if (!lines)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		free(lines);
		return (NULL);
	}
	if (!read_lines_from_file(fd, lines, *line_count))
	{
		close(fd);
		return (NULL);
	}
	close(fd);
	return (lines);
}
