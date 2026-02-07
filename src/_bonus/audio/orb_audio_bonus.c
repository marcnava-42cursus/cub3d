/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_audio_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 04:31:11 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include <string.h>
#include "miniaudio.h"

#define ORB_LAUNCH_AUDIO_PATH "./assets/audio/scream.mp3"
#define STEP_AUDIO_PATH "./assets/audio/step.mp3"
#define STEP_AUDIO_VOLUME 0.10f
#define STEP_AUDIO_VOLUME_MIN 0.0f
#define STEP_AUDIO_VOLUME_MAX 1.0f

typedef struct s_bonus_audio
{
	bool		initialized;
	bool		orb_sound_ready;
	bool		step_sound_ready;
	ma_engine	engine;
	ma_sound	orb_launch_sound;
	ma_sound	step_sound;
}	t_bonus_audio;

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
	if (audio->step_sound_ready)
	{
		ma_sound_set_volume(&audio->step_sound, clamp_volume(STEP_AUDIO_VOLUME));
		ma_sound_set_looping(&audio->step_sound, MA_TRUE);
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

void	bonus_audio_set_step_loop(bool enabled)
{
	t_bonus_audio	*audio;

	audio = bonus_audio_ctx();
	if (!audio->initialized || !audio->step_sound_ready)
		return ;
	if (enabled)
	{
		ma_sound_set_looping(&audio->step_sound, MA_TRUE);
		if (!ma_sound_is_playing(&audio->step_sound))
		{
			ma_sound_seek_to_pcm_frame(&audio->step_sound, 0);
			ma_sound_start(&audio->step_sound);
		}
	}
	else if (ma_sound_is_playing(&audio->step_sound))
		ma_sound_set_looping(&audio->step_sound, MA_FALSE);
}
