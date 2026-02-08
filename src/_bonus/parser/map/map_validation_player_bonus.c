/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_player_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static void	set_player_from_cell_bonus(t_player *player, char orientation,
		int x, int y)
{
	player->x = (float)x + 0.5f;
	player->y = (float)y + 0.5f;
	player->orientation = orientation;
	if (orientation == NORTH)
		player->angle = 3.0f * (FT_PI / 2);
	else if (orientation == SOUTH)
		player->angle = FT_PI / 2;
	else if (orientation == EAST)
		player->angle = 0.0f;
	else if (orientation == WEST)
		player->angle = FT_PI;
}

static int	process_player_cell_bonus(t_map *map, t_player *player,
		int *found, int pos[2])
{
	if (!is_player_char(map->grid[pos[1]][pos[0]]))
		return (1);
	if (*found)
	{
		printf("Error\nMultiple player positions found\n");
		return (0);
	}
	set_player_from_cell_bonus(player, map->grid[pos[1]][pos[0]],
		pos[0], pos[1]);
	map->grid[pos[1]][pos[0]] = '0';
	*found = 1;
	return (1);
}

int	find_player_position_bonus(t_map *map, t_player *player)
{
	int	pos[2];
	int	found;

	found = 0;
	pos[1] = 0;
	while (pos[1] < map->height)
	{
		pos[0] = 0;
		while (pos[0] < (int)ft_strlen(map->grid[pos[1]]))
		{
			if (!process_player_cell_bonus(map, player, &found, pos))
				return (0);
			pos[0]++;
		}
		pos[1]++;
	}
	if (!found)
	{
		player->x = -1.0f;
		player->y = -1.0f;
		player->angle = 0.0f;
		player->orientation = 0;
	}
	return (1);
}

int	validate_map_bonus(t_map *map, t_player *player)
{
	if (!validate_map_characters_bonus(map))
		return (0);
	if (!find_player_position_bonus(map, player))
		return (0);
	if (!is_map_closed_bonus(map))
		return (0);
	return (1);
}
