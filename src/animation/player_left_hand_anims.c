/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_left_hand_anims.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:24:26 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/27 06:28:21 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	init_player_left_hand_anims(t_player *player)
{
	static const unsigned int	empty_frames[1] = {5};
	static const unsigned int	empty_holds[1] = {1};
	static const unsigned int	take_frames[4] =  {5, 6, 8};
	static const unsigned int	take_holds[4] =  {4, 1, 2};
	static const unsigned int	hold_frames[1] = {0};
	static const unsigned int	hold_holds[1] = {1};
	static const unsigned int	throw_frames[3] = {1, 2, 3};
	static const unsigned int	throw_holds[3] = {2, 1, 3};

	player->left_hand_anims = ft_calloc(4, sizeof(t_anim));
	if (!player->left_hand_anims) 
		return ;
	anim_init(&player->left_hand_anims[ANIM_EMPTY], &player->textures.hand, empty_frames,
			empty_holds, 1, true);
	anim_init(&player->left_hand_anims[ANIM_TAKE], &player->textures.hand, take_frames,
			take_holds, 3, false);
	anim_init(&player->left_hand_anims[ANIM_HOLD], &player->textures.hand, hold_frames,
			hold_holds, 1, true);
	anim_init(&player->left_hand_anims[ANIM_THROW], &player->textures.hand, throw_frames,
			throw_holds, 3, false);
	player->current_left_hand_anim = ANIM_HOLD;
}

void	update_player_left_hand_anims(t_player *player, float delta_time)
{
	bool finished;

	finished = anim_update(
			&player->left_hand_anims[player->current_left_hand_anim],
			delta_time);
	if (finished)
	{
		if (player->state = STATE_EMPTY)
		{
			if (player->inventory)
				player->state = STATE_HOLD;
			else
				player->state = STATE_TAKE;
		}
		else if (player->state = STATE_TAKE)
			player->state = STATE_HOLD;
		
		else if (player->state = STATE_HOLD)
			player->state = STATE_THROW;
		else if (player->state = STATE_THROW)

			player->state = STATE_EMPTY;
	}
	if (player->state == STATE_THROW)
		set_player_anim(player->left_hand_anims,
				&player->current_left_hand_anim, ANIM_THROW);
	else if (player->state == STATE_TAKE)
		set_player_anim(player->left_hand_anims,
				&player->current_left_hand_anim, ANIM_TAKE);
	else if (player->state == STATE_HOLD)
		set_player_anim(player->left_hand_anims,
				&player->current_left_hand_anim, ANIM_HOLD);
	else
		set_player_anim(player->left_hand_anims, 
				&player->current_left_hand_anim, ANIM_EMPTY);
}
