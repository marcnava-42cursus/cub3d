/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_H
# define AUDIO_H

# include <stdbool.h>

# define MINIAUDIO_IMPLEMENTATION
# include "miniaudio.h"

# define ORB_LAUNCH_AUDIO_PATH "./assets/audio/scream.mp3"
# define STEP_AUDIO_PATH "./assets/audio/step.mp3"

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
	bool		orb_sound_ready;
	bool		step_sound_ready;
	ma_engine	engine;
	ma_sound	orb_launch_sound;
	ma_sound	step_sound;
}	t_bonus_audio;

#endif
