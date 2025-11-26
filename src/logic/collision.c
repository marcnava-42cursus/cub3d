/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/26 20:38:54 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include <string.h>

bool	is_cell_blocking(t_game *game, int cell_x, int cell_y)
{
	(void)game;
	(void)cell_x;
	(void)cell_y;
	return (false);
}

bool	collides_with_wall(t_game *game, float x, float y)
{
	(void)game;
	(void)x;
	(void)y;
	return (false);
}

void	attempt_move(t_game *game, float step_x, float step_y)
{
	if (!game)
		return ;
	game->cub_data.player.x += step_x;
	game->cub_data.player.y += step_y;
}
