/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_orchestrator_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 15:12:38 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/31 16:30:34 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

void	init_cub_data(t_cub_data *data);
void	init_cub_data_parser_only(t_cub_data *data);
int		process_file_data(char **lines, int line_count, t_cub_data *data);

static char	*extract_link_path_advanced(const char *line, size_t id_len)
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

static int	store_link_path_advanced(char **slot, char *path, const char *label)
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

static int	resolve_link_target_advanced(char *trimmed, t_cub_data *data,
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

static int	validate_link_path_advanced(char *path, const char *label)
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

static int	parse_link_line_advanced(const char *line, t_cub_data *data)
{
	char		*path;
	char		*trimmed;
	t_link_info	info;

	ft_bzero(&info, sizeof(info));
	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (!resolve_link_target_advanced(trimmed, data, &info))
		return (0);
	path = extract_link_path_advanced(trimmed, info.id_len);
	if (!validate_link_path_advanced(path, info.label))
		return (0);
	return (store_link_path_advanced(info.target, path, info.label));
}

static void	apply_default_colors_advanced(t_cub_data *data)
{
	if (data->floor_color.r == -1)
	{
		data->floor_color.r = 30;
		data->floor_color.g = 30;
		data->floor_color.b = 30;
	}
	if (data->ceiling_color.r == -1)
	{
		data->ceiling_color.r = 120;
		data->ceiling_color.g = 170;
		data->ceiling_color.b = 255;
	}
}

static int	parse_element_line_advanced(char *line, t_cub_data *data)
{
	if (is_link_identifier_advanced(line))
		return (parse_link_line_advanced(line, data));
	if (is_texture_identifier(line))
		return (parse_texture_line(line, &data->textures));
	if (is_color_identifier(line))
		return (parse_color_line(line, &data->floor_color,
				&data->ceiling_color));
	return (1);
}

static int	parse_elements(char **lines, int map_start, t_cub_data *data)
{
	int	i;

	i = 0;
	while (i < map_start)
	{
		if (is_empty_line(lines[i]))
		{
			i++;
			continue ;
		}
		if (!parse_element_line_advanced(lines[i], data))
			return (0);
		i++;
	}
	return (1);
}

static int	validate_parsed_data(const t_cub_data *data)
{
	if (!data->textures.north_path || !data->textures.south_path
		|| !data->textures.west_path || !data->textures.east_path)
		return (0);
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
		return (0);
	return (1);
}

static int	parse_map_with_errors_advanced(char **lines, int map_start,
		t_cub_data *data, const char *e)
{
	apply_default_colors_advanced(data);
	if (!parse_map_section(lines, map_start, data))
	{
		printf("%sFailed parsing map section\n", e);
		return (0);
	}
	if (!validate_parsed_data(data))
	{
		printf("%sParsed data invalid (missing textures)\n", e);
		return (0);
	}
	return (1);
}

int	process_file_data_advanced(char **lines, int line_count, t_cub_data *data)
{
	int			map_start;
	const char	*e = "Error: ";

	map_start = find_map_start(lines, line_count);
	if (map_start == -1)
	{
		printf("%sMap section not found\n", e);
		return (0);
	}
	if (!parse_elements(lines, map_start, data))
	{
		printf("%sFailed parsing elements before map\n", e);
		return (0);
	}
	if (!parse_map_with_errors_advanced(lines, map_start, data, e))
		return (0);
	return (1);
}

int	parse_cub_file_advanced(const char *filename, t_cub_data *data)
{
	char	**lines;
	int		line_count;

	if (!validate_file_extension(filename))
	{
		printf("Error: Invalid file extension. Expected .cub\n");
		return (0);
	}
	init_cub_data(data);
	lines = read_file_lines(filename, &line_count);
	if (!lines)
	{
		printf("Error: Could not read file %s\n", filename);
		return (0);
	}
	if (!process_file_data_advanced(lines, line_count, data))
	{
		free_cub_data(data);
		free_lines(lines, line_count);
		return (0);
	}
	if (!build_floor_graph_advanced(filename, data))
		return (0);
	free_lines(lines, line_count);
	return (1);
}
