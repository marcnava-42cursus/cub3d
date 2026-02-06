/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_audio_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include <string.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define ORB_LAUNCH_AUDIO_PATH "./assets/audio/scream.mp3"

typedef struct s_bonus_audio
{
	bool		initialized;
	bool		orb_sound_ready;
	ma_engine	engine;
	ma_sound	orb_launch_sound;
}	t_bonus_audio;

static t_bonus_audio	*bonus_audio_ctx(void)
{
	static t_bonus_audio	audio_ctx;

	return (&audio_ctx);
}

bool	bonus_audio_init(void)
{
	t_bonus_audio	*audio;
	ma_result		result;

	audio = bonus_audio_ctx();
	if (audio->initialized)
		return (audio->orb_sound_ready);
	result = ma_engine_init(NULL, &audio->engine);
	if (result != MA_SUCCESS)
		return (false);
	audio->initialized = true;
	result = ma_sound_init_from_file(&audio->engine, ORB_LAUNCH_AUDIO_PATH,
			MA_SOUND_FLAG_DECODE, NULL, NULL, &audio->orb_launch_sound);
	if (result != MA_SUCCESS)
	{
		ma_engine_uninit(&audio->engine);
		memset(audio, 0, sizeof(*audio));
		return (false);
	}
	audio->orb_sound_ready = true;
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
