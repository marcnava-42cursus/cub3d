/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_distance_audio_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 05:05:45 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "audio.h"
#include "cub3d.h"
#include "logic_bonus.h"

static float	clamp_unit(float value)
{
	if (value < 0.0f)
		return (0.0f);
	if (value > 1.0f)
		return (1.0f);
	return (value);
}

static float	orb_distance_normalized(const t_game *game)
{
	float	dx;
	float	dy;
	float	distance;

	dx = game->orb.x - game->cub_data.player.x;
	dy = game->orb.y - game->cub_data.player.y;
	distance = sqrtf(dx * dx + dy * dy);
	return (clamp_unit(distance / ORB_AUDIO_MAX_DISTANCE));
}

void	bonus_audio_update_orb_volume(const t_game *game)
{
	float	distance_norm;
	float	volume;

	if (!game || !game->orb.active)
		return ;
	distance_norm = orb_distance_normalized(game);
	volume = ORB_AUDIO_VOLUME_MAX - (distance_norm
			* (ORB_AUDIO_VOLUME_MAX - ORB_AUDIO_VOLUME_MIN));
	bonus_audio_set_orb_volume(volume);
}
