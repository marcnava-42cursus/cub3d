/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_extraction.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:50:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/03 19:04:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*find_path_start(char *trimmed, const char *identifier)
{
	char	*path_start;

	path_start = trimmed + ft_strlen(identifier);
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	return (path_start);
}

static char	*find_path_end(char *path_start)
{
	char	*path_end;

	path_end = path_start + ft_strlen(path_start) - 1;
	while (path_end > path_start && (*path_end == ' '
			|| *path_end == '\t' || *path_end == '\n'))
		path_end--;
	return (path_end);
}

static char	*allocate_and_copy(char *path_start, char *path_end)
{
	char	*path;
	int		len;
	int		i;

	len = path_end - path_start + 1;
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	i = 0;
	while (i < len)
	{
		path[i] = path_start[i];
		i++;
	}
	path[len] = '\0';
	return (path);
}

char	*extract_texture_path(const char *line, const char *identifier)
{
	char	*trimmed;
	char	*path_start;
	char	*path_end;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (NULL);
	path_start = find_path_start(trimmed, identifier);
	if (*path_start == '\0')
		return (NULL);
	path_end = find_path_end(path_start);
	return (allocate_and_copy(path_start, path_end));
}
