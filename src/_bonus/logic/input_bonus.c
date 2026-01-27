/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/23 01:25:48 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic.h"
#include "render.h"

void	refresh_key_states_bonus(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	if (is_config_modal_open(game))
	{
		game->key_w_pressed = false;
		game->key_s_pressed = false;
		game->key_a_pressed = false;
		game->key_d_pressed = false;
		game->key_left_pressed = false;
		game->key_right_pressed = false;
		return ;
	}
	game->key_w_pressed = mlx_is_key_down(game->mlx, MLX_KEY_W);
	game->key_s_pressed = mlx_is_key_down(game->mlx, MLX_KEY_S);
	game->key_a_pressed = mlx_is_key_down(game->mlx, MLX_KEY_A);
	game->key_d_pressed = mlx_is_key_down(game->mlx, MLX_KEY_D);
	game->key_left_pressed = mlx_is_key_down(game->mlx, MLX_KEY_LEFT);
	game->key_right_pressed = mlx_is_key_down(game->mlx, MLX_KEY_RIGHT);
	game->key_up_pressed = mlx_is_key_down(game->mlx, MLX_KEY_UP);
	game->key_down_pressed = mlx_is_key_down(game->mlx, MLX_KEY_DOWN);
}

void	key_hook_bonus(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (!game)
		return ;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		toggle_config_modal(game);
		return ;
	}
	if (is_config_modal_open(game))
	{
		config_modal_handle_key(game, keydata);
		return ;
	}
	if (keydata.key == MLX_KEY_R && keydata.action == MLX_PRESS)
		place_breakable_block(game);
	else if (keydata.key == MLX_KEY_E && keydata.action == MLX_PRESS)
		test_break_wall_in_front(game);
}

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

	(void)ypos;
	game = (t_game *)param;
	if (!game)
		return ;
	if (is_config_modal_open(game))
		return ;
	if (!game->mlx)
		return ;
	if (!game->mouse_initialized)
	{
		game->last_mouse_x = xpos;
		game->mouse_initialized = true;
		return ;
	}
	delta_x = xpos - game->last_mouse_x;
	game->last_mouse_x = xpos;
	game->mouse_delta_accumulated += (float)delta_x * game->mouse_sensitivity;
}

bool	process_mouse_rotation_bonus(t_game *game)
{
	float	rotation_amount;

	if (is_config_modal_open(game) || !game)
		return (false);
	if (game->mouse_delta_accumulated == 0.0f)
		return (false);
	rotation_amount = game->mouse_delta_accumulated;
	game->mouse_delta_accumulated = 0.0f;
	if (rotation_amount > -EPSILON && rotation_amount < EPSILON)
	{
		game->mouse_delta_accumulated = 0.0f;
		return (false);
	}
	game->cub_data.player.angle += rotation_amount;
	game->cub_data.player.angle = normalize_angle(game->cub_data.player.angle);
	return (true);
}
