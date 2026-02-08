/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_orchestrator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
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
		if (is_link_identifier(lines[i])
			&& !parse_link_line(lines[i], data))
			return (0);
		if (IS_TEXTURE_IDENTIFIER(lines[i])
			&& !PARSE_TEXTURE_LINE(lines[i], &data->textures))
			return (0);
		if (is_color_identifier(lines[i])
			&& !parse_color_line(lines[i], &data->floor_color,
				&data->ceiling_color))
			return (0);
		i++;
	}
	return (1);
}

static int	finalize_parsed_data(t_cub_data *data)
{
	if (data->floor_color.r == -1 || data->ceiling_color.r == -1)
		return (0);
	if (!data->textures.north_path || !data->textures.south_path
		|| !data->textures.west_path || !data->textures.east_path)
		return (0);
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
		return (0);
	return (1);
}

static int	error_message(const char *message)
{
	printf("%s\n", message);
	return (0);
}

int	process_file_data(char **lines, int line_count, t_cub_data *data)
{
	int	map_start;

	map_start = FIND_MAP_START(lines, line_count);
	if (map_start == -1)
		return (error_message("Error\nMap section not found"));
	if (!parse_elements(lines, map_start, data))
		return (error_message("Error\nFailed parsing elements before map"));
	if (!parse_map_section(lines, map_start, data))
		return (error_message("Error\nFailed parsing map section"));
	if (!finalize_parsed_data(data))
		return (error_message(
				"Error\nParsed data invalid (missing textures/map/player)"));
	return (1);
}

int	parse_cub_file(const char *filename, t_cub_data *data)
{
	char	**lines;
	int		line_count;

	if (!validate_file_extension(filename))
	{
		printf("Error\nInvalid file extension. Expected .cub\n");
		return (0);
	}
	init_cub_data(data);
	lines = read_file_lines(filename, &line_count);
	if (!lines)
	{
		printf("Error\nCould not read file %s\n", filename);
		return (0);
	}
	if (!process_file_data(lines, line_count, data))
	{
		free_cub_data(data);
		free_lines(lines, line_count);
		return (0);
	}
	free_lines(lines, line_count);
	return (1);
}
