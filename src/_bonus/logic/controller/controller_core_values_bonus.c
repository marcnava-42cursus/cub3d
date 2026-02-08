/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_core_values_bonus.c                     :+:      :+:    :+:   */
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

static float	clamp_axis_value(float value, float deadzone)
{
	if (value > 1.0f)
		value = 1.0f;
	if (value < -1.0f)
		value = -1.0f;
	if (value > deadzone)
		return ((value - deadzone) / (1.0f - deadzone));
	if (value < -deadzone)
		return ((value + deadzone) / (1.0f - deadzone));
	return (0.0f);
}

static float	controller_axis_value(t_game *game,
	const GLFWgamepadstate *state, t_controller_bind bind, float deadzone)
{
	float	value;

	if (bind.id < 0 || bind.id >= CONTROLLER_AXIS_COUNT || bind.dir == 0)
		return (0.0f);
	value = controller_axis_delta(game, state, bind.id);
	if (bind.dir < 0)
		value = -value;
	if (value <= deadzone)
		return (0.0f);
	value = clamp_axis_value(value, deadzone);
	if (value < 0.0f)
		return (0.0f);
	return (value);
}

float	controller_action_value(t_game *game, int action,
			const GLFWgamepadstate *state, float deadzone)
{
	t_controller_bind	bind;

	if (!game || !state)
		return (0.0f);
	if (action < 0 || action >= CONFIG_MODAL_CONTROL_COUNT)
		return (0.0f);
	bind = game->controller.binds[action];
	if (bind.type == CONTROLLER_BIND_BUTTON)
	{
		if (bind.id < 0 || bind.id >= CONTROLLER_BUTTON_COUNT)
			return (0.0f);
		return (state->buttons[bind.id] == GLFW_PRESS);
	}
	if (bind.type == CONTROLLER_BIND_AXIS)
		return (controller_axis_value(game, state, bind, deadzone));
	return (0.0f);
}
