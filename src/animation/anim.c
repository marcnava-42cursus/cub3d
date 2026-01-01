/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 13:28:38 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/01 14:04:04 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	anim_init(t_anim *anim, const unsigned int *frames,
		const unsigned int *holds, bool loop, unsigned int fps)
{
	anim->frames = frames;
	anim->holds = holds;
	anim->hold_left = anim->holds[0];
	anim->count = 0;
	anim->i = 0;
	anim->time = 0.0f;
	anim->spf = 1.0 / (float)fps;
	anim->loop = loop;	
	anim->finished = true;
}

void	anim_update(t_anim *anim, t_atlas *atlas, float delta_time)
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
		get_next_frame(atlas, anim->frames[anim->i]);
	}
}
