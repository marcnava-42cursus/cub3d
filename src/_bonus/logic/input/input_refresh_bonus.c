/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_refresh_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 07:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 07:05:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_bonus.h"
#include "structs.h"
#include "logic_bonus.h"

static void	clear_movement_keys(t_game *game)
{
	if (!game)
		return ;
	game->key_w_pressed = false;
	game->key_s_pressed = false;
	game->key_a_pressed = false;
	game->key_d_pressed = false;
	game->key_left_pressed = false;
	game->key_right_pressed = false;
	game->key_up_pressed = false;
	game->key_down_pressed = false;
}

static void	update_movement_keys(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	game->key_w_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_FORWARD]);
	game->key_s_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_BACKWARD]);
	game->key_d_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_STRAFE_RIGHT]);
	game->key_a_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_STRAFE_LEFT]);
	game->key_right_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_TURN_RIGHT]);
	game->key_left_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_TURN_LEFT]);
	game->key_up_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_LOOK_UP]);
	game->key_down_pressed = mlx_is_key_down(game->mlx,
			game->menu.controls_key_codes[ACTION_LOOK_DOWN]);
}

void	refresh_key_states_bonus(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	if (is_config_modal_open(game))
	{
		clear_movement_keys(game);
		return ;
	}
	update_movement_keys(game);
	controller_update_bonus(game);
}
