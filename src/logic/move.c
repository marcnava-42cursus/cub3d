/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/31 12:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"
#include <math.h>

#ifndef FT_PI_2
# define FT_PI_2 (FT_PI / 2.0f)
#endif

/**
 * @brief Moves the player forward or backward in the direction they're facing
 *
 * This function calculates the movement vector based on the player's current
 * angle and move speed, then attempts to move the player in that direction.
 * Collision detection is handled by attempt_move().
 *
 * @param game Pointer to the game structure
 * @param forward true to move forward, false to move backward
 */
void	move_forward(t_game *game, bool forward)
{
	float	new_x;
	float	new_y;
	float	direction;
	float	speed;

	if (!game)
		return ;
	direction = forward ? 1.0f : -1.0f;
	speed = game->move_speed * (float)game->delta_time * direction;
	new_x = cosf(game->cub_data.player.angle) * speed;
	new_y = sinf(game->cub_data.player.angle) * speed;
	attempt_move(game, new_x, new_y);
}

/**
 * @brief Moves the player left or right (strafing)
 *
 * This function calculates the movement vector perpendicular to the player's
 * facing direction and attempts to move the player sideways.
 * Collision detection is handled by attempt_move().
 *
 * @param game Pointer to the game structure
 * @param right true to strafe right, false to strafe left
 */
void	move_strafe(t_game *game, bool right)
{
	float	new_x;
	float	new_y;
	float	strafe_angle;
	float	direction;
	float	speed;

	if (!game)
		return ;
	direction = right ? 1.0f : -1.0f;
	strafe_angle = game->cub_data.player.angle + FT_PI_2 * direction;
	speed = game->move_speed * (float)game->delta_time;
	new_x = cosf(strafe_angle) * speed;
	new_y = sinf(strafe_angle) * speed;
	attempt_move(game, new_x, new_y);
}
