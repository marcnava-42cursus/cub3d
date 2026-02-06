/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/23 15:30:15 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static int	calculate_map_width(char **lines, int start, int end)
{
	int	max_width;
	int	i;
	int	len;

	max_width = 0;
	i = start;
	while (i <= end)
	{
		len = ft_strlen(lines[i]);
		if (len > 0 && lines[i][len - 1] == '\n')
			len--;
		if (len > max_width)
			max_width = len;
		i++;
	}
	return (max_width);
}

static void	free_partial_grid(char **grid, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(grid[j]);
		j++;
	}
	free(grid);
}

static int	copy_map_line(t_cub_data *data, char *line, int i)
{
	int	line_len;
	int	j;

	line_len = ft_strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
		line_len--;
	data->map.grid[i] = malloc(data->map.width + 1);
	if (!data->map.grid[i])
	{
		free_partial_grid(data->map.grid, i);
		return (0);
	}
	j = 0;
	while (j < line_len)
	{
		data->map.grid[i][j] = line[j];
		j++;
	}
	while (j < data->map.width)
	{
		data->map.grid[i][j] = ' ';
		j++;
	}
	data->map.grid[i][data->map.width] = '\0';
	return (1);
}

static int	fill_map_grid(char **lines, int start_line, t_cub_data *data)
{
	int	i;

	data->map.grid = malloc(sizeof(char *) * (data->map.height + 1));
	if (!data->map.grid)
		return (0);
	i = 0;
	while (i < data->map.height)
	{
		if (!copy_map_line(data, lines[start_line + i], i))
			return (0);
		i++;
	}
	data->map.grid[data->map.height] = NULL;
	return (1);
}

int	parse_map_section(char **lines, int start_line, t_cub_data *data)
{
	int	i;
	int	map_end;

	map_end = -1;
	i = start_line;
	while (lines[i])
	{
		if (is_empty_line(lines[i]))
			return (printf("Error: Empty line inside map\n"), 0);
		if (!is_map_line(lines[i]))
			return (printf("Error: Invalid line after map start\n"), 0);
		map_end = i;
		i++;
	}
	if (map_end < start_line)
		return (printf("Error: Invalid map section\n"), 0);
	data->map.height = map_end - start_line + 1;
	data->map.width = calculate_map_width(lines, start_line, map_end);
	if (data->map.height <= 0 || data->map.width <= 0)
		return (printf("Error: Invalid map dimensions\n"), 0);
	if (!fill_map_grid(lines, start_line, data))
		return (0);
	return (validate_map(&data->map, &data->player));
}
