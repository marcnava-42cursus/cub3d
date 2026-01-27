/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 05:12:12 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "structs.h"
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

	direction = -1.0f;
	if (!game || !game->mlx)
		return ;
	if (right)
		direction = 1.0f;
	rotation_delta = game->rot_speed * (float)game->mlx->delta_time * direction;
	game->cub_data.player.angle += rotation_delta;
	game->cub_data.player.angle = normalize_angle(game->cub_data.player.angle);
}

void	pitch_player(t_game *game, bool up)
{
	float	direction;
	float	rotation_delta;
	float	max_pitch;

	direction = -1.0f;
	if (!game || !game->mlx)
		return ;
	if (up)
		direction = 1.0f;
	rotation_delta = 600.0f * (float)game->mlx->delta_time * direction;
	game->cub_data.player.pitch += rotation_delta;
	// this height / 2 is wrong but it stops crash with pitch
	max_pitch = game->double_buffer[NEXT]->height * 0.35f;
	game->cub_data.player.pitch =
		clamp(game->cub_data.player.pitch, -max_pitch, max_pitch);
}
