/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_anims.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:47:25 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/02 16:39:02 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

static void	init_orb_shoot_anims(t_effects *effects)
{
	static const unsigned int	spawn_frames[6] = {0, 1, 2, 3, 4, 5};
	static const unsigned int	spawn_holds[6] = {1, 1, 1, 1, 2, 1};
	static const unsigned int	loop_frames[4] = {6, 7, 8, 9};
	static const unsigned int	loop_holds[4] = {1, 1, 1, 1};

	anim_init(&effects->orb_anims[ORB_SPAWN], &effects->orb_atlas, false);
	store_anim_frame_data(&effects->orb_anims[ORB_SPAWN], spawn_frames,
		spawn_holds, 6);
	anim_init(&effects->orb_anims[ORB_LOOP], &effects->orb_atlas, true);
	store_anim_frame_data(&effects->orb_anims[ORB_LOOP], loop_frames,
		loop_holds, 4);
}

static void	init_orb_hold_anim(t_effects *effects)
{
	static const unsigned int	hold_frames[6] = {10, 11, 12, 13, 14, 15};
	static const unsigned int	hold_holds[6] = {2, 2, 2, 2, 2, 2};

	anim_init(&effects->orb_anims[ORB_HOLD], &effects->orb_atlas, true);
	store_anim_frame_data(&effects->orb_anims[ORB_HOLD], hold_frames,
		hold_holds, 6);
}

void	init_orb_anims(t_effects *effects)
{
	effects->orb_anims = ft_calloc(3, sizeof(t_anim));
	if (!effects->orb_anims)
		return ;
	init_orb_shoot_anims(effects);
	init_orb_hold_anim(effects);
	effects->current_orb_anim = ORB_HOLD;
}

static void	set_orb_anim(t_anim *anim_ptr, int *current_anim, int new_anim)
{
	if (*current_anim == new_anim)
		return ;
	*current_anim = new_anim;
	anim_start(&anim_ptr[*current_anim]);
}

void	update_orb_anims(t_orb_projectile *orb, t_effects *effects,
		float delta_time)
{
	bool	finished;

	finished = anim_update(&effects->orb_anims[effects->current_orb_anim],
			delta_time);
	if (orb->active)
	{
		if (orb->mode == ORB_MODE_PLACE)
			set_orb_anim(effects->orb_anims, &effects->current_orb_anim,
				ORB_LOOP);
		else if (orb->mode == ORB_MODE_TAKE)
		{
			if (effects->current_orb_anim != ORB_SPAWN 
					&& effects->current_orb_anim != ORB_LOOP && !finished)
				set_orb_anim(effects->orb_anims, &effects->current_orb_anim,
					ORB_SPAWN);
			else if (effects->current_orb_anim == ORB_SPAWN && finished)
				set_orb_anim(effects->orb_anims, &effects->current_orb_anim,
					ORB_LOOP);
		}
	}
	else
		set_orb_anim(effects->orb_anims, &effects->current_orb_anim, ORB_HOLD);
}

void	free_orb_anims(t_effects *effects)
{
	free(effects->orb_anims);
}
