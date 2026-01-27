/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 13:28:38 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/27 19:16:12 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

static void	set_frame_from_id(t_anim *anim, unsigned int id)
{
	unsigned int	width;

	width = anim->atlas->max_frame[X];
	anim->current_frame[X] = id % width;
	anim->current_frame[Y] = id / width;
}

void	anim_init(t_anim *anim, t_atlas *atlas, const unsigned int *frames,
		const unsigned int *holds, unsigned int count, bool loop)
{
	anim->atlas = atlas;
	anim->current_frame[X] = 0;
	anim->current_frame[Y] = 0;
	anim->frames = frames;
	anim->holds = holds;
	anim->hold_left = anim->holds[0];
	anim->count = count;
	anim->i = 0;
	anim->time = 0.0f;
	anim->spf = 1.0f / FPS;
	anim->loop = loop;
	anim->finished = false;
}

void	anim_start(t_anim *anim)
{
	if (!anim || anim->count == 0)
		return ;
	anim->i = 0;
	anim->time = 0.0f;
	anim->finished = false;
	anim->hold_left = anim->holds[0];
	set_frame_from_id(anim, anim->frames[0]);
}

bool	anim_update(t_anim *anim, float delta_time)
{
	if (anim->finished)
		return (true);
	anim->time += delta_time;
	while (anim->time >= anim->spf)
	{
		anim->time -= anim->spf;
		if (anim->hold_left > 1)
		{
			anim->hold_left--;
			continue ;
		}
		if (anim->i + 1 < anim->count)
			anim->i++;
		else if (anim->loop)
			anim->i = 0;
		else
		{
			anim->finished = true;
			return (true);
		}
		anim->hold_left = anim->holds[anim->i];
		set_frame_from_id(anim, anim->frames[anim->i]);
	}
	return (anim->finished);
}
