/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bindings_rebind_bonus.c                 :+:      :+:    :+:   */
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

static bool	controller_rebind_mode_active(t_game *game)
{
	if (!game || !game->menu.open)
		return (false);
	if (game->menu.current_tab != CONFIG_MENU_CONTROLLER_CONTROLS)
		return (false);
	if (!game->menu.controls_rebinding
		|| game->menu.controls_rebind_column != CONTROLS_COLUMN_CONTROLLER)
		return (false);
	return (true);
}

static bool	controller_apply_rebind(t_game *game, t_controller_bind bind)
{
	int	action;

	if (!game)
		return (false);
	action = game->menu.controls_rebind_target;
	if (action < 0 || action >= CONFIG_MODAL_CONTROL_COUNT)
		return (false);
	game->controller.binds[action] = bind;
	controller_refresh_texts(game);
	config_controls_cancel_rebind(game);
	draw_modal_layout(game);
	return (true);
}

static void	controller_refresh_prev_actions(t_game *game,
			const GLFWgamepadstate *state, float deadzone)
{
	int	i;

	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		game->controller.prev_action_active[i] = controller_action_active(
				game, i, state, deadzone);
		i++;
	}
}

static bool	controller_finalize_rebind(t_game *game,
	const GLFWgamepadstate *state, float deadzone, t_controller_bind bind)
{
	if (!controller_apply_rebind(game, bind))
	{
		controller_store_raw_state(game, state);
		return (false);
	}
	controller_refresh_prev_actions(game, state, deadzone);
	controller_store_raw_state(game, state);
	return (true);
}

bool	controller_handle_rebind_bonus(t_game *game)
{
	GLFWgamepadstate	state;
	float				deadzone;
	t_controller_bind	bind;

	if (!controller_rebind_mode_active(game))
		return (false);
	if (!controller_poll_state(game, &state))
		return (false);
	if (!game->controller.axis_calibrated)
		controller_calibrate_axes(game, &state);
	deadzone = game->controller.deadzone;
	if (!controller_find_new_bind(game, &state, deadzone, &bind))
	{
		controller_store_raw_state(game, &state);
		return (false);
	}
	return (controller_finalize_rebind(game, &state, deadzone, bind));
}
