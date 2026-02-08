/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_distance_audio_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 12:33:04 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "audio.h"
#include "cub3d.h"
#include "logic_bonus.h"

static float	orb_distance_normalized(const t_game *game)
{
	float	dx;
	float	dy;
	float	distance;

	dx = game->orb.x - game->cub_data.player.x;
	dy = game->orb.y - game->cub_data.player.y;
	distance = sqrtf(dx * dx + dy * dy);
	return (clamp(distance / ORB_AUDIO_MAX_DISTANCE, 0.0f, 1.0f));
}

void	audio_orb_update_volume(const t_game *game)
{
	float	distance_norm;
	float	proximity;
	float	volume;

	if (!game || !game->orb.active)
		return ;
	distance_norm = orb_distance_normalized(game);
	proximity = 1.0f - distance_norm;
	proximity = powf(proximity, ORB_AUDIO_CURVE_EXPONENT);
	volume = ORB_AUDIO_VOLUME_MIN + (proximity
			* (ORB_AUDIO_VOLUME_MAX - ORB_AUDIO_VOLUME_MIN));
	audio_set_orb_volume(volume);
}
