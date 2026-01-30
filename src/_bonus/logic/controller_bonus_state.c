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

static bool	controller_fill_state_from_raw(t_game *game, int jid,
			GLFWgamepadstate *state)
{
	const unsigned char	*buttons;
	const float			*axes;
	int					button_count;
	int					axis_count;
	int					limit;
	int					i;

	if (!game || !state)
		return (false);
	button_count = 0;
	axis_count = 0;
	buttons = glfwGetJoystickButtons(jid, &button_count);
	axes = glfwGetJoystickAxes(jid, &axis_count);
	if ((!buttons || button_count <= 0) && (!axes || axis_count <= 0))
		return (false);
	ft_bzero(state, sizeof(*state));
	if (buttons && button_count > 0)
	{
		limit = button_count;
		if (limit > CONTROLLER_BUTTON_COUNT)
			limit = CONTROLLER_BUTTON_COUNT;
		i = 0;
		while (i < limit)
		{
			state->buttons[i] = (buttons[i] == GLFW_PRESS);
			i++;
		}
	}
	if (axes && axis_count > 0)
	{
		limit = axis_count;
		if (limit > CONTROLLER_AXIS_COUNT)
			limit = CONTROLLER_AXIS_COUNT;
		i = 0;
		while (i < limit)
		{
			state->axes[i] = axes[i];
			i++;
		}
	}
	game->controller.connected = true;
	game->controller.gamepad_id = jid;
	return (true);
}

bool	controller_poll_state(t_game *game, GLFWgamepadstate *state)
{
	int	jid;

	if (!game || !state)
		return (false);
	jid = game->controller.gamepad_id;
	if (jid >= 0 && glfwJoystickPresent(jid))
	{
		if (controller_fill_state_from_raw(game, jid, state))
			return (true);
	}
	jid = 0;
	while (jid <= GLFW_JOYSTICK_LAST)
	{
		if (glfwJoystickPresent(jid))
		{
			if (controller_fill_state_from_raw(game, jid, state))
				return (true);
		}
		jid++;
	}
	game->controller.connected = false;
	game->controller.gamepad_id = -1;
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
