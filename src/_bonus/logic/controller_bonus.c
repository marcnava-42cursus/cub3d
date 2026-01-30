/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bonus.c                                 :+:      :+:    :+:   */
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

void	controller_update_bonus(t_game *game)
{
	GLFWgamepadstate	state;
	bool				active[CONFIG_MODAL_CONTROL_COUNT];
	bool				break_pressed;
	bool				place_pressed;
	bool				menu_pressed;
	bool				map_pressed;
	bool				allow_menu_toggle;
	float				deadzone;
	int					i;

	if (!game)
		return ;
	if (!controller_poll_state(game, &state))
	{
		game->controller.menu_quit_held = false;
		return ;
	}
	if (!game->controller.axis_calibrated)
		controller_calibrate_axes(game, &state);
	deadzone = game->controller.deadzone;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		active[i] = controller_action_active(game, i,
				&game->controller.binds[i], &state, deadzone);
		i++;
	}
	allow_menu_toggle = true;
	if (game->menu.open && game->menu.controls_rebinding
		&& game->menu.controls_rebind_column == CONTROLS_COLUMN_CONTROLLER)
		allow_menu_toggle = false;
	menu_pressed = active[ACTION_MENU]
		&& !game->controller.prev_action_active[ACTION_MENU];
	if (menu_pressed && allow_menu_toggle)
		toggle_config_modal(game);
	if (is_config_modal_open(game))
	{
		if (game->menu.controls_rebinding)
			game->controller.menu_quit_held = false;
		else
		{
			game->controller.menu_quit_held = controller_menu_hold_quit(game,
					&state);
			controller_update_menu(game, &state);
		}
		ft_memcpy(game->controller.prev_action_active, active,
			sizeof(active));
		controller_store_raw_state(game, &state);
		return ;
	}
	game->controller.menu_quit_held = false;
	if (active[ACTION_FORWARD])
		game->key_w_pressed = true;
	if (active[ACTION_BACKWARD])
		game->key_s_pressed = true;
	if (active[ACTION_STRAFE_RIGHT])
		game->key_d_pressed = true;
	if (active[ACTION_STRAFE_LEFT])
		game->key_a_pressed = true;
	if (active[ACTION_TURN_RIGHT])
		game->key_right_pressed = true;
	if (active[ACTION_TURN_LEFT])
		game->key_left_pressed = true;
	if (active[ACTION_LOOK_UP])
		game->key_up_pressed = true;
	if (active[ACTION_LOOK_DOWN])
		game->key_down_pressed = true;
	break_pressed = active[ACTION_BREAK]
		&& !game->controller.prev_action_active[ACTION_BREAK];
	place_pressed = active[ACTION_PLACE]
		&& !game->controller.prev_action_active[ACTION_PLACE];
	map_pressed = active[ACTION_MAP]
		&& !game->controller.prev_action_active[ACTION_MAP];
	if (break_pressed)
		test_break_wall_in_front(game);
	if (place_pressed)
		place_breakable_block(game);
	if (map_pressed)
		toggle_map_overlay_bonus(game);
	ft_memcpy(game->controller.prev_action_active, active, sizeof(active));
	controller_store_raw_state(game, &state);
}
