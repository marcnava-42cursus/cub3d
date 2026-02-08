/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_events_nav_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 06:04:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 06:04:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

void	config_modal_sync_settings_selection_bonus(t_game *game)
{
	if (!game)
		return ;
	if (game->menu.current_tab == CONFIG_MENU_GENERAL
		&& game->menu.options.selected >= CONFIG_MODAL_TOGGLE_COUNT)
		game->menu.options.selected = CONFIG_OPTION_SHOW_FPS;
	else if (game->menu.current_tab == CONFIG_MENU_TUNING
		&& game->menu.options.selected < CONFIG_MODAL_TOGGLE_COUNT)
		game->menu.options.selected = CONFIG_OPTION_FPS_LIMIT;
}

void	config_modal_advance_section_bonus(t_game *game, int delta)
{
	int	next;

	if (!game)
		return ;
	next = game->menu.current_tab + delta;
	if (next < CONFIG_MENU_GENERAL)
		next = CONFIG_MENU_SECTION_COUNT - 1;
	else if (next >= CONFIG_MENU_SECTION_COUNT)
		next = CONFIG_MENU_GENERAL;
	if (next == game->menu.current_tab)
		return ;
	game->menu.current_tab = next;
	config_modal_sync_settings_selection_bonus(game);
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
}

bool	config_modal_handle_column_navigation_bonus(t_game *game,
			mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_LEFT
		&& game->menu.current_column == CONFIG_MENU_COLUMN_RIGHT)
	{
		config_controls_cancel_rebind(game);
		game->menu.current_column = CONFIG_MENU_COLUMN_LEFT;
		draw_modal_layout(game);
		return (true);
	}
	if (keydata.key == MLX_KEY_RIGHT
		&& game->menu.current_column == CONFIG_MENU_COLUMN_LEFT)
	{
		config_controls_cancel_rebind(game);
		game->menu.current_column = CONFIG_MENU_COLUMN_RIGHT;
		draw_modal_layout(game);
		return (true);
	}
	return (false);
}

bool	config_modal_handle_left_column_keys_bonus(t_game *game,
			bool confirm_pressed, mlx_key_data_t keydata)
{
	if (game->menu.current_column != CONFIG_MENU_COLUMN_LEFT)
		return (false);
	if (keydata.key == MLX_KEY_UP)
		config_modal_advance_section_bonus(game, -1);
	else if (keydata.key == MLX_KEY_DOWN)
		config_modal_advance_section_bonus(game, 1);
	else if (confirm_pressed)
	{
		game->menu.current_column = CONFIG_MENU_COLUMN_RIGHT;
		draw_modal_layout(game);
	}
	return (true);
}

bool	config_modal_handle_controls_tab_keys_bonus(t_game *game,
			bool confirm_pressed, mlx_key_data_t keydata)
{
	int	column;

	if (game->menu.current_tab != CONFIG_MENU_KEYBOARD_CONTROLS
		&& game->menu.current_tab != CONFIG_MENU_CONTROLLER_CONTROLS)
		return (false);
	if (game->menu.current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
		column = CONTROLS_COLUMN_CONTROLLER;
	else
		column = CONTROLS_COLUMN_KEYBOARD;
	game->menu.controls_column = column;
	if (keydata.key == MLX_KEY_UP)
		config_controls_select(game, -1);
	else if (keydata.key == MLX_KEY_DOWN)
		config_controls_select(game, 1);
	else if (confirm_pressed)
		config_controls_begin_rebind(game);
	return (true);
}
