/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 22:47:29 by marcnava         ###   ########.fr       */
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

static float	controller_axis_value(t_game *game, const GLFWgamepadstate *state,
				t_controller_bind bind, float deadzone)
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

static float	controller_action_value(t_game *game, int action,
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

static void	controller_collect_active(t_game *game, const GLFWgamepadstate *state,
				float deadzone, bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	int	i;

	i = 0;
	while (i < CONFIG_MODAL_CONTROL_COUNT)
	{
		active[i] = controller_action_active(game, i,
				&game->controller.binds[i], state, deadzone);
		i++;
	}
}

static void	controller_apply_movement(t_game *game, const GLFWgamepadstate *state,
				float deadzone)
{
	game->controller.move_forward = controller_action_value(game, ACTION_FORWARD,
			state, deadzone) - controller_action_value(game, ACTION_BACKWARD,
			state, deadzone);
	game->controller.move_strafe = controller_action_value(game,
			ACTION_STRAFE_RIGHT, state, deadzone)
		- controller_action_value(game, ACTION_STRAFE_LEFT, state, deadzone);
	game->controller.turn = controller_action_value(game, ACTION_TURN_RIGHT,
			state, deadzone) - controller_action_value(game, ACTION_TURN_LEFT,
			state, deadzone);
	game->controller.look = controller_action_value(game, ACTION_LOOK_UP,
			state, deadzone) - controller_action_value(game, ACTION_LOOK_DOWN,
			state, deadzone);
}

static bool	controller_handle_modal_state(t_game *game,
				const GLFWgamepadstate *state,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	if (!is_config_modal_open(game))
		return (false);
	if (game->menu.controls_rebinding)
		game->controller.menu_quit_held = false;
	else
	{
		game->controller.menu_quit_held = controller_menu_hold_quit(game, state);
		controller_update_menu(game, state);
	}
	ft_memcpy(game->controller.prev_action_active, active, sizeof(bool)
		* CONFIG_MODAL_CONTROL_COUNT);
	controller_store_raw_state(game, state);
	return (true);
}

static void	controller_handle_gameplay_actions(t_game *game,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	bool	break_pressed;
	bool	place_pressed;

	game->controller.menu_quit_held = false;
	break_pressed = active[ACTION_BREAK]
		&& !game->controller.prev_action_active[ACTION_BREAK];
	place_pressed = active[ACTION_PLACE]
		&& !game->controller.prev_action_active[ACTION_PLACE];
	if (break_pressed)
		test_break_wall_in_front(game);
	if (place_pressed)
		place_breakable_block(game);
}

static void	controller_store_prev_actions(t_game *game,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	ft_memcpy(game->controller.prev_action_active, active, sizeof(bool)
		* CONFIG_MODAL_CONTROL_COUNT);
}

static bool	controller_prepare_update(t_game *game, GLFWgamepadstate *state,
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

static void	controller_try_toggle_menu(t_game *game,
				bool active[CONFIG_MODAL_CONTROL_COUNT])
{
	bool	allow_menu_toggle;
	bool	menu_pressed;

	allow_menu_toggle = true;
	if (game->menu.open && game->menu.controls_rebinding
		&& game->menu.controls_rebind_column == CONTROLS_COLUMN_CONTROLLER)
		allow_menu_toggle = false;
	menu_pressed = active[ACTION_MENU]
		&& !game->controller.prev_action_active[ACTION_MENU];
	if (menu_pressed && allow_menu_toggle)
		toggle_config_modal(game);
}

void	controller_update_advanced(t_game *game)
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
