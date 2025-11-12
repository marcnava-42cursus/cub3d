/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/12 18:57:49 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"

void	update_delta_time(t_game *game)
{
	double	current_time;
	double	dt;

	if (!game)
		return ;
	current_time = mlx_get_time();
	if (game->last_frame_time <= 0.0)
	{
		game->delta_time = 0.0;
		game->last_frame_time = current_time;
		return ;
	}
	dt = current_time - game->last_frame_time;
	if (dt < 0.0)
		dt = 0.0;
	if (dt > MAX_DELTA_TIME)
		dt = MAX_DELTA_TIME;
	game->delta_time = dt;
	game->last_frame_time = current_time;
}

void	init_player_parameters(t_game *game)
{
	if (!game)
		return ;
	game->move_speed = 4.0f;
	game->rot_speed = 2.5f;
	game->player_radius = 0.2f;
	game->delta_time = 0.0;
	game->last_frame_time = mlx_get_time();
}
