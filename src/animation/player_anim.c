/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_anim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:16:11 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/15 16:43:37 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

static void	set_player_anim(t_player *player, int new_anim)
{
	if (player->current_anim == new_anim)
		return ;
	player->current_anim = new_anim;
	anim_start(&player->anims[player->current_anim]);
}

void	init_player_anims(t_player *player)
{
	static const unsigned int	empty_frames[1] = {5};
	static const unsigned int	empty_holds[1] = {1};
	static const unsigned int	take_frames[3] =  {4, 6, 8};
	static const unsigned int	take_holds[3] =  {1, 1, 1};
	static const unsigned int	hold_frames[1] = {0};
	static const unsigned int	hold_holds[1] = {1};
	static const unsigned int	throw_frames[3] = {1, 2, 3};
	static const unsigned int	throw_holds[3] = {1, 1, 4};

	player->anims = ft_calloc(4, sizeof(t_anim));
	if (!player->anims) 
		return ;
	anim_init(&player->anims[ANIM_EMPTY], &player->textures.hand, empty_frames,
			empty_holds, 1, true);
	anim_init(&player->anims[ANIM_TAKE], &player->textures.hand, take_frames,
			take_holds, 4, false);
	anim_init(&player->anims[ANIM_HOLD], &player->textures.hand, hold_frames,
			hold_holds, 1, true);
	anim_init(&player->anims[ANIM_THROW], &player->textures.hand, throw_frames,
			throw_holds, 3, false);
	player->current_anim = ANIM_EMPTY;
}

void	update_player_anims(t_player *player, float delta_time)
{
	bool finished;

	finished = anim_update(&player->anims[player->current_anim], delta_time);
	if (finished)
		player->state = STATE_NONE;
	if (player->state == STATE_THROW)
		set_player_anim(player, ANIM_THROW);
	else if (player->state == STATE_TAKE)
		set_player_anim(player, ANIM_TAKE);
	else
	{
		if (player->inventory)
			set_player_anim(player, ANIM_HOLD);
		else
			set_player_anim(player, ANIM_EMPTY);
	}
}

void	free_player_anims(t_player *player)
{
	free(player->anims);
}
