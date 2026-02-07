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

static t_bonus_audio	*audio_ctx(void)
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

static void	stop_and_rewind_sound(ma_sound *sound)
{
	ma_sound_stop(sound);
	ma_sound_seek_to_pcm_frame(sound, 0);
}

static void	update_orb_loop_state(t_bonus_audio *audio)
{
	if (!audio->orb_loop_ready || !audio->orb_loop_pending)
		return ;
	if (audio->orb_init_ready && ma_sound_is_playing(&audio->orb_init_sound))
		return ;
	if (!ma_sound_is_playing(&audio->orb_loop_sound))
		ma_sound_start(&audio->orb_loop_sound);
	audio->orb_loop_pending = false;
}

float	clamp_audio(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
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
	min_pitch = clamp_audio(STEP_AUDIO_PITCH_MIN,
			STEP_AUDIO_PITCH_ABS_MIN, STEP_AUDIO_PITCH_ABS_MAX);
	max_pitch = clamp_audio(STEP_AUDIO_PITCH_MAX,
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

bool	audio_system_init(void)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (audio->initialized)
		return (audio->orb_init_ready || audio->orb_loop_ready
			|| audio->orb_end_ready || audio->step_sound_ready);
	if (ma_engine_init(NULL, &audio->engine) != MA_SUCCESS)
		return (false);
	audio->initialized = true;
	audio->orb_init_ready = load_sound(&audio->engine, ORB_INIT_AUDIO_PATH,
			&audio->orb_init_sound);
	audio->orb_loop_ready = load_sound(&audio->engine, ORB_LOOP_AUDIO_PATH,
			&audio->orb_loop_sound);
	audio->orb_end_ready = load_sound(&audio->engine, ORB_END_AUDIO_PATH,
			&audio->orb_end_sound);
	audio->step_sound_ready = load_sound(&audio->engine, STEP_AUDIO_PATH,
			&audio->step_sound);
	if (audio->orb_init_ready)
	{
		ma_sound_set_looping(&audio->orb_init_sound, MA_FALSE);
		ma_sound_set_volume(&audio->orb_init_sound, ORB_AUDIO_VOLUME_MAX);
	}
	if (audio->orb_loop_ready)
	{
		ma_sound_set_looping(&audio->orb_loop_sound, MA_TRUE);
		ma_sound_set_volume(&audio->orb_loop_sound, ORB_AUDIO_VOLUME_MAX);
	}
	if (audio->orb_end_ready)
		ma_sound_set_looping(&audio->orb_end_sound, MA_FALSE);
	if (audio->step_sound_ready)
	{
		ma_sound_set_volume(&audio->step_sound,
			clamp_audio(STEP_AUDIO_VOLUME, STEP_AUDIO_VOLUME_MIN,
				STEP_AUDIO_VOLUME_MAX));
		ma_sound_set_looping(&audio->step_sound, MA_FALSE);
	}
	if (!audio->orb_init_ready && !audio->orb_loop_ready
		&& !audio->orb_end_ready && !audio->step_sound_ready)
	{
		ma_engine_uninit(&audio->engine);
		memset(audio, 0, sizeof(*audio));
		return (false);
	}
	return (true);
}

void	audio_system_shutdown(void)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (!audio->initialized)
		return ;
	if (audio->orb_init_ready)
		ma_sound_uninit(&audio->orb_init_sound);
	if (audio->orb_loop_ready)
		ma_sound_uninit(&audio->orb_loop_sound);
	if (audio->orb_end_ready)
		ma_sound_uninit(&audio->orb_end_sound);
	if (audio->step_sound_ready)
		ma_sound_uninit(&audio->step_sound);
	ma_engine_uninit(&audio->engine);
	memset(audio, 0, sizeof(*audio));
}

void	audio_orb_start(void)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (!audio->initialized || (!audio->orb_init_ready
			&& !audio->orb_loop_ready))
		return ;
	audio->orb_loop_pending = false;
	if (audio->orb_end_ready)
		stop_and_rewind_sound(&audio->orb_end_sound);
	if (audio->orb_init_ready)
	{
		stop_and_rewind_sound(&audio->orb_init_sound);
		if (audio->orb_loop_ready)
			stop_and_rewind_sound(&audio->orb_loop_sound);
		ma_sound_start(&audio->orb_init_sound);
		audio->orb_loop_pending = audio->orb_loop_ready;
	}
	else if (audio->orb_loop_ready)
	{
		stop_and_rewind_sound(&audio->orb_loop_sound);
		ma_sound_start(&audio->orb_loop_sound);
	}
}

void	audio_orb_stop(void)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (!audio->initialized)
		return ;
	audio->orb_loop_pending = false;
	if (audio->orb_init_ready)
		stop_and_rewind_sound(&audio->orb_init_sound);
	if (audio->orb_loop_ready)
		stop_and_rewind_sound(&audio->orb_loop_sound);
	if (audio->orb_end_ready)
	{
		stop_and_rewind_sound(&audio->orb_end_sound);
		ma_sound_start(&audio->orb_end_sound);
	}
}

void	audio_set_orb_volume(float volume)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (!audio->initialized)
		return ;
	volume = clamp_audio(volume, ORB_AUDIO_VOLUME_MIN,
			ORB_AUDIO_VOLUME_MAX);
	if (audio->orb_init_ready)
		ma_sound_set_volume(&audio->orb_init_sound, volume);
	if (audio->orb_loop_ready)
		ma_sound_set_volume(&audio->orb_loop_sound, volume);
	update_orb_loop_state(audio);
}

void	audio_step_set_loop(bool enabled)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (!audio->initialized || !audio->step_sound_ready)
		return ;
	if (enabled && !ma_sound_is_playing(&audio->step_sound))
		start_step_sound(&audio->step_sound);
}
