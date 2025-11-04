/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_player.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	find_player_position(t_map *map, t_player *player)
{
	int		x;
	int		y;
	int		player_count;
	char	c;

	player_count = 0;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < (int)ft_strlen(map->grid[y]))
		{
			c = map->grid[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				player_count++;
				player->x = (float)x + 0.5f;
				player->y = (float)y + 0.5f;
				player->orientation = (t_orientation)c;
				if (player->orientation == NORTH)
					player->angle = 3.0f * (FT_PI / 2);
				else if (player->orientation == SOUTH)
					player->angle = FT_PI / 2;
				else if (player->orientation == EAST)
					player->angle = 0.0f;
				else if (player->orientation == WEST)
					player->angle = FT_PI;
				map->grid[y][x] = '0';
			}
			x++;
		}
		y++;
	}
	if (player_count == 0)
	{
		printf("Error: No player position found in map\n");
		return (0);
	}
	if (player_count > 1)
	{
		printf("Error: Multiple player positions found in map\n");
		return (0);
	}
	return (1);
}
