/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 22:46:31 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"
#include "render.h"

static void	clear_movement_keys(t_game *game)
{
	if (!game)
		return ;
	game->key_w_pressed = false;
	game->key_s_pressed = false;
	game->key_a_pressed = false;
	game->key_d_pressed = false;
	game->key_left_pressed = false;
	game->key_right_pressed = false;
	game->key_up_pressed = false;
	game->key_down_pressed = false;
}

static void	update_movement_keys(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	game->key_w_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_FORWARD]);
	game->key_s_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_BACKWARD]);
	game->key_d_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_STRAFE_RIGHT]);
	game->key_a_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_STRAFE_LEFT]);
	game->key_right_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_TURN_RIGHT]);
	game->key_left_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_TURN_LEFT]);
	game->key_up_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_LOOK_UP]);
	game->key_down_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_LOOK_DOWN]);
}

void	refresh_key_states_advanced(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	if (is_config_modal_open(game))
	{
		clear_movement_keys(game);
		return ;
	}
	update_movement_keys(game);
	controller_update_advanced(game);
}

static bool	handle_modal_key_input(t_game *game, mlx_key_data_t keydata)
{
	keys_t	menu_key;

	if (!game)
		return (true);
	if (is_config_modal_open(game) && game->menu.controls_rebinding)
	{
		config_modal_handle_key(game, keydata);
		return (true);
	}
	menu_key = game->menu.controls_key_codes[ACTION_MENU];
	if (keydata.key == menu_key && keydata.action == MLX_PRESS)
	{
		toggle_config_modal(game);
		return (true);
	}
	if (is_config_modal_open(game))
	{
		config_modal_handle_key(game, keydata);
		return (true);
	}
	return (false);
}

static void	handle_place_break_input(t_game *game, mlx_key_data_t keydata)
{
	keys_t	break_key;
	keys_t	place_key;

	place_key = game->menu.controls_key_codes[ACTION_PLACE];
	if (keydata.key == place_key && keydata.action == MLX_PRESS)
	{
		place_breakable_block(game);
		return ;
	}
	break_key = game->menu.controls_key_codes[ACTION_BREAK];
	if (keydata.key == break_key && keydata.action == MLX_PRESS)
		test_break_wall_in_front(game);
}

void	key_hook_advanced(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	/* keys_t	map_key; */
	game = (t_game *)param;
	if (!game)
		return ;
	if (handle_modal_key_input(game, keydata))
		return ;
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
	{
		trigger_jump_advanced(game);
		return ;
	}
	/*
	map_key = game->menu.controls_key_codes[ACTION_MAP];
	if (keydata.key == map_key && keydata.action == MLX_PRESS)
	{
		toggle_map_overlay_advanced(game);
		return ;
	}
	*/
	handle_place_break_input(game, keydata);
}

void	mouse_hook_advanced(mouse_key_t button, action_t action,
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

void	cursor_hook_advanced(double xpos, double ypos, void *param)
{
	t_game		*game;
	double		delta_x;
	double		delta_y;

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

bool	process_mouse_rotation_advanced(t_game *game)
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
