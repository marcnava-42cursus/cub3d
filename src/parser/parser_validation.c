/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/02 02:30:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

int	validate_parsed_data(const t_cub_data *data)
{
	if (!data->textures.north || !data->textures.south
		|| !data->textures.west || !data->textures.east)
	{
		printf("Error: Missing texture definitions\n");
		return (0);
	}
	if (data->floor_color.r == -1 || data->ceiling_color.r == -1)
	{
		printf("Error: Missing color definitions\n");
		return (0);
	}
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
	{
		printf("Error: Invalid or missing map\n");
		return (0);
	}
	if (data->player.x == -1 || data->player.y == -1)
	{
		printf("Error: Player position not found\n");
		return (0);
	}
	return (1);
}

static int	check_element_type(char *line, int *texture_count, int *color_count)
{
	if (is_texture_identifier(line))
	{
		(*texture_count)++;
		return (0);
	}
	if (is_color_identifier(line))
	{
		(*color_count)++;
		return (0);
	}
	if (is_map_line(line) && *texture_count == 4 && *color_count == 2)
		return (1);
	if (!is_texture_identifier(line) && !is_color_identifier(line)
		&& !is_empty_line(line))
	{
		if (*texture_count < 4 || *color_count < 2)
			return (-1);
		return (1);
	}
	return (0);
}

static int	count_elements(char **lines, int line_count,
	int *texture_count, int *color_count)
{
	int	i;
	int	result;

	*texture_count = 0;
	*color_count = 0;
	i = 0;
	while (i < line_count)
	{
		if (is_empty_line(lines[i]))
		{
			i++;
			continue ;
		}
		result = check_element_type(lines[i], texture_count, color_count);
		if (result == 1)
			return (i);
		if (result == -1)
		{
			printf("Error: Map found before all elements are defined\n");
			return (-1);
		}
		i++;
	}
	printf("Error: Map section not found\n");
	return (-1);
}

int	find_map_start(char **lines, int line_count)
{
	int	texture_count;
	int	color_count;

	return (count_elements(lines, line_count, &texture_count, &color_count));
}
