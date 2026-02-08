/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_floor_switch_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include "render.h"

#include "structs.h"

#include "parser.h"

void	refresh_key_states_bonus(t_game *game);
void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);

#define HEADBOB_AMPLITUDE 5.0f
#define HEADBOB_SPEED 14.0f
#define HEADBOB_RECOVER_SPEED 30.0f
#define JUMP_PITCH_IMPULSE 700.0f
#define GRAVITY 2200.0f

bool	load_target_floor_textures(t_game *game, t_floor *to)
{
	game->cub_data.textures = to->textures;
	game->cub_data.floor_color = to->floor_color;
	game->cub_data.ceiling_color = to->ceiling_color;
	if (to->textures_loaded)
		return (true);
	if (!load_textures(&game->cub_data.textures))
	{
		printf("Error\nFailed to load textures for floor %s\n", to->path);
		return (false);
	}
	to->textures = game->cub_data.textures;
	to->textures_loaded = true;
	return (true);
}

void	apply_floor_switch(t_game *game, t_floor *to, int tx, int ty)
{
	game->cub_data.current_floor = to;
	game->cub_data.map = to->map;
	game->cub_data.player_floor_index = to->index;
	if (game->cub_data.player_floor_path)
		free(game->cub_data.player_floor_path);
	game->cub_data.player_floor_path = ft_strdup(to->path);
	game->cub_data.player.x = (float)tx + 0.5f;
	game->cub_data.player.y = (float)ty + 0.5f;
	game->last_grid_x = tx;
	game->last_grid_y = ty;
}

bool	resolve_floor_destination(t_game *game, char id,
				t_elevator_target *target, double *now)
{
	int	slot;

	if (!game || !game->cub_data.current_floor)
		return (false);
	slot = find_elevator_slot(&game->cub_data, id);
	if (slot < 0)
		return (false);
	*now = mlx_get_time();
	target->floor = get_target_floor(game, slot);
	if (!target->floor)
		return (false);
	if (!get_elevator_coords(target->floor, id, &target->x, &target->y))
	{
		printf("Error\nElevator '%c' missing target coords in %s\n",
			id, target->floor->path);
		return (false);
	}
	return (true);
}

bool	switch_floor(t_game *game, char id)
{
	t_elevator_target	target;
	double				now;

	if (!resolve_floor_destination(game, id, &target, &now))
		return (false);
	if (!load_target_floor_textures(game, target.floor))
		return (false);
	apply_floor_switch(game, target.floor, target.x, target.y);
	game->last_teleport_time = now;
	game->last_teleport_id = id;
	game->movement_lock_until = now + 1.0;
	printf("Elevator '%c': moved to floor %s (index %d) at (%d, %d)\n",
		id, target.floor->path, target.floor->index, target.x, target.y);
	return (true);
}

float	recover_to_zero(float value, float amount)
{
	if (value > amount)
		return (value - amount);
	if (value < -amount)
		return (value + amount);
	return (0.0f);
}
