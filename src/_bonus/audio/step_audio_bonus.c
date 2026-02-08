/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_audio_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 04:34:57 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "audio.h"
#include "logic_bonus.h"

t_bonus_audio	*audio_ctx(void);

static float	step_pitch_random(void)
{
	static uint32_t	seed;
	float			min_pitch;
	float			max_pitch;
	float			tmp;
	float			t;

	if (seed == 0)
		seed = (uint32_t)(mlx_get_time() * 1000000.0) + 1u;
	min_pitch = clamp(STEP_AUDIO_PITCH_MIN,
			STEP_AUDIO_PITCH_ABS_MIN, STEP_AUDIO_PITCH_ABS_MAX);
	max_pitch = clamp(STEP_AUDIO_PITCH_MAX,
			STEP_AUDIO_PITCH_ABS_MIN, STEP_AUDIO_PITCH_ABS_MAX);
	if (min_pitch > max_pitch)
	{
		tmp = min_pitch;
		min_pitch = max_pitch;
		max_pitch = tmp;
	}
	seed = seed * 1664525u + 1013904223u;
	t = (float)(seed & 0x00FFFFFFu) / 16777215.0f;
	return (min_pitch + (max_pitch - min_pitch) * t);
}

static void	start_step_sound(ma_sound *step_sound)
{
	ma_sound_set_pitch(step_sound, step_pitch_random());
	ma_sound_seek_to_pcm_frame(step_sound, 0);
	ma_sound_start(step_sound);
}

bool	audio_step_set_loop(bool enabled)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (!audio->initialized || !audio->step_sound_ready)
		return (false);
	if (enabled && !ma_sound_is_playing(&audio->step_sound))
	{
		start_step_sound(&audio->step_sound);
		return (true);
	}
	return (false);
}

bool	audio_step_update_loop(bool player_is_moving)
{
	return (audio_step_set_loop(player_is_moving));
}
