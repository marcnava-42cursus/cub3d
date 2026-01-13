/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_visibility_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 15:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_modal_bonus.h"

static bool	ensure_config_modal_ready(t_game *game)
{
	if (!game || !game->mlx)
		return (false);
	game->config_quit_w = CONFIG_MODAL_QUIT_BTN_W;
	game->config_quit_h = CONFIG_MODAL_QUIT_BTN_H;
	game->config_quit_hover = false;
	game->config_quit_hold_time = 0.0;
	game->config_current_tab = 0;
	config_options_reset(game);
	if (!game->config_modal)
	{
		game->config_modal = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
				MAX_WINDOW_HEIGHT);
		if (!game->config_modal)
			return (false);
	}
	if (!game->config_modal_attached)
	{
		if (mlx_image_to_window(game->mlx, game->config_modal, 0, 0) < 0)
			return (false);
		game->config_modal_attached = true;
	}
	draw_modal_layout(game);
	return (true);
}

static void	clear_input_state(t_game *game)
{
	if (!game)
		return ;
	game->key_w_pressed = false;
	game->key_s_pressed = false;
	game->key_a_pressed = false;
	game->key_d_pressed = false;
	game->key_left_pressed = false;
	game->key_right_pressed = false;
	game->mouse_initialized = false;
	game->mouse_delta_accumulated = 0.0f;
}

void	set_config_modal_visible(t_game *game, bool visible)
{
	if (!game || !game->mlx)
		return ;
	if (visible)
	{
		if (!ensure_config_modal_ready(game))
			return ;
		set_image_enabled(game->config_modal, true);
		game->config_modal_open = true;
		clear_input_state(game);
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
		return ;
	}
	game->config_modal_open = false;
	clear_input_state(game);
	hide_settings_options(game);
	set_image_enabled(game->config_modal, false);
	set_image_enabled(game->config_quit_label, false);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
}

void	toggle_config_modal(t_game *game)
{
	if (!game)
		return ;
	set_config_modal_visible(game, !game->config_modal_open);
}

bool	is_config_modal_open(const t_game *game)
{
	if (!game)
		return (false);
	return (game->config_modal_open);
}
