/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_state_axes_bonus.c                      :+:      :+:    :+:   */
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
