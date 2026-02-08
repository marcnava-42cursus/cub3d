/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bonus_bindings.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 02:48:22 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"
#include <math.h>

static void	write_prefixed_int(char *buffer, size_t size,
		const char *prefix, int value)
{
	char	*num;

	ft_strlcpy(buffer, prefix, size);
	num = ft_itoa(value);
	if (!num)
		return ;
	ft_strlcat(buffer, num, size);
	free(num);
}

static void	append_axis_sign(char *buffer, size_t size, int dir)
{
	if (dir < 0)
		ft_strlcat(buffer, "-", size);
	else
		ft_strlcat(buffer, "+", size);
}

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
		write_prefixed_int(buffer, size, "B", bind->id);
		return (buffer);
	}
	if (bind->type == CONTROLLER_BIND_AXIS)
	{
		write_prefixed_int(buffer, size, "A", bind->id);
		append_axis_sign(buffer, size, bind->dir);
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

void	controller_init_bonus(t_game *game)
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

static bool	controller_detect_pressed_button(t_game *game,
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

static bool	controller_detect_pressed_axis(t_game *game,
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

static bool	controller_find_new_bind(t_game *game,
	const GLFWgamepadstate *state, float deadzone, t_controller_bind *bind)
{
	bind->type = CONTROLLER_BIND_NONE;
	bind->id = -1;
	bind->dir = 0;
	if (controller_detect_pressed_button(game, state, bind))
		return (true);
	return (controller_detect_pressed_axis(game, state, deadzone, bind));
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
