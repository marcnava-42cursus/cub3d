/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_audio_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 04:45:37 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string.h>

#include "miniaudio_implementation.h"
#include "audio.h"
#include "logic_bonus.h"

static t_bonus_audio	*bonus_audio_ctx(void)
{
	static t_bonus_audio	audio_ctx;

	return (&audio_ctx);
}

static bool	load_sound(ma_engine *engine, const char *path, ma_sound *sound)
{
	if (ma_sound_init_from_file(engine, path, MA_SOUND_FLAG_DECODE, NULL, NULL,
			sound) != MA_SUCCESS)
		return (false);
	return (true);
}

static float	clamp_volume(float value)
{
	if (value < STEP_AUDIO_VOLUME_MIN)
		return (STEP_AUDIO_VOLUME_MIN);
	if (value > STEP_AUDIO_VOLUME_MAX)
		return (STEP_AUDIO_VOLUME_MAX);
	return (value);
}

static float	clamp_pitch(float value)
{
	if (value < STEP_AUDIO_PITCH_ABS_MIN)
		return (STEP_AUDIO_PITCH_ABS_MIN);
	if (value > STEP_AUDIO_PITCH_ABS_MAX)
		return (STEP_AUDIO_PITCH_ABS_MAX);
	return (value);
}

static float	step_pitch_random(void)
{
	static uint32_t	seed;
	float			min_pitch;
	float			max_pitch;
	float			tmp;
	float			t;

	if (seed == 0)
		seed = (uint32_t)(mlx_get_time() * 1000000.0) + 1u;
	min_pitch = clamp_pitch(STEP_AUDIO_PITCH_MIN);
	max_pitch = clamp_pitch(STEP_AUDIO_PITCH_MAX);
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

bool	bonus_audio_init(void)
{
	t_bonus_audio	*audio;

	audio = bonus_audio_ctx();
	if (audio->initialized)
		return (audio->orb_sound_ready || audio->step_sound_ready);
	if (ma_engine_init(NULL, &audio->engine) != MA_SUCCESS)
		return (false);
	audio->initialized = true;
	audio->orb_sound_ready = load_sound(&audio->engine, ORB_LAUNCH_AUDIO_PATH,
			&audio->orb_launch_sound);
	audio->step_sound_ready = load_sound(&audio->engine, STEP_AUDIO_PATH,
			&audio->step_sound);
	if (audio->orb_sound_ready)
	{
		ma_sound_set_looping(&audio->orb_launch_sound, MA_TRUE);
		ma_sound_set_volume(&audio->orb_launch_sound, ORB_AUDIO_VOLUME_MAX);
	}
	if (audio->step_sound_ready)
	{
		ma_sound_set_volume(&audio->step_sound, clamp_volume(STEP_AUDIO_VOLUME));
		ma_sound_set_looping(&audio->step_sound, MA_FALSE);
	}
	if (!audio->orb_sound_ready && !audio->step_sound_ready)
	{
		ma_engine_uninit(&audio->engine);
		memset(audio, 0, sizeof(*audio));
		return (false);
	}
	return (true);
}

void	bonus_audio_shutdown(void)
{
	t_bonus_audio	*audio;

	audio = bonus_audio_ctx();
	if (!audio->initialized)
		return ;
	if (audio->orb_sound_ready)
		ma_sound_uninit(&audio->orb_launch_sound);
	if (audio->step_sound_ready)
		ma_sound_uninit(&audio->step_sound);
	ma_engine_uninit(&audio->engine);
	memset(audio, 0, sizeof(*audio));
}

void	bonus_audio_play_orb_launch(void)
{
	t_bonus_audio	*audio;

	audio = bonus_audio_ctx();
	if (!audio->initialized || !audio->orb_sound_ready)
		return ;
	ma_sound_stop(&audio->orb_launch_sound);
	ma_sound_seek_to_pcm_frame(&audio->orb_launch_sound, 0);
	ma_sound_start(&audio->orb_launch_sound);
}

void	bonus_audio_stop_orb_launch(void)
{
	t_bonus_audio	*audio;

	audio = bonus_audio_ctx();
	if (!audio->initialized || !audio->orb_sound_ready)
		return ;
	ma_sound_stop(&audio->orb_launch_sound);
	ma_sound_seek_to_pcm_frame(&audio->orb_launch_sound, 0);
}

void	bonus_audio_set_orb_volume(float volume)
{
	t_bonus_audio	*audio;

	audio = bonus_audio_ctx();
	if (!audio->initialized || !audio->orb_sound_ready)
		return ;
	if (volume < ORB_AUDIO_VOLUME_MIN)
		volume = ORB_AUDIO_VOLUME_MIN;
	if (volume > ORB_AUDIO_VOLUME_MAX)
		volume = ORB_AUDIO_VOLUME_MAX;
	ma_sound_set_volume(&audio->orb_launch_sound, volume);
}

void	bonus_audio_set_step_loop(bool enabled)
{
	t_bonus_audio	*audio;

	audio = bonus_audio_ctx();
	if (!audio->initialized || !audio->step_sound_ready)
		return ;
	if (enabled && !ma_sound_is_playing(&audio->step_sound))
		start_step_sound(&audio->step_sound);
}
