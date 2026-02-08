/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_closed_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:33:10 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_walkable_char_bonus(char c)
{
	if (c == '0')
		return (1);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	if (c >= 'a' && c <= 'z')
		return (1);
	return (0);
}

static int	is_walkable_on_border_bonus(t_map *map, int x, int y)
{
	if (y == 0 || y == map->height - 1)
		return (1);
	if (x == 0 || x == (int)ft_strlen(map->grid[y]) - 1)
		return (1);
	return (0);
}

static int	has_adjacent_space_bonus(t_map *map, int x, int y)
{
	if (y > 0 && x < (int)ft_strlen(map->grid[y - 1])
		&& map->grid[y - 1][x] == ' ')
		return (1);
	if (y < map->height - 1 && x < (int)ft_strlen(map->grid[y + 1])
		&& map->grid[y + 1][x] == ' ')
		return (1);
	if (x > 0 && map->grid[y][x - 1] == ' ')
		return (1);
	if (x < (int)ft_strlen(map->grid[y]) - 1 && map->grid[y][x + 1] == ' ')
		return (1);
	return (0);
}

static int	validate_walkable_cell_bonus(t_map *map, char c, int x, int y)
{
	if (is_walkable_on_border_bonus(map, x, y))
	{
		printf("Error: Walkable char '%c' at border (%d, %d)\n", c, x, y);
		return (0);
	}
	if (has_adjacent_space_bonus(map, x, y))
	{
		printf("Error: Map not closed - space adjacent at (%d, %d)\n", x, y);
		return (0);
	}
	return (1);
}

int	is_map_closed_bonus(t_map *map)
{
	int		x;
	int		y;
	char	c;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(map->grid[y]))
		{
			c = map->grid[y][x];
			if (is_walkable_char_bonus(c)
				&& !validate_walkable_cell_bonus(map, c, x, y))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}
