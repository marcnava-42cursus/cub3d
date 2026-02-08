/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bindings_detect_bonus.c                 :+:      :+:    :+:   */
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

#include <math.h>

bool	controller_detect_pressed_button(t_game *game,
			const GLFWgamepadstate *state, t_controller_bind *bind)
{
	int	i;

	i = 0;
	while (i < CONTROLLER_BUTTON_COUNT)
	{
		if (state->buttons[i] == GLFW_PRESS
			&& !game->controller.prev_buttons[i])
		{
			*bind = controller_bind_button(i);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	controller_detect_pressed_axis(t_game *game,
			const GLFWgamepadstate *state, float deadzone,
			t_controller_bind *bind)
{
	float	value;
	int		i;

	i = 0;
	while (i < CONTROLLER_AXIS_COUNT)
	{
		value = controller_axis_delta(game, state, i);
		if (fabsf(value) > deadzone && fabsf(game->controller.prev_axes[i])
			<= deadzone)
		{
			if (value >= 0.0f)
				*bind = controller_bind_axis(i, 1);
			else
				*bind = controller_bind_axis(i, -1);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	controller_find_new_bind(t_game *game,
			const GLFWgamepadstate *state, float deadzone,
			t_controller_bind *bind)
{
	bind->type = CONTROLLER_BIND_NONE;
	bind->id = -1;
	bind->dir = 0;
	if (controller_detect_pressed_button(game, state, bind))
		return (true);
	return (controller_detect_pressed_axis(game, state, deadzone, bind));
}
