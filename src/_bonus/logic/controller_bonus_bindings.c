/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bonus_bindings.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 19:26:25 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"
#include <math.h>
#include <stdio.h>

static const char	*controller_bind_text(const t_controller_bind *bind,
		char *buffer, size_t size)
{
	if (!buffer || size == 0)
		return ("");
	buffer[0] = '\0';
	if (!bind || bind->type == CONTROLLER_BIND_NONE)
	{
		ft_strlcpy(buffer, "-", size);
		return (buffer);
	}
	if (bind->type == CONTROLLER_BIND_BUTTON)
	{
		snprintf(buffer, size, "B%d", bind->id);
		return (buffer);
	}
	if (bind->type == CONTROLLER_BIND_AXIS)
	{
		snprintf(buffer, size, "A%d%s", bind->id, (bind->dir < 0) ? "-" : "+");
		return (buffer);
	}
	ft_strlcpy(buffer, "-", size);
	return (buffer);
}

static void	controller_refresh_texts(t_game *game)
{
	int		i;
	char	buf[CONFIG_MODAL_KEY_LABEL_LEN];

	if (!game)
		return ;
	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		controller_bind_text(&game->controller.binds[i], buf, sizeof(buf));
		ft_strlcpy(game->menu.controls_controller_text[i], buf,
			CONFIG_MODAL_KEY_LABEL_LEN);
		i++;
	}
}

static t_controller_bind	controller_bind_button(int id)
{
	t_controller_bind	bind;

	bind.type = CONTROLLER_BIND_BUTTON;
	bind.id = id;
	bind.dir = 0;
	return (bind);
}

static t_controller_bind	controller_bind_axis(int id, int dir)
{
	t_controller_bind	bind;

	bind.type = CONTROLLER_BIND_AXIS;
	bind.id = id;
	bind.dir = dir;
	return (bind);
}

static void	controller_set_default_binds(t_game *game)
{
	game->controller.binds[ACTION_FORWARD] = controller_bind_axis(1, -1);
	game->controller.binds[ACTION_BACKWARD] = controller_bind_axis(1, 1);
	game->controller.binds[ACTION_STRAFE_RIGHT] = controller_bind_axis(0, 1);
	game->controller.binds[ACTION_STRAFE_LEFT] = controller_bind_axis(0, -1);
	game->controller.binds[ACTION_TURN_RIGHT] = controller_bind_axis(3, 1);
	game->controller.binds[ACTION_TURN_LEFT] = controller_bind_axis(3, -1);
	game->controller.binds[ACTION_LOOK_UP] = controller_bind_axis(4, -1);
	game->controller.binds[ACTION_LOOK_DOWN] = controller_bind_axis(4, 1);
	game->controller.binds[ACTION_BREAK] = controller_bind_axis(2, 1);
	game->controller.binds[ACTION_PLACE] = controller_bind_axis(5, 1);
	game->controller.binds[ACTION_MENU] = controller_bind_button(7);
	game->controller.binds[ACTION_MAP] = controller_bind_button(6);
	game->controller.binds[ACTION_ACCEPT] = controller_bind_button(0);
	game->controller.binds[ACTION_QUIT] = controller_bind_button(1);
}

static bool	controller_apply_rebind(t_game *game,
		t_controller_bind bind)
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

void	controller_init_advanced(t_game *game)
{
	if (!game)
		return ;
	ft_bzero(&game->controller, sizeof(game->controller));
	game->controller.gamepad_id = -1;
	game->controller.deadzone = CONTROLLER_DEADZONE;
	game->controller.menu_quit_held = false;
	controller_set_default_binds(game);
	controller_refresh_texts(game);
}

bool	controller_handle_rebind_advanced(t_game *game)
{
	GLFWgamepadstate	state;
	float				deadzone;
	float				value;
	t_controller_bind	bind;
	bool				found;
	int					i;

	if (!game || !game->menu.open)
		return (false);
	if (game->menu.current_tab != CONFIG_MENU_CONTROLLER_CONTROLS)
		return (false);
	if (!game->menu.controls_rebinding
		|| game->menu.controls_rebind_column != CONTROLS_COLUMN_CONTROLLER)
		return (false);
	if (!controller_poll_state(game, &state))
		return (false);
	if (!game->controller.axis_calibrated)
		controller_calibrate_axes(game, &state);
	deadzone = game->controller.deadzone;
	found = false;
	bind.type = CONTROLLER_BIND_NONE;
	bind.id = -1;
	bind.dir = 0;
	i = 0;
	while (i < CONTROLLER_BUTTON_COUNT)
	{
		if (state.buttons[i] == GLFW_PRESS
			&& !game->controller.prev_buttons[i])
		{
			bind = controller_bind_button(i);
			found = true;
			break ;
		}
		i++;
	}
	if (!found)
	{
		i = 0;
		while (i < CONTROLLER_AXIS_COUNT)
		{
			value = controller_axis_delta(game, &state, i);
			if (fabsf(value) > deadzone
				&& fabsf(game->controller.prev_axes[i]) <= deadzone)
			{
				bind = controller_bind_axis(i,
						(value >= 0.0f) ? 1 : -1);
				found = true;
				break ;
			}
			i++;
		}
	}
	if (found)
	{
		if (!controller_apply_rebind(game, bind))
		{
			controller_store_raw_state(game, &state);
			return (false);
		}
		i = 0;
		while (i < CONFIG_MODAL_CONTROL_COUNT)
		{
			game->controller.prev_action_active[i]
				= controller_action_active(game, i,
					&game->controller.binds[i], &state, deadzone);
			i++;
		}
		controller_store_raw_state(game, &state);
		return (true);
	}
	controller_store_raw_state(game, &state);
	return (false);
}
