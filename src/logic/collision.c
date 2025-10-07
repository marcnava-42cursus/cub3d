/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/07 19:53:10 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include <string.h>

bool    is_cell_blocking(t_game *game, int cell_x, int cell_y)
{
    char    *row;

    if (cell_x < 0 || cell_y < 0 || cell_y >= game->cub_data.map.height)
        return (true);
    row = game->cub_data.map.grid[cell_y];
    if (!row)
        return (true);
    if (cell_x >= (int)strlen(row))
        return (true);
    return (row[cell_x] == '1' || row[cell_x] == ' ');
}

bool    collides_with_wall(t_game *game, float x, float y)
{
    const float   r = game->player_radius;
    float         sx[4];
    float         sy[4];
    int           i;

    sx[0] = x - r; sy[0] = y - r;
    sx[1] = x + r; sy[1] = y - r;
    sx[2] = x - r; sy[2] = y + r;
    sx[3] = x + r; sy[3] = y + r;
    i = 0;
    while (i < 4)
    {
        if (is_cell_blocking(game, (int)floorf(sx[i]), (int)floorf(sy[i])))
            return (true);
        i++;
    }
    return (false);
}

void    attempt_move(t_game *game, float step_x, float step_y)
{
    float   nx;
    float   ny;

    nx = game->cub_data.player.x + step_x;
    ny = game->cub_data.player.y + step_y;
    if (!collides_with_wall(game, nx, game->cub_data.player.y))
        game->cub_data.player.x = nx;
    if (!collides_with_wall(game, game->cub_data.player.x, ny))
        game->cub_data.player.y = ny;
}

