/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 20:30:52 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				if (y == 0 || y == map->height - 1)
				{
					if (c != '1')
					{
						printf("Error: Map not closed at border (%d, %d)\n",
							x, y);
						return (0);
					}
				}
				if (x == 0 || x == (int)ft_strlen(map->grid[y]) - 1)
				{
					if (c != '1')
					{
						printf("Error: Map not closed at border (%d, %d)\n",
							x, y);
						return (0);
					}
				}
				if (y > 0 && x < (int)ft_strlen(map->grid[y - 1])
					&& map->grid[y - 1][x] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
				if (y < map->height - 1 && x < (int)ft_strlen(map->grid[y + 1])
					&& map->grid[y + 1][x] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
				if (x > 0 && map->grid[y][x - 1] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
				if (x < (int)ft_strlen(map->grid[y]) - 1
					&& map->grid[y][x + 1] == ' ')
				{
					printf(
						"Error: Map not closed - space adjacent at (%d, %d)\n",
						x, y);
					return (0);
				}
			}
			x++;
		}
		y++;
	}
	return (1);
}

int	validate_map(t_map *map, t_player *player)
{
	if (!validate_map_characters(map))
		return (0);
	if (!find_player_position(map, player))
		return (0);
	if (!is_map_closed(map))
		return (0);
	return (1);
}
