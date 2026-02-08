/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_mouse_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 07:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 12:31:31 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"
#include "render.h"

void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param)
{
	t_game	*game;
	int32_t	mouse_x;
	int32_t	mouse_y;
	int		index;

	(void)mods;
	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
	if (!is_config_modal_open(game))
		return ;
	if (button != MLX_MOUSE_BUTTON_LEFT || action != MLX_PRESS)
		return ;
	mlx_get_mouse_pos(game->mlx, &mouse_x, &mouse_y);
	index = config_option_from_pos(game, mouse_x, mouse_y);
	if (index < 0)
		return ;
	config_option_toggle(game, index);
}

void	cursor_hook_bonus(double xpos, double ypos, void *param)
{
	t_game	*game;
	double	delta_x;
	double	delta_y;

	game = (t_game *)param;
	if (!game || is_config_modal_open(game) || !game->mlx)
		return ;
	if (!game->mouse_initialized)
	{
		game->last_mouse_x = xpos;
		game->last_mouse_y = ypos;
		game->mouse_initialized = true;
		return ;
	}
	delta_x = xpos - game->last_mouse_x;
	delta_y = ypos - game->last_mouse_y;
	game->last_mouse_x = xpos;
	game->last_mouse_y = ypos;
	game->mouse_delta_accumulated += (float)delta_x * game->mouse_sensitivity;
	game->mouse_delta_accumulated_y += ((float)(-delta_y))
		* game->mouse_sensitivity * 220.0f;
}

static bool	apply_mouse_yaw(t_game *game, float rotation_amount)
{
	if (rotation_amount <= EPSILON && rotation_amount >= -EPSILON)
		return (false);
	game->cub_data.player.angle += rotation_amount;
	game->cub_data.player.angle = normalize_angle(game->cub_data.player.angle);
	return (true);
}

static bool	apply_mouse_pitch(t_game *game, float pitch_amount)
{
	float	max_pitch;

	if (pitch_amount <= EPSILON && pitch_amount >= -EPSILON)
		return (false);
	game->cub_data.player.pitch += pitch_amount;
	max_pitch = game->double_buffer[NEXT]->height * 0.35f;
	game->cub_data.player.pitch = clamp(game->cub_data.player.pitch,
			-max_pitch, max_pitch);
	return (true);
}

bool	process_mouse_rotation_bonus(t_game *game)
{
	float	rotation_amount;
	float	pitch_amount;
	bool	rotated;

	if (!game || is_config_modal_open(game))
		return (false);
	if (game->mouse_delta_accumulated == 0.0f
		&& game->mouse_delta_accumulated_y == 0.0f)
		return (false);
	rotation_amount = game->mouse_delta_accumulated;
	pitch_amount = game->mouse_delta_accumulated_y;
	game->mouse_delta_accumulated = 0.0f;
	game->mouse_delta_accumulated_y = 0.0f;
	rotated = false;
	if (apply_mouse_yaw(game, rotation_amount))
		rotated = true;
	if (apply_mouse_pitch(game, pitch_amount))
		rotated = true;
	return (rotated);
}
