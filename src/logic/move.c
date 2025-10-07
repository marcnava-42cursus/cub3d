/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/07 19:53:45 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"
#include <math.h>

#ifndef FT_PI_2
# define FT_PI_2 (FT_PI / 2.0)
#endif

void    move_forward(t_game *game, bool forward)
{
    float   nx;
    float   ny;
    float   dir;
    float   spd;

    dir = forward ? 1.0f : -1.0f;
    spd = game->move_speed * (float)game->delta_time * dir;
    nx = cosf(game->cub_data.player.angle) * spd;
    ny = sinf(game->cub_data.player.angle) * spd;
    attempt_move(game, nx, ny);
}

void    move_strafe(t_game *game, bool right)
{
    float   nx;
    float   ny;
    float   ang;
    float   dir;
    float   spd;

    dir = right ? 1.0f : -1.0f;
    ang = game->cub_data.player.angle + (float)FT_PI_2 * dir;
    spd = game->move_speed * (float)game->delta_time;
    nx = cosf(ang) * spd;
    ny = sinf(ang) * spd;
    attempt_move(game, nx, ny);
}
