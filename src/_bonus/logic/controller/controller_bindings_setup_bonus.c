/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller_bindings_setup_bonus.c                  :+:      :+:    :+:   */
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

t_controller_bind	controller_bind_button(int id)
{
	t_controller_bind	bind;

	bind.type = CONTROLLER_BIND_BUTTON;
	bind.id = id;
	bind.dir = 0;
	return (bind);
}

t_controller_bind	controller_bind_axis(int id, int dir)
{
	t_controller_bind	bind;

	bind.type = CONTROLLER_BIND_AXIS;
	bind.id = id;
	bind.dir = dir;
	return (bind);
}

void	controller_set_default_binds(t_game *game)
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
