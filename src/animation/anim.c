/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 13:28:38 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/04 15:14:58 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	anim_init(t_anim *anim, t_atlas *atlas, const unsigned int *frames,
		const unsigned int *holds, bool loop)
{
	anim->atlas = atlas;
	anim->current_frame[X] = 0;
	anim->current_frame[Y] = 0;
	anim->frames = frames;
	anim->holds = holds;
	anim->hold_left = anim->holds[0];
	anim->count = 0;
	anim->i = 0;
	anim->time = 0.0f;
	anim->spf = 1.0f / FPS;
	anim->loop = loop;	
	anim->finished = true;
}

void	get_next_frame(t_anim *anim, int increment)
{
	unsigned int	x;
	unsigned int	y;

	x = anim->current_frame[X] + increment;
	y = anim->current_frame[Y];
	y += x / anim->atlas->max_frame[X];
	x %= anim->atlas->max_frame[X];
	y %= anim->atlas->max_frame[Y];
	anim->current_frame[X] = x;
	anim->current_frame[Y] = y;
}

void	anim_update(t_anim *anim, float delta_time)
{
	if (anim->finished)
		return ;
	anim->time += delta_time;
	while (anim->time >= anim->spf)
	{
		anim->time -= anim->spf;
		if (anim->hold_left > 0)
			anim->hold_left--;
		if (anim->hold_left == 0)
		{
			if (anim->i + i < anim_count)
				anim->i++;
			else if (anim->loop)
				anim->i = 0;
			else {
				anim->finished = true;
				break;
			}
		}
		anim->hold_left = anim->holds[anim->i];
		get_next_frame(anim, anim->frames[anim->i]);
	}
}
