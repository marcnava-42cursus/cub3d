/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_elevator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 19:25:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 19:50:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "parser.h"
#include "structs.h"
#include <math.h>

static t_floor	*get_target_floor(t_cub_data *data, char id)
{
	int		i;
	t_floor	*to;

	if (!data || !data->current_floor)
		return (NULL);
	i = 0;
	while (i < data->elevator_id_count && data->elevator_ids[i] != id)
		i++;
	if (i >= data->elevator_id_count)
		return (NULL);
	to = data->elevator_floor_a[i];
	if (to == data->current_floor)
		to = data->elevator_floor_b[i];
	if (!to || to == data->current_floor)
		return (NULL);
	return (to);
}

static int	set_target_coords(t_floor *to, char id, t_elevator_target *target)
{
	int	i;

	if (!to)
		return (0);
	i = 0;
	while (i < to->elevator_count && to->elevator_ids[i] != id)
		i++;
	if (i >= to->elevator_count)
	{
		printf("Error: Elevator '%c' missing target coords in %s\n",
			id, to->path);
		return (0);
	}
	target->floor = to;
	target->x = to->elevator_x[i];
	target->y = to->elevator_y[i];
	return (1);
}

static void	apply_floor_switch(t_game *game, t_elevator_target *target,
		char id, double now)
{
	game->cub_data.current_floor = target->floor;
	game->cub_data.map = target->floor->map;
	game->cub_data.player_floor_index = target->floor->index;
	if (game->cub_data.player_floor_path)
		free(game->cub_data.player_floor_path);
	game->cub_data.player_floor_path = ft_strdup(target->floor->path);
	game->cub_data.player.x = (float)target->x + 0.5f;
	game->cub_data.player.y = (float)target->y + 0.5f;
	game->last_grid_x = target->x;
	game->last_grid_y = target->y;
	game->last_teleport_time = now;
	game->last_teleport_id = id;
	game->movement_lock_until = now + 1.0;
	printf("Elevator '%c': moved to floor %s (index %d) at (%d, %d)\n",
		id, target->floor->path, target->floor->index, target->x, target->y);
}

static bool	switch_floor(t_game *game, char id, double now)
{
	t_elevator_target	target;
	t_floor				*to;

	if (!game)
		return (false);
	to = get_target_floor(&game->cub_data, id);
	if (!set_target_coords(to, id, &target))
		return (false);
	game->cub_data.textures = target.floor->textures;
	game->cub_data.floor_color = target.floor->floor_color;
	game->cub_data.ceiling_color = target.floor->ceiling_color;
	if (!target.floor->textures_loaded)
	{
		if (!load_textures(&game->cub_data.textures))
		{
			printf("Error: Failed to load textures for floor %s\n",
				target.floor->path);
			return (false);
		}
		target.floor->textures = game->cub_data.textures;
		target.floor->textures_loaded = true;
	}
	apply_floor_switch(game, &target, id, now);
	return (true);
}

void	movement_handle_elevator(t_game *game)
{
	int		grid_x;
	int		grid_y;
	char	cell;
	double	now;
	char	*set;

	if (!game || !game->cub_data.map.grid)
		return ;
	grid_x = (int)floor(game->cub_data.player.x);
	grid_y = (int)floor(game->cub_data.player.y);
	if (grid_y < 0 || grid_y >= game->cub_data.map.height)
		return ;
	if (grid_x < 0 || grid_x >= (int)ft_strlen(
			game->cub_data.map.grid[grid_y]))
		return ;
	cell = game->cub_data.map.grid[grid_y][grid_x];
	set = "!\"·$%&/()=?¿";
	if (!ft_strchr(set, cell))
		return ;
	now = mlx_get_time();
	if (now < game->movement_lock_until)
		return ;
	switch_floor(game, cell, now);
}
