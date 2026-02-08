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

void	reset_headbob_state(t_game *game)
{
	game->headbob_offset = 0.0f;
	game->headbob_phase = 0.0f;
	game->headbob_applied = 0.0f;
}

void	reset_jump_state(t_game *game)
{
	game->jump_active = false;
	game->jump_offset = 0.0f;
	game->jump_velocity = 0.0f;
	game->jump_applied = 0.0f;
}

void	clear_pitch_offsets(t_game *game)
{
	if (!game)
		return ;
	if (game->headbob_applied != 0.0f)
		game->cub_data.player.pitch -= game->headbob_applied;
	if (game->jump_applied != 0.0f)
		game->cub_data.player.pitch -= game->jump_applied;
	game->headbob_applied = 0.0f;
	game->jump_applied = 0.0f;
}

vertex_t	capture_player_position(t_game *game)
{
	vertex_t	snapshot;

	snapshot.x = game->cub_data.player.x;
	snapshot.y = game->cub_data.player.y;
	return (snapshot);
}

bool	player_translated_since(t_game *game,
				vertex_t prev)
{
	return (fabsf(game->cub_data.player.x - prev.x) > 0.0001f
		|| fabsf(game->cub_data.player.y - prev.y) > 0.0001f);
}
