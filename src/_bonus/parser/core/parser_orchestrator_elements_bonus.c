/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_orchestrator_elements_bonus.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:24 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	apply_default_colors_bonus(t_cub_data *data)
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

static int	parse_element_line_bonus(char *line, t_cub_data *data)
{
	if (is_link_identifier_bonus(line))
		return (parse_link_line_bonus(line, data));
	if (IS_TEXTURE_IDENTIFIER(line))
		return (PARSE_TEXTURE_LINE(line, &data->textures));
	if (is_color_identifier(line))
		return (parse_color_line(line, &data->floor_color,
				&data->ceiling_color));
	return (1);
}

int	parse_elements_bonus(char **lines, int map_start, t_cub_data *data)
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
		if (!parse_element_line_bonus(lines[i], data))
			return (0);
		i++;
	}
	return (1);
}

int	validate_parsed_data_bonus(const t_cub_data *data)
{
	if (!data->textures.north_path || !data->textures.south_path
		|| !data->textures.west_path || !data->textures.east_path)
		return (0);
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
		return (0);
	return (1);
}

int	parse_map_with_errors_bonus(char **lines, int map_start,
		t_cub_data *data, const char *error_prefix)
{
	apply_default_colors_bonus(data);
	if (!parse_map_section(lines, map_start, data))
	{
		printf("%sFailed parsing map section\n", error_prefix);
		return (0);
	}
	if (!validate_parsed_data_bonus(data))
	{
		printf("%sParsed data invalid (missing textures)\n", error_prefix);
		return (0);
	}
	return (1);
}
