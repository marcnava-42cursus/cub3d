/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_menu_navigation_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 04:35:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 04:35:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

void	controller_menu_sync_settings_selection(t_game *game)
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

void	controller_menu_advance_section(t_game *game, int delta)
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
	controller_menu_sync_settings_selection(game);
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
}

bool	controller_menu_handle_left_column(t_game *game, const bool in[7])
{
	if (game->menu.current_column != CONFIG_MENU_COLUMN_LEFT)
		return (false);
	if (in[0])
		controller_menu_advance_section(game, -1);
	else if (in[1])
		controller_menu_advance_section(game, 1);
	if (in[3] || in[4])
	{
		game->menu.current_column = CONFIG_MENU_COLUMN_RIGHT;
		draw_modal_layout(game);
	}
	return (true);
}

bool	controller_menu_handle_controls(t_game *game, bool up, bool down,
			bool confirm)
{
	if (game->menu.current_tab != CONFIG_MENU_KEYBOARD_CONTROLS
		&& game->menu.current_tab != CONFIG_MENU_CONTROLLER_CONTROLS)
		return (false);
	if (game->menu.current_tab == CONFIG_MENU_CONTROLLER_CONTROLS)
		game->menu.controls_column = CONTROLS_COLUMN_CONTROLLER;
	else
		game->menu.controls_column = CONTROLS_COLUMN_KEYBOARD;
	if (up)
		config_controls_select(game, -1);
	else if (down)
		config_controls_select(game, 1);
	if (confirm)
		config_controls_begin_rebind(game);
	return (true);
}

void	controller_menu_handle_settings(t_game *game, const bool in[7])
{
	if (in[0])
		config_option_select(game, -1);
	else if (in[1])
		config_option_select(game, 1);
	if (in[3] && game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, 1);
	if (in[5] && game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, -1);
	else if (in[6] && game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, 1);
	if (in[4])
		config_option_toggle(game, game->menu.options.selected);
}
