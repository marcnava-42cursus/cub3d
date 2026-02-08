/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/23 19:42:24 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static int	check_border_integrity(t_map *map, int x, int y)
{
	if (y == 0 || y == map->height - 1 || x == 0
		|| x == (int)ft_strlen(map->grid[y]) - 1)
	{
		if (map->grid[y][x] != '1')
		{
			printf("Error: Map not closed at border (%d, %d)\n", x, y);
			return (0);
		}
	}
	return (1);
}

static int	check_neighbor_integrity(t_map *map, int x, int y)
{
	if ((y > 0 && x < (int)ft_strlen(map->grid[y - 1])
			&& map->grid[y - 1][x] == ' ')
		|| (y < map->height - 1 && x < (int)ft_strlen(map->grid[y + 1])
			&& map->grid[y + 1][x] == ' ')
		|| (x > 0 && map->grid[y][x - 1] == ' ')
		|| (x < (int)ft_strlen(map->grid[y]) - 1
			&& map->grid[y][x + 1] == ' '))
	{
		printf("Error: Map not closed - space adjacent at (%d, %d)\n", x, y);
		return (0);
	}
	return (1);
}

static int	is_walkable_char(char c)
{
	if (c == '0')
		return (1);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

int	is_map_closed(t_map *map)
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
			if (is_walkable_char(c))
			{
				if (!check_border_integrity(map, x, y))
					return (0);
				if (!check_neighbor_integrity(map, x, y))
					return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}

int	validate_map(t_map *map, t_player *player)
{
	if (!VALIDATE_MAP_CHARACTERS(map))
		return (0);
	if (!FIND_PLAYER_POSITION(map, player))
		return (0);
	if (!IS_MAP_CLOSED(map))
		return (0);
	return (1);
}
