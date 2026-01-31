/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absorb_anims.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 18:57:12 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/31 20:14:32 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	init_absorb_anims(t_effects *effects)
{
	static const unsigned int	absorb_frames[9] = {8, 7, 6, 5, 4, 3, 2, 1, 0};	
	static const unsigned int	absorb_holds[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};	
	static const unsigned int	spawn_frames[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};	
	static const unsigned int	spawn_holds[9] =  {1, 1, 1, 1, 1, 1, 1, 1, 1};
	
	effects->anims = ft_calloc(2, sizeof(t_anim));
	if (!effects->anims)
		return ;

	anim_init(&effects->anims[ANIM_ABSORB], &effects->atlas, true);
	store_anim_frame_data(&effects->anims[ANIM_ABSORB], absorb_frames,
		absorb_holds, 9);
	anim_start(&effects->anims[ANIM_ABSORB]);
	anim_init(&effects->anims[ANIM_SPAWN], &effects->atlas, true);
	store_anim_frame_data(&effects->anims[ANIM_SPAWN], spawn_frames,
		spawn_holds, 9);
	anim_start(&effects->anims[ANIM_SPAWN]);
}

void	update_absorb_anims(t_player *player, t_effects *effects,
		float delta_time)
{
	if (player->inventory)
		effects->current_anim = ANIM_ABSORB;
	else
		effects->current_anim = ANIM_SPAWN;
	anim_update(&effects->anims[effects->current_anim], delta_time);
}

void	free_effects_anims(t_anim *effects)
{
	free(effects);
}
