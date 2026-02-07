/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bonus_menu.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 23:40:34 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

#define MENU_IN_UP 0
#define MENU_IN_DOWN 1
#define MENU_IN_LEFT 2
#define MENU_IN_RIGHT 3
#define MENU_IN_CONFIRM 4
#define MENU_IN_TAB_LEFT 5
#define MENU_IN_TAB_RIGHT 6
#define MENU_IN_COUNT 7

static void	sync_settings_selection(t_game *game)
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

static void	advance_menu_section(t_game *game, int delta)
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
	sync_settings_selection(game);
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
}

bool	controller_menu_hold_quit(t_game *game,
				const GLFWgamepadstate *state)
{
	if (!game || !state)
		return (false);
	return (controller_action_active(game,
			ACTION_QUIT, state, game->controller.deadzone));
}

static void	read_menu_inputs(t_game *game, const GLFWgamepadstate *state,
				bool in[MENU_IN_COUNT])
{
	in[MENU_IN_UP] = controller_action_pressed(game, state, ACTION_FORWARD)
		|| controller_button_pressed(game, state, GLFW_GAMEPAD_BUTTON_DPAD_UP);
	in[MENU_IN_DOWN] = controller_action_pressed(game, state, ACTION_BACKWARD)
		|| controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
	in[MENU_IN_LEFT] = controller_action_pressed(game, state,
			ACTION_STRAFE_LEFT)
		|| controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_DPAD_LEFT);
	in[MENU_IN_RIGHT] = controller_action_pressed(game, state,
			ACTION_STRAFE_RIGHT)
		|| controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);
}

static bool	handle_left_menu_column(t_game *game,
				const bool in[MENU_IN_COUNT])
{
	if (game->menu.current_column != CONFIG_MENU_COLUMN_LEFT)
		return (false);
	if (in[MENU_IN_UP])
		advance_menu_section(game, -1);
	else if (in[MENU_IN_DOWN])
		advance_menu_section(game, 1);
	if (in[MENU_IN_RIGHT] || in[MENU_IN_CONFIRM])
	{
		game->menu.current_column = CONFIG_MENU_COLUMN_RIGHT;
		draw_modal_layout(game);
	}
	return (true);
}

static bool	handle_controls_menu(t_game *game, bool up, bool down, bool confirm)
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

static void	handle_settings_menu(t_game *game,
				const bool in[MENU_IN_COUNT])
{
	if (in[MENU_IN_UP])
		config_option_select(game, -1);
	else if (in[MENU_IN_DOWN])
		config_option_select(game, 1);
	if (in[MENU_IN_RIGHT] && game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, 1);
	if (in[MENU_IN_TAB_LEFT] && game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, -1);
	else if (in[MENU_IN_TAB_RIGHT]
		&& game->menu.current_tab == CONFIG_MENU_TUNING)
		config_option_adjust(game, 1);
	if (in[MENU_IN_CONFIRM])
		config_option_toggle(game, game->menu.options.selected);
}

static void	read_menu_tab_buttons(t_game *game, const GLFWgamepadstate *state,
				bool in[MENU_IN_COUNT])
{
	in[MENU_IN_CONFIRM] = controller_action_pressed(game, state, ACTION_ACCEPT);
	in[MENU_IN_TAB_LEFT] = controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);
	in[MENU_IN_TAB_RIGHT] = controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
}

static bool	handle_column_back_to_left(t_game *game,
				const bool in[MENU_IN_COUNT])
{
	if (!in[MENU_IN_LEFT])
		return (false);
	game->menu.current_column = CONFIG_MENU_COLUMN_LEFT;
	draw_modal_layout(game);
	return (true);
}

void	controller_update_menu(t_game *game, const GLFWgamepadstate *state)
{
	bool	in[MENU_IN_COUNT];

	if (!game || !state)
		return ;
	ft_bzero(in, sizeof(in));
	read_menu_inputs(game, state, in);
	read_menu_tab_buttons(game, state, in);
	if (in[MENU_IN_TAB_LEFT])
		advance_menu_section(game, -1);
	else if (in[MENU_IN_TAB_RIGHT])
		advance_menu_section(game, 1);
	if (handle_left_menu_column(game, in))
		return ;
	if (handle_column_back_to_left(game, in))
		return ;
	if (handle_controls_menu(game, in[MENU_IN_UP], in[MENU_IN_DOWN],
			in[MENU_IN_CONFIRM]))
		return ;
	handle_settings_menu(game, in);
}
