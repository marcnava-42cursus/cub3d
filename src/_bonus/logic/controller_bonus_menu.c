/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bonus_menu.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/29 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

bool	controller_menu_hold_quit(t_game *game,
			const GLFWgamepadstate *state)
{
	if (!game || !state)
		return (false);
	return (controller_action_active(game,
			ACTION_QUIT, &game->controller.binds[ACTION_QUIT],
			state, game->controller.deadzone));
}

void	controller_update_menu(t_game *game,
			const GLFWgamepadstate *state)
{
	bool	up;
	bool	down;
	bool	left;
	bool	right;
	bool	confirm;
	bool	tab_left;
	bool	tab_right;

	if (!game || !state)
		return ;
	up = controller_action_pressed(game, state, ACTION_FORWARD)
		|| controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_DPAD_UP);
	down = controller_action_pressed(game, state, ACTION_BACKWARD)
		|| controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
	left = controller_action_pressed(game, state, ACTION_STRAFE_LEFT)
		|| controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_DPAD_LEFT);
	right = controller_action_pressed(game, state, ACTION_STRAFE_RIGHT)
		|| controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);
	confirm = controller_action_pressed(game, state, ACTION_ACCEPT);
	tab_left = controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);
	tab_right = controller_button_pressed(game, state,
			GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
	if (tab_left && game->menu.current_tab > 0)
	{
		game->menu.current_tab--;
		config_controls_cancel_rebind(game);
		draw_modal_layout(game);
	}
	else if (tab_right && game->menu.current_tab < 1)
	{
		game->menu.current_tab++;
		config_controls_cancel_rebind(game);
		draw_modal_layout(game);
	}
	if (game->menu.current_tab != 0)
	{
		if (up)
			config_controls_select(game, -1);
		else if (down)
			config_controls_select(game, 1);
		if (left)
			config_controls_set_column(game, -1);
		else if (right)
			config_controls_set_column(game, 1);
		if (confirm)
			config_controls_begin_rebind(game);
		return ;
	}
	if (up)
		config_option_select(game, -1);
	else if (down)
		config_option_select(game, 1);
	if (left)
		config_option_adjust(game, -1);
	else if (right)
		config_option_adjust(game, 1);
	if (confirm)
		config_option_toggle(game, game->menu.options.selected);
}
