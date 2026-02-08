/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_identifier_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_texture_identifier_bonus(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (is_link_identifier_bonus(trimmed))
		return (0);
	if (ft_strlen(trimmed) < 3)
		return (0);
	result = (ft_isgraph(trimmed[0])
			&& ft_isalpha(trimmed[1])
			&& trimmed[2] == ' ');
	return (result);
}

char	*extract_texture_path_bonus(const char *line, const char *identifier)
{
	char	*trimmed;
	char	*path_start;
	char	*path_end;
	char	*path;
	int		len;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (NULL);
	path_start = trimmed + ft_strlen(identifier);
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
		return (NULL);
	path_end = path_start + ft_strlen(path_start) - 1;
	while (path_end > path_start && (*path_end == ' '
			|| *path_end == '\t' || *path_end == '\n'))
		path_end--;
	len = path_end - path_start + 1;
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	ft_strlcpy(path, path_start, len + 1);
	return (path);
}

int	extract_identifier_bonus(const char *line, char identifier[3])
{
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || ft_strlen(trimmed) < 3)
		return (0);
	identifier[0] = trimmed[0];
	identifier[1] = trimmed[1];
	identifier[2] = '\0';
	return (1);
}

int	validate_identifier_bonus(const char *identifier)
{
	if (!ft_isalpha(identifier[1]) || !ft_isgraph(identifier[0]))
	{
		printf("Error\nInvalid texture identifier: %s\n", identifier);
		return (0);
	}
	return (1);
}
