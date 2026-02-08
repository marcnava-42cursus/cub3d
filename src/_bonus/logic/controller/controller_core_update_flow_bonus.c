/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_core_update_flow_bonus.c                :+:      :+:    :+:   */
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

static void	controller_reset_disconnected_state(t_game *game)
{
	if (!game)
		return ;
	game->controller.menu_quit_held = false;
	game->controller.move_forward = 0.0f;
	game->controller.move_strafe = 0.0f;
	game->controller.turn = 0.0f;
	game->controller.look = 0.0f;
	ft_bzero(game->controller.prev_action_active,
		sizeof(game->controller.prev_action_active));
}

static void	controller_collect_active(t_game *game,
	const GLFWgamepadstate *state, float deadzone,
	bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	int	i;

	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		active[i] = controller_action_active(game, i, state, deadzone);
		i++;
	}
}

static void	controller_apply_movement(t_game *game,
	const GLFWgamepadstate *state, float deadzone)
{
	game->controller.move_forward = controller_action_value(
			game, ACTION_FORWARD, state, deadzone) - controller_action_value(
			game, ACTION_BACKWARD, state, deadzone);
	game->controller.move_strafe = controller_action_value(
			game, ACTION_STRAFE_RIGHT, state, deadzone)
		- controller_action_value(game, ACTION_STRAFE_LEFT,
			state, deadzone);
	game->controller.turn = controller_action_value(
			game, ACTION_TURN_RIGHT, state, deadzone) - controller_action_value(
			game, ACTION_TURN_LEFT, state, deadzone);
	game->controller.look = controller_action_value(
			game, ACTION_LOOK_UP, state, deadzone) - controller_action_value(
			game, ACTION_LOOK_DOWN, state, deadzone);
}

bool	controller_prepare_update(t_game *game, GLFWgamepadstate *state,
				bool active[CONFIG_MODAL_CONTROL_COUNT], float *deadzone)
{
	if (!controller_poll_state(game, state))
	{
		controller_reset_disconnected_state(game);
		return (false);
	}
	if (!game->controller.axis_calibrated)
		controller_calibrate_axes(game, state);
	*deadzone = game->controller.deadzone;
	controller_collect_active(game, state, *deadzone, active);
	controller_apply_movement(game, state, *deadzone);
	return (true);
}

void	controller_update_bonus(t_game *game)
{
	GLFWgamepadstate	state;
	bool				active[CONFIG_MODAL_CONTROL_COUNT];
	float				deadzone;

	if (!game)
		return ;
	if (!controller_prepare_update(game, &state, active, &deadzone))
		return ;
	(void)deadzone;
	controller_try_toggle_menu(game, active);
	if (controller_handle_modal_state(game, &state, active))
		return ;
	controller_handle_gameplay_actions(game, active);
	controller_store_prev_actions(game, active);
	controller_store_raw_state(game, &state);
}
