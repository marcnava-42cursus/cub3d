/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bonus_state.c                           :+:      :+:    :+:   */
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

static void	controller_reset_runtime_state(t_game *game)
{
	if (!game)
		return ;
	game->controller.move_forward = 0.0f;
	game->controller.move_strafe = 0.0f;
	game->controller.turn = 0.0f;
	game->controller.look = 0.0f;
	game->controller.menu_quit_held = false;
	ft_bzero(game->controller.prev_buttons,
		sizeof(game->controller.prev_buttons));
	ft_bzero(game->controller.prev_axes, sizeof(game->controller.prev_axes));
	ft_bzero(game->controller.prev_action_active,
		sizeof(game->controller.prev_action_active));
}

static bool	controller_fill_state_from_raw(t_game *game, int jid,
			GLFWgamepadstate *state)
{
	if (!game || !state)
		return (false);
	if (!glfwJoystickIsGamepad(jid))
		return (false);
	if (!glfwGetGamepadState(jid, state))
		return (false);
	game->controller.connected = true;
	game->controller.gamepad_id = jid;
	return (true);
}

bool	controller_poll_state(t_game *game, GLFWgamepadstate *state)
{
	int		jid;
	int		prev_jid;
	bool	was_connected;

	if (!game || !state)
		return (false);
	prev_jid = game->controller.gamepad_id;
	was_connected = game->controller.connected;
	jid = game->controller.gamepad_id;
	if (jid >= 0 && glfwJoystickPresent(jid))
	{
		if (controller_fill_state_from_raw(game, jid, state))
		{
			if (!was_connected || jid != prev_jid)
			{
				game->controller.axis_calibrated = false;
				controller_reset_runtime_state(game);
			}
			return (true);
		}
	}
	jid = 0;
	while (jid <= GLFW_JOYSTICK_LAST)
	{
		if (glfwJoystickPresent(jid))
		{
			if (controller_fill_state_from_raw(game, jid, state))
			{
				if (!was_connected || jid != prev_jid)
				{
					game->controller.axis_calibrated = false;
					controller_reset_runtime_state(game);
				}
				return (true);
			}
		}
		jid++;
	}
	game->controller.connected = false;
	game->controller.gamepad_id = -1;
	game->controller.axis_calibrated = false;
	controller_reset_runtime_state(game);
	return (false);
}

void	controller_calibrate_axes(t_game *game,
			const GLFWgamepadstate *state)
{
	int	i;

	if (!game || !state)
		return ;
	i = 0;
	while (i < CONTROLLER_AXIS_COUNT)
	{
		game->controller.axis_center[i] = state->axes[i];
		i++;
	}
	game->controller.axis_calibrated = true;
}

float	controller_axis_delta(const t_game *game,
			const GLFWgamepadstate *state, int axis)
{
	float	value;

	if (!game || !state || axis < 0 || axis >= CONTROLLER_AXIS_COUNT)
		return (0.0f);
	value = state->axes[axis];
	if (game->controller.axis_calibrated)
		value -= game->controller.axis_center[axis];
	return (value);
}

void	controller_store_raw_state(t_game *game,
			const GLFWgamepadstate *state)
{
	int	i;

	if (!game || !state)
		return ;
	i = 0;
	while (i < CONTROLLER_BUTTON_COUNT)
	{
		game->controller.prev_buttons[i] = (state->buttons[i] == GLFW_PRESS);
		i++;
	}
	i = 0;
	while (i < CONTROLLER_AXIS_COUNT)
	{
		game->controller.prev_axes[i] = controller_axis_delta(game, state, i);
		i++;
	}
}
