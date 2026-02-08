/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_menu_update_bonus.c                     :+:      :+:    :+:   */
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

#define MENU_IN_UP 0
#define MENU_IN_DOWN 1
#define MENU_IN_LEFT 2
#define MENU_IN_RIGHT 3
#define MENU_IN_CONFIRM 4
#define MENU_IN_TAB_LEFT 5
#define MENU_IN_TAB_RIGHT 6
#define MENU_IN_COUNT 7

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
		controller_menu_advance_section(game, -1);
	else if (in[MENU_IN_TAB_RIGHT])
		controller_menu_advance_section(game, 1);
	if (controller_menu_handle_left_column(game, in))
		return ;
	if (handle_column_back_to_left(game, in))
		return ;
	if (controller_menu_handle_controls(game, in[MENU_IN_UP], in[MENU_IN_DOWN],
			in[MENU_IN_CONFIRM]))
		return ;
	controller_menu_handle_settings(game, in);
}
