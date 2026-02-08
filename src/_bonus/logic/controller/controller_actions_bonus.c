/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_actions_bonus.c                         :+:      :+:    :+:   */
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

bool	controller_action_active(t_game *game, int action,
			const GLFWgamepadstate *state, float deadzone)
{
	const t_controller_bind	*bind;
	float					value;

	if (!game || !state)
		return (false);
	if (action < 0 || action >= CONFIG_MODAL_CONTROL_COUNT)
		return (false);
	bind = &game->controller.binds[action];
	if (bind->type == CONTROLLER_BIND_BUTTON)
	{
		if (bind->id < 0 || bind->id >= CONTROLLER_BUTTON_COUNT)
			return (false);
		return (state->buttons[bind->id] == GLFW_PRESS);
	}
	if (bind->type == CONTROLLER_BIND_AXIS)
	{
		if (bind->id < 0 || bind->id >= CONTROLLER_AXIS_COUNT)
			return (false);
		value = controller_axis_delta(game, state, bind->id);
		if (bind->dir < 0)
			return (value < -deadzone);
		if (bind->dir > 0)
			return (value > deadzone);
	}
	return (false);
}

bool	controller_action_pressed(t_game *game,
			const GLFWgamepadstate *state, int action)
{
	const t_controller_bind	*bind;

	if (!game || !state)
		return (false);
	if (action < 0 || action >= CONFIG_MODAL_CONTROL_COUNT)
		return (false);
	bind = &game->controller.binds[action];
	if (bind->type == CONTROLLER_BIND_BUTTON)
	{
		if (bind->id < 0 || bind->id >= CONTROLLER_BUTTON_COUNT)
			return (false);
		return (state->buttons[bind->id] == GLFW_PRESS
			&& !game->controller.prev_buttons[bind->id]);
	}
	if (bind->type == CONTROLLER_BIND_AXIS)
	{
		if (bind->id < 0 || bind->id >= CONTROLLER_AXIS_COUNT)
			return (false);
		return (controller_action_active(game, action, state,
				game->controller.deadzone)
			&& !game->controller.prev_action_active[action]);
	}
	return (false);
}

bool	controller_button_pressed(t_game *game,
			const GLFWgamepadstate *state, int button)
{
	if (!game || !state)
		return (false);
	if (button < 0 || button >= CONTROLLER_BUTTON_COUNT)
		return (false);
	return (state->buttons[button] == GLFW_PRESS
		&& !game->controller.prev_buttons[button]);
}
