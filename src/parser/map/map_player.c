/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_player.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/13 15:25:23 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	set_player_orientation(t_player *player, char c)
{
	player->orientation = (t_orientation)c;
	if (player->orientation == NORTH)
		player->angle = 3.0f * (FT_PI / 2);
	else if (player->orientation == SOUTH)
		player->angle = FT_PI / 2;
	else if (player->orientation == EAST)
		player->angle = 0.0f;
	else if (player->orientation == WEST)
		player->angle = FT_PI;
}

static void	set_player_pos(t_player *player, int x, int y, char c)
{
	player->x = (float)x;
	player->y = (float)y;
	set_player_orientation(player, c);
}

static void	scan_row_for_player(t_map *map, t_player *player, int y, int *count)
{
	int		x;
	char	c;

	x = 0;
	while (x < (int)ft_strlen(map->grid[y]))
	{
		c = map->grid[y][x];
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			(*count)++;
			set_player_pos(player, x, y, c);
			map->grid[y][x] = '0';
		}
		x++;
	}
}

int	find_player_position(t_map *map, t_player *player)
{
	int	y;
	int	player_count;

	player_count = 0;
	y = 0;
	while (y < map->height)
	{
		scan_row_for_player(map, player, y, &player_count);
		y++;
	}
	if (player_count == 0)
		return (printf("Error: No player position found in map\n"), 0);
	if (player_count > 1)
		return (printf("Error: Multiple player positions found in map\n"), 0);
	return (1);
}
