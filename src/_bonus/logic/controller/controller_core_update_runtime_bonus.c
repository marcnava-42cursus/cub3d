/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_core_update_runtime_bonus.c             :+:      :+:    :+:   */
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

bool	controller_handle_modal_state(t_game *game,
				const GLFWgamepadstate *state,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	if (!is_config_modal_open(game))
		return (false);
	if (game->menu.controls_rebinding)
		game->controller.menu_quit_held = false;
	else
	{
		game->controller.menu_quit_held = controller_menu_hold_quit(
				game, state);
		controller_update_menu(game, state);
	}
	ft_memcpy(game->controller.prev_action_active,
		active, sizeof(bool) * CONFIG_MODAL_CONTROL_COUNT);
	controller_store_raw_state(game, state);
	return (true);
}

void	controller_handle_gameplay_actions(t_game *game,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	bool	break_pressed;
	bool	place_pressed;

	game->controller.menu_quit_held = false;
	break_pressed = active[ACTION_BREAK]
		&& !game->controller.prev_action_active[ACTION_BREAK];
	place_pressed = active[ACTION_PLACE]
		&& !game->controller.prev_action_active[ACTION_PLACE];
	if (break_pressed)
		test_break_wall_in_front(game);
	if (place_pressed)
		place_breakable_block(game);
}

void	controller_store_prev_actions(t_game *game,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	ft_memcpy(game->controller.prev_action_active, active, sizeof(bool)
		* CONFIG_MODAL_CONTROL_COUNT);
}

void	controller_try_toggle_menu(t_game *game,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	bool	allow_menu_toggle;
	bool	menu_pressed;

	allow_menu_toggle = true;
	if (game->menu.open && game->menu.controls_rebinding
		&& game->menu.controls_rebind_column == CONTROLS_COLUMN_CONTROLLER)
		allow_menu_toggle = false;
	menu_pressed = active[ACTION_MENU]
		&& !game->controller.prev_action_active[ACTION_MENU];
	if (menu_pressed && allow_menu_toggle)
		toggle_config_modal(game);
}
