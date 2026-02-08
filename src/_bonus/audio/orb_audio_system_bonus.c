/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_audio_system_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:20:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 13:20:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "audio.h"
#include "logic_bonus.h"

t_bonus_audio	*audio_ctx(void);

static bool	load_sound(ma_engine *engine, const char *path, ma_sound *sound)
{
	if (ma_sound_init_from_file(engine, path, MA_SOUND_FLAG_DECODE, NULL, NULL,
			sound) != MA_SUCCESS)
		return (false);
	return (true);
}

static void	init_audio_data(t_bonus_audio *audio)
{
	audio->initialized = true;
	audio->orb_init_ready = load_sound(&audio->engine, ORB_INIT_AUDIO_PATH,
			&audio->orb_init_sound);
	audio->orb_loop_ready = load_sound(&audio->engine, ORB_LOOP_AUDIO_PATH,
			&audio->orb_loop_sound);
	audio->orb_end_ready = load_sound(&audio->engine, ORB_END_AUDIO_PATH,
			&audio->orb_end_sound);
	audio->step_sound_ready = load_sound(&audio->engine, STEP_AUDIO_PATH,
			&audio->step_sound);
}

static void	configure_audio_defaults(t_bonus_audio *audio)
{
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
			clamp(STEP_AUDIO_VOLUME, STEP_AUDIO_VOLUME_MIN,
				STEP_AUDIO_VOLUME_MAX));
		ma_sound_set_looping(&audio->step_sound, MA_FALSE);
	}
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
	init_audio_data(audio);
	configure_audio_defaults(audio);
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
