/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_characters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/23 15:29:27 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_valid_map_char(char c);

int	is_map_line(const char *line)
{
	int	i;
	int	has_map_char;

	if (!line || ft_strlen(line) == 0)
		return (0);
	has_map_char = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\n'
			&& line[i] != '\t' && line[i] != '\r')
		{
			if (!is_valid_map_char(line[i]))
				return (0);
			has_map_char = 1;
		}
		i++;
	}
	return (has_map_char);
}

static int	is_valid_map_char(char c)
{
	return (c >= 33 && c <= 126);
}

int	validate_map_characters(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(map->grid[y]))
		{
			if (!is_valid_map_char(map->grid[y][x])
				&& map->grid[y][x] != ' ')
			{
				printf("Error: Invalid character '%c' at position (%d, %d)\n",
					map->grid[y][x], x, y);
				return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}
