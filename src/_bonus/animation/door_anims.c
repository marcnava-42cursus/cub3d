/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_anims.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:39:36 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 12:43:32 by ivmirand         ###   ########.fr       */
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

void	update_door_anims(t_player *player, t_orb_projectile *orb,
		t_effects *effects, float delta_time)
{
	int		player_cell[2];
	bool	finished;

	player_cell[X] = (int)(player->x / WORLDMAP_TILE_SIZE);
	player_cell[Y] = (int)(player->y / WORLDMAP_TILE_SIZE);
	finished = anim_update(&effects->door_anims[effects->current_door_anim],
			delta_time);
	if (orb->elevator_place)
	{
		set_current_anim(effects->door_anims, &effects->current_door_anim,
			DOOR_OPEN);
	}
	//if (/*distance between player and door is below 3 tiles*/)		
	//{
	//	if (current_door_anim == DOOR_CLOSED)
	//		set_current_anim(&door_anims, current_door_anim, DOOR_OPEN);
	//	else if (current_door_anim == DOOR_CLOSE && finished)
	//		set_current_anim(&door_anims current_door_anim, DOOR_CLOSED);
	//}
}
