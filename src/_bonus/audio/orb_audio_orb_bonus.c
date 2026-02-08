/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_audio_orb_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 13:20:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 13:20:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "audio.h"
#include "logic_bonus.h"

t_bonus_audio	*audio_ctx(void);

static void	stop_and_rewind_sound(ma_sound *sound)
{
	ma_sound_stop(sound);
	ma_sound_seek_to_pcm_frame(sound, 0);
}

void	audio_orb_start(void)
{
	t_bonus_audio	*audio;

	audio = audio_ctx();
	if (!audio->initialized
		|| (!audio->orb_init_ready && !audio->orb_loop_ready))
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
	volume = clamp(volume, ORB_AUDIO_VOLUME_MIN, ORB_AUDIO_VOLUME_MAX);
	if (audio->orb_init_ready)
		ma_sound_set_volume(&audio->orb_init_sound, volume);
	if (audio->orb_loop_ready)
		ma_sound_set_volume(&audio->orb_loop_sound, volume);
	if (!audio->orb_loop_ready || !audio->orb_loop_pending)
		return ;
	if (audio->orb_init_ready && ma_sound_is_playing(&audio->orb_init_sound))
		return ;
	if (!ma_sound_is_playing(&audio->orb_loop_sound))
		ma_sound_start(&audio->orb_loop_sound);
	audio->orb_loop_pending = false;
}
