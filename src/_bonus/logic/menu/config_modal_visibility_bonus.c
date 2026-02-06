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

#include "config_bonus.h"
#include "render.h"
#include "structs.h"
#include "logic_bonus.h"

static bool	ensure_config_modal_ready(t_game *game)
{
	if (!game || !game->mlx)
		return (false);
	game->menu.quit_w = CONFIG_MODAL_QUIT_BTN_W;
	game->menu.quit_h = CONFIG_MODAL_QUIT_BTN_H;
	game->menu.quit_hover = false;
	game->menu.quit_hold_time = 0.0;
	game->menu.current_tab = CONFIG_MENU_GENERAL;
	game->menu.current_column = CONFIG_MENU_COLUMN_LEFT;
	config_options_reset(game);
	if (!game->menu.modal)
	{
		game->menu.modal = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
				MAX_WINDOW_HEIGHT);
		if (!game->menu.modal)
			return (false);
	}
	if (!game->menu.attached)
	{
		if (mlx_image_to_window(game->mlx, game->menu.modal, 0, 0) < 0)
			return (false);
		game->menu.attached = true;
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
	game->mouse_delta_accumulated_y = 0.0f;
	game->last_mouse_x = 0.0;
	game->last_mouse_y = 0.0;
}

void	set_config_modal_visible(t_game *game, bool visible)
{
	if (!game || !game->mlx)
		return ;
	if (visible)
	{
		if (!ensure_config_modal_ready(game))
			return ;
		set_image_enabled(game->menu.modal, true);
		game->menu.open = true;
		clear_input_state(game);
		mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
		return ;
	}
	game->menu.open = false;
	clear_input_state(game);
	disable_label_group(game->menu.labels.menu_entries,
		CONFIG_MENU_SECTION_COUNT);
	hide_settings_options(game);
	hide_controls_options(game);
	config_controls_cancel_rebind(game);
	set_image_enabled(game->menu.modal, false);
	set_image_enabled(game->menu.quit_label, false);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
}

void	toggle_config_modal(t_game *game)
{
	if (!game)
		return ;
	set_config_modal_visible(game, !game->menu.open);
}

bool	is_config_modal_open(const t_game *game)
{
	if (!game)
		return (false);
	return (game->menu.open);
}
