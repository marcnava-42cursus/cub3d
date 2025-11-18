/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/31 12:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"
#include "render.h"

/**
 * @brief Rotates the player's camera view left or right
 *
 * This function updates the player's angle based on the rotation speed
 * and delta time. The angle is kept within the range [-PI, PI] by wrapping
 * around when necessary.
 *
 * @param game Pointer to the game structure
 * @param right true to rotate right (clockwise), false to rotate left
 */
void	rotate_player(t_game *game, bool right)
{
	float	direction;
	float	rotation_delta;

	if (!game)
		return ;
	direction = right ? 1.0f : -1.0f;
	rotation_delta = game->rot_speed * (float)game->delta_time * direction;
	game->cub_data.player.angle += rotation_delta;
	game->cub_data.player.angle = normalize_angle(game->cub_data.player.angle);
}
