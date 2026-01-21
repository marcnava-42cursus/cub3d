/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

int	is_link_identifier(const char *line)
{
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || *trimmed == '\0')
		return (0);
	if (ft_strncmp(trimmed, "UP", 2) == 0
		&& (trimmed[2] == '\0' || trimmed[2] == ' '))
		return (1);
	if (ft_strncmp(trimmed, "DOWN", 4) == 0
		&& (trimmed[4] == '\0' || trimmed[4] == ' '))
		return (1);
	return (0);
}

int	parse_link_line(const char *line, t_cub_data *data)
{
	char		*trimmed;
	char		*path_start;
	char		*path;
	char		**target;
	const char	*label;
	size_t		id_len;

	target = NULL;
	label = "";
	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (ft_strncmp(trimmed, "UP", 2) == 0
		&& (trimmed[2] == '\0' || trimmed[2] == ' '))
	{
		id_len = 2;
		target = &data->up_path;
		label = "UP";
	}
	else if (ft_strncmp(trimmed, "DOWN", 4) == 0
		&& (trimmed[4] == '\0' || trimmed[4] == ' '))
	{
		id_len = 4;
		target = &data->down_path;
		label = "DOWN";
	}
	if (!target)
		return (0);
	path_start = trimmed + id_len;
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
		return (1);
	path = ft_strdup(path_start);
	if (!path)
		return (0);
	if (!validate_file_extension(path))
	{
		printf("Error: %s path must be a .cub file\n", label);
		free(path);
		return (0);
	}
	if (*target)
	{
		printf("Error: Duplicate %s definition\n", label);
		free(path);
		return (0);
	}
	*target = path;
	return (1);
}
