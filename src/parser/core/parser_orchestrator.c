/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_orchestrator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/19 20:06:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

void	init_cub_data(t_cub_data *data);
void	init_cub_data_parser_only(t_cub_data *data);
int		process_file_data(char **lines, int line_count, t_cub_data *data);

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
		if (is_link_identifier(lines[i]))
		{
			if (!parse_link_line(lines[i], data))
				return (0);
		}
		else if (is_texture_identifier(lines[i]))
		{
			if (!parse_texture_line(lines[i], &data->textures))
				return (0);
		}
		else if (is_color_identifier(lines[i]))
		{
			if (!parse_color_line(lines[i], &data->floor_color,
					&data->ceiling_color))
				return (0);
		}
		i++;
	}
	return (1);
}

static void	apply_default_colors(t_cub_data *data)
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

static int	validate_parsed_data(const t_cub_data *data)
{
	if (!data->textures.north_path || !data->textures.south_path
		|| !data->textures.west_path || !data->textures.east_path)
		return (0);
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
		return (0);
	return (1);
}

int	process_file_data(char **lines, int line_count, t_cub_data *data)
{
	int	map_start;

	map_start = find_map_start(lines, line_count);
	if (map_start == -1)
	{
		printf("Error: Map section not found\n");
		return (0);
	}
	if (!parse_elements(lines, map_start, data))
	{
		printf("Error: Failed parsing elements ");
		printf("(textures/colors/links) before map\n");
		return (0);
	}
	apply_default_colors(data);
	if (!parse_map_section(lines, map_start, data))
	{
		printf("Error: Failed parsing map section\n");
		return (0);
	}
	if (!validate_parsed_data(data))
	{
		printf(
			"%s Parsed data invalid (missing textures"
			"/map/player)\n",
			"Error: ");
		return (0);
	}
	return (1);
}

int	parse_cub_file(const char *filename, t_cub_data *data)
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
	if (!process_file_data(lines, line_count, data))
	{
		free_cub_data(data);
		free_lines(lines, line_count);
		return (0);
	}
#ifdef BONUS
	if (!build_floor_graph_bonus(filename, data))
	{
		free_lines(lines, line_count);
		return (0);
	}
#endif
	free_lines(lines, line_count);
	return (1);
}
