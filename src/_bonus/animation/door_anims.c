/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_anims.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:39:36 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 02:43:34 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

static void	init_open_anims(t_effects *effects)
{
	static const unsigned int	opened_frames[1] = {0};
	static const unsigned int	opened_holds[1] = {1};
	static const unsigned int	open_frames[7] = {1, 2, 3, 4, 5, 6, 7};
	static const unsigned int	open_holds[7] = {1, 1, 1, 1, 1, 1, 1};

	anim_init(&effects->door_anims[DOOR_OPENED], &effects->door_atlas, true);
	store_anim_frame_data(&effects->door_anims[DOOR_OPENED], opened_frames,
		opened_holds, 1);
	anim_init(&effects->door_anims[DOOR_OPEN], &effects->door_atlas, false);
	store_anim_frame_data(&effects->door_anims[DOOR_OPEN], open_frames,
		open_holds, 7);
}

static void	init_close_anims(t_effects *effects)
{
	static const unsigned int	closed_frames[1] = {8};
	static const unsigned int	closed_holds[1] = {1};
	static const unsigned int	close_frames[7] = {7, 6, 5, 4, 3, 2, 1};
	static const unsigned int	close_holds[7] = {1, 1, 1, 1, 1, 1, 1};

	anim_init(&effects->door_anims[DOOR_CLOSED], &effects->door_atlas, true);
	store_anim_frame_data(&effects->door_anims[DOOR_CLOSED], closed_frames,
		closed_holds, 1);
	anim_init(&effects->door_anims[DOOR_CLOSE], &effects->door_atlas, false);
	store_anim_frame_data(&effects->door_anims[DOOR_CLOSE], close_frames,
		close_holds, 7);
}

void	init_door_anims(t_effects *effects)
{
	effects->door_anims = ft_calloc(4, sizeof(t_anim));
	if (!effects->door_anims)
		return ;
	init_close_anims(effects);
	init_open_anims(effects);
}

//void	update_door_anims(t_player *player, t_effects *effects,
//		float delta_time)
//{
//	int	player[2];
//
//	player[X] = (int)(player->x / WORLDMAP_TILE_SIZE);
//	player[Y] = (int)(player->y / WORLDMAP_TILE_SIZE);
//	if (/*distance between player and door is below 3 tiles*/)		
//	{
//		if (current_door_anim == DOOR_CLOSED)
//			set_door_anim(&door_anims, current_door_anim, DOOR_OPEN);
//		else if (current_door_anim == DOOR_CLOSE && finished)
//			set_door_anim(&door_anims current_door_anim, DOOR_CLOSED);
//	}
//}
