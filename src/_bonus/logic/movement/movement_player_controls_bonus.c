/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 02:47:18 by marcnava         ###   ########.fr       */
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

bool	apply_pitch_input(t_game *game)
{
	bool	moved;
	float	look;
	float	max_pitch;
	float	pitch_delta;

	moved = false;
	if (game->key_up_pressed)
		moved = (pitch_player(game, true), true);
	if (game->key_down_pressed)
		moved = (pitch_player(game, false), true);
	look = game->controller.look;
	if (look > EPSILON || look < -EPSILON)
	{
		pitch_delta = 600.0f * (float)game->mlx->delta_time * look;
		game->cub_data.player.pitch += pitch_delta;
		max_pitch = game->double_buffer[NEXT]->height * 0.35f;
		game->cub_data.player.pitch = clamp(game->cub_data.player.pitch,
				-max_pitch, max_pitch);
		moved = true;
	}
	return (moved);
}

bool	process_movement_input(t_game *game)
{
	bool	moved;
	float	forward;
	float	strafe;

	if (!game)
		return (false);
	moved = false;
	read_move_axes(game, &forward, &strafe);
	moved = apply_translation_input(game, forward, strafe) || moved;
	moved = apply_rotation_input(game) || moved;
	moved = apply_pitch_input(game) || moved;
	return (moved);
}

char	get_player_cell(t_game *game)
{
	int	x;
	int	y;

	if (!game || !game->cub_data.map.grid)
		return (' ');
	x = (int)floor(game->cub_data.player.x);
	y = (int)floor(game->cub_data.player.y);
	if (y < 0 || y >= game->cub_data.map.height)
		return (' ');
	if (x < 0 || x >= (int)ft_strlen(game->cub_data.map.grid[y]))
		return (' ');
	return (game->cub_data.map.grid[y][x]);
}

void	try_switch_on_elevator(t_game *game, char cell, double now)
{
	if (!is_elevator_char_logic(cell))
		return ;
	if (now < game->movement_lock_until || !is_elevator_open(game, cell))
		return ;
	switch_floor(game, cell);
}

/**
 * @brief Handles rendering updates after movement
 *
 * This function updates all rendering components when the player moves:
 * 1. Updates player position on 2D map overlay (if visible)
 * 2. Re-renders the 3D view with double buffering
 * 3. Updates terminal debug output
 *
 * @param game Game state structure
 */
void	handle_movement_rendering(t_game *game)
{
	char	cell;
	double	now;

	if (!game)
		return ;
	now = mlx_get_time();
	cell = get_player_cell(game);
	try_switch_on_elevator(game, cell, now);
}
