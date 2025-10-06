/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex                                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by codex             #+#    #+#             */
/*   Updated: 2025/10/02 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void    rotate_player(t_game *game, bool right)
{
    float   dir;
    float   rs;

    dir = right ? 1.0f : -1.0f;
    rs = game->rot_speed * (float)game->delta_time * dir;
    game->cub_data.player.angle += rs;
    if (game->cub_data.player.angle > FT_PI)
        game->cub_data.player.angle -= 2 * FT_PI;
    else if (game->cub_data.player.angle < -FT_PI)
        game->cub_data.player.angle += 2 * FT_PI;
}

