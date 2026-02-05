/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 19:25:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 19:25:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "structs.h"

static void	init_player_angle(t_game *game)
{
	if (!game)
		return ;
	if (game->cub_data.player.orientation == NORTH)
		game->cub_data.player.angle = -FT_PI / 2.0f;
	else if (game->cub_data.player.orientation == SOUTH)
		game->cub_data.player.angle = FT_PI / 2.0f;
	else if (game->cub_data.player.orientation == EAST)
		game->cub_data.player.angle = 0.0f;
	else if (game->cub_data.player.orientation == WEST)
		game->cub_data.player.angle = FT_PI;
}

void	init_movement_system(t_game *game)
{
	if (!game)
		return ;
	init_player_angle(game);
	init_player_parameters(game);
	game->movement_lock_until = 0.0;
	game->last_teleport_time = -10.0;
	game->last_teleport_id = '\0';
	game->mouse_initialized = false;
	game->mouse_delta_accumulated = 0.0f;
	game->mouse_delta_accumulated_y = 0.0f;
	game->mouse_sensitivity = DEFAULT_MOUSE_SENS;
	game->last_mouse_x = 0.0;
	game->last_mouse_y = 0.0;
	game->last_player_angle = game->cub_data.player.angle;
	game->last_grid_x = -1;
	game->last_grid_y = -1;
	mlx_key_hook(game->mlx, KEY_HOOK, game);
	mlx_mouse_hook(game->mlx, MOUSE_HOOK, game);
	mlx_loop_hook(game->mlx, UPDATE_GAME_LOOP, game);
}
