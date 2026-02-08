/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_state_poll_bonus.c                      :+:      :+:    :+:   */
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

static void	controller_on_connection_change(t_game *game, int jid,
				int prev_jid, bool was_connected)
{
	if (!game)
		return ;
	if (!was_connected || jid != prev_jid)
	{
		game->controller.axis_calibrated = false;
		controller_reset_runtime_state(game);
	}
}

static int	controller_find_active_jid(t_game *game, GLFWgamepadstate *state)
{
	int	jid;

	jid = game->controller.gamepad_id;
	if (jid >= 0 && glfwJoystickPresent(jid)
		&& controller_fill_state_from_raw(game, jid, state))
		return (jid);
	jid = 0;
	while (jid <= GLFW_JOYSTICK_LAST)
	{
		if (glfwJoystickPresent(jid)
			&& controller_fill_state_from_raw(game, jid, state))
			return (jid);
		jid++;
	}
	return (-1);
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
	jid = controller_find_active_jid(game, state);
	if (jid >= 0)
	{
		controller_on_connection_change(game, jid, prev_jid, was_connected);
		return (true);
	}
	game->controller.connected = false;
	game->controller.gamepad_id = -1;
	game->controller.axis_calibrated = false;
	controller_reset_runtime_state(game);
	return (false);
}
