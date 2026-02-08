/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_orchestrator_links_bonus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:28 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*extract_link_path_bonus(const char *line, size_t id_len)
{
	char	*path_start;

	if (!line)
		return (NULL);
	path_start = (char *)line + id_len;
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
		return (NULL);
	return (ft_strdup(path_start));
}

static int	store_link_path_bonus(char **slot, char *path, const char *label)
{
	if (*slot)
	{
		printf("Error: Duplicate %s definition\n", label);
		free(path);
		return (0);
	}
	*slot = path;
	return (1);
}

static int	resolve_link_target_bonus(char *trimmed, t_cub_data *data,
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

static int	validate_link_path_bonus(char *path, const char *label)
{
	if (!path || *path == '\0')
		return (free(path), 1);
	if (!validate_file_extension(path))
	{
		printf("Error: %s path must be a .cub file\n", label);
		free(path);
		return (0);
	}
	return (1);
}

int	parse_link_line_bonus(const char *line, t_cub_data *data)
{
	char		*path;
	char		*trimmed;
	t_link_info	info;

	ft_bzero(&info, sizeof(info));
	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (!resolve_link_target_bonus(trimmed, data, &info))
		return (0);
	path = extract_link_path_bonus(trimmed, info.id_len);
	if (!validate_link_path_bonus(path, info.label))
		return (0);
	return (store_link_path_bonus(info.target, path, info.label));
}
