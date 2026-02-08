/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_anim_states.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:29:32 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/08 04:09:02 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	update_door_anims(t_cub_data *data, t_orb_projectile *orb,
		float delta_time)
{
	bool		finished;

	finished = anim_update(&data->effects.door_anims[
			data->effects.current_door_anim], delta_time);
	if (orb->mode == ORB_MODE_NONE)
	{
		if (finished)
			set_current_anim(data->effects.door_anims,
				&data->effects.current_door_anim, DOOR_CLOSED);
	}
	else if (orb->mode == ORB_MODE_PLACE)
	{
		if (!finished && data->effects.current_door_anim != DOOR_OPEN)
			set_current_anim(data->effects.door_anims,
				&data->effects.current_door_anim, DOOR_OPEN);
		else if (finished && data->effects.current_door_anim == DOOR_OPEN)
			set_current_anim(data->effects.door_anims,
				&data->effects.current_door_anim, DOOR_OPENED);
	}
	else if (orb->mode == ORB_MODE_TAKE)
	{
		if (!finished && data->effects.current_door_anim != DOOR_OPEN)
			set_current_anim(data->effects.door_anims,
				&data->effects.current_door_anim, DOOR_OPEN);
		else if (finished && data->effects.current_door_anim == DOOR_OPEN)
			set_current_anim(data->effects.door_anims,
				&data->effects.current_door_anim, DOOR_CLOSE);
		else if (finished && data->effects.current_door_anim == DOOR_CLOSE)
			set_current_anim(data->effects.door_anims,
				&data->effects.current_door_anim, DOOR_CLOSED);
	}
}
