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

bool	update_step_audio_and_headbob(t_game *game, bool player_translated,
				float prev_headbob)
{
	bool	step_started;

	step_started = audio_step_update_loop(player_translated);
	return (apply_headbob_pitch(game, player_translated, step_started,
			prev_headbob));
}

bool	update_jump_pitch(t_game *game, float prev_jump)
{
	float	max_pitch;
	float	prev_pitch;

	if (!game || !game->mlx)
		return (false);
	if (game->jump_active)
	{
		game->jump_velocity -= GRAVITY * (float)game->mlx->delta_time;
		game->jump_offset += game->jump_velocity * (float)game->mlx->delta_time;
		if (game->jump_offset <= 0.0f && game->jump_velocity < 0.0f)
			reset_jump_state(game);
	}
	prev_pitch = game->cub_data.player.pitch;
	max_pitch = game->double_buffer[NEXT]->height * 0.35f;
	game->cub_data.player.pitch = clamp(prev_pitch + game->jump_offset,
			-max_pitch, max_pitch);
	game->jump_applied = game->cub_data.player.pitch - prev_pitch;
	return (fabsf(game->jump_offset - prev_jump) > 0.0001f);
}

void	trigger_jump_bonus(t_game *game)
{
	if (!game || !game->mlx || game->jump_active)
		return ;
	game->jump_active = true;
	game->jump_offset = 0.0f;
	game->jump_velocity = JUMP_PITCH_IMPULSE;
}

bool	handle_modal_loop(t_game *game)
{
	if (!is_config_modal_open(game))
		return (false);
	clear_pitch_offsets(game);
	audio_step_update_loop(false);
	reset_headbob_state(game);
	reset_jump_state(game);
	controller_handle_rebind_bonus(game);
	controller_update_bonus(game);
	update_config_modal(game);
	return (true);
}

bool	throttle_fps_loop(t_game *game, double *next_tick, int *last_limit)
{
	double	now;
	double	min_step;
	int		fps_limit;

	fps_limit = config_fps_limit_value(game->menu.options.fps_limit_index);
	if (fps_limit != *last_limit)
	{
		*next_tick = 0.0;
		*last_limit = fps_limit;
	}
	if (fps_limit <= 0)
	{
		*next_tick = 0.0;
		return (false);
	}
	now = mlx_get_time();
	min_step = 1.0 / (double)fps_limit;
	if (*next_tick <= 0.0)
		*next_tick = now + min_step;
	if (now < *next_tick)
		return (true);
	*next_tick += min_step;
	while (now >= *next_tick)
		*next_tick += min_step;
	return (false);
}
