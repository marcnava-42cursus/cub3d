/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_swapper_target_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:30:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "structs.h"
#include "logic_bonus.h"

bool	state_is_elevator_bonus(char c)
{
	return (get_elevator_index(c) >= 0);
}

int	get_elevator_slot_bonus(t_cub_data *data, char id)
{
	int	i;

	if (!data)
		return (-1);
	i = 0;
	while (i < data->elevator_id_count)
	{
		if (data->elevator_ids[i] == id)
			return (i);
		i++;
	}
	return (-1);
}

bool	state_target_inside_map_bonus(t_game *game, int x, int y)
{
	size_t	row_len;

	if (!game || !game->cub_data.map.grid)
		return (false);
	if (y < 0 || y >= game->cub_data.map.height)
		return (false);
	row_len = ft_strlen(game->cub_data.map.grid[y]);
	return (x >= 0 && x < (int)row_len);
}

bool	state_target_intersects_player_bonus(t_game *game, int x, int y)
{
	float	nearest_x;
	float	nearest_y;
	float	dx;
	float	dy;

	nearest_x = fmaxf((float)x, fminf(game->cub_data.player.x,
				(float)x + 1.0f));
	nearest_y = fmaxf((float)y, fminf(game->cub_data.player.y,
				(float)y + 1.0f));
	dx = game->cub_data.player.x - nearest_x;
	dy = game->cub_data.player.y - nearest_y;
	return ((dx * dx + dy * dy) <= game->player_radius * game->player_radius);
}

bool	state_is_valid_target_cell_bonus(t_game *game, int x, int y)
{
	int	player_x;
	int	player_y;

	if (!state_target_inside_map_bonus(game, x, y))
		return (false);
	player_x = (int)floorf(game->cub_data.player.x);
	player_y = (int)floorf(game->cub_data.player.y);
	if (x == player_x && y == player_y)
		return (false);
	if (state_target_intersects_player_bonus(game, x, y))
		return (false);
	return (game->cub_data.map.grid[y][x] == '0');
}
