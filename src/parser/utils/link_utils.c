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

static int	fill_link_info(const char *trimmed, t_cub_data *data,
		t_link_info *info)
{
	if (ft_strncmp(trimmed, "UP", 2) == 0
		&& (trimmed[2] == '\0' || trimmed[2] == ' '))
	{
		info->id_len = 2;
		info->target = &data->up_path;
		info->label = "UP";
		return (1);
	}
	if (ft_strncmp(trimmed, "DOWN", 4) == 0
		&& (trimmed[4] == '\0' || trimmed[4] == ' '))
	{
		info->id_len = 4;
		info->target = &data->down_path;
		info->label = "DOWN";
		return (1);
	}
	return (0);
}

static char	*extract_link_path(const char *trimmed, size_t id_len)
{
	const char	*path_start;

	path_start = trimmed + id_len;
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
		return (NULL);
	return (ft_strdup(path_start));
}

int	parse_link_line(const char *line, t_cub_data *data)
{
	char		*trimmed;
	char		*path;
	t_link_info	info;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || !fill_link_info(trimmed, data, &info))
		return (0);
	path = extract_link_path(trimmed, info.id_len);
	if (!path)
		return (1);
	if (!validate_file_extension(path))
	{
		printf("Error: %s path must be a .cub file\n", info.label);
		free(path);
		return (0);
	}
	if (*info.target)
	{
		printf("Error: Duplicate %s definition\n", info.label);
		free(path);
		return (0);
	}
	*info.target = path;
	return (1);
}
