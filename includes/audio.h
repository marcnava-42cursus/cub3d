/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 21:25:48 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_H
# define AUDIO_H

# include <stdbool.h>

# include "miniaudio.h"

# define ORB_INIT_AUDIO_PATH "./assets/audio/orb_init.mp3"
# define ORB_LOOP_AUDIO_PATH "./assets/audio/orb_loop.mp3"
# define ORB_END_AUDIO_PATH "./assets/audio/orb_end.mp3"
# define STEP_AUDIO_PATH "./assets/audio/step.mp3"

# define ORB_AUDIO_VOLUME_MIN 0.03f
# define ORB_AUDIO_VOLUME_MAX 1.80f
# define ORB_AUDIO_MAX_DISTANCE 12.0f
# define ORB_AUDIO_CURVE_EXPONENT 2.2f

# define STEP_AUDIO_VOLUME 0.25f
# define STEP_AUDIO_VOLUME_MIN 0.0f
# define STEP_AUDIO_VOLUME_MAX 1.0f

# define STEP_AUDIO_PITCH_MIN 0.8f
# define STEP_AUDIO_PITCH_MAX 1.2f
# define STEP_AUDIO_PITCH_ABS_MIN 0.05f
# define STEP_AUDIO_PITCH_ABS_MAX 4.0f

typedef struct s_bonus_audio
{
	bool		initialized;
	bool		orb_init_ready;
	bool		orb_loop_ready;
	bool		orb_end_ready;
	bool		orb_loop_pending;
	bool		step_sound_ready;
	ma_engine	engine;
	ma_sound	orb_init_sound;
	ma_sound	orb_loop_sound;
	ma_sound	orb_end_sound;
	ma_sound	step_sound;
}	t_bonus_audio;

void	audio_set_orb_volume(float volume);

#endif
