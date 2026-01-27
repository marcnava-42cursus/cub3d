/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_right_hand_anims.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:24:26 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/27 02:01:48 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	init_player_right_hand_anims(t_player *player)
{
	static const unsigned int	empty_frames[1] = {5};
	static const unsigned int	empty_holds[1] = {1};
	static const unsigned int	take_frames[4] =  {4, 6, 7};
	static const unsigned int	take_holds[4] =  {1, 1, 2};
	static const unsigned int	hold_frames[1] = {5};
	static const unsigned int	hold_holds[1] = {1};
	static const unsigned int	throw_frames[2] = {6, 7};
	static const unsigned int	throw_holds[2] = {1, 2};

	player->right_hand_anims = ft_calloc(4, sizeof(t_anim));
	if (!player->right_hand_anims) 
		return ;
	anim_init(&player->right_hand_anims[ANIM_EMPTY], &player->textures.hand, empty_frames,
			empty_holds, 1, true);
	anim_init(&player->right_hand_anims[ANIM_TAKE], &player->textures.hand, take_frames,
			take_holds, 3, false);
	anim_init(&player->right_hand_anims[ANIM_HOLD], &player->textures.hand, hold_frames,
			hold_holds, 1, true);
	anim_init(&player->right_hand_anims[ANIM_THROW], &player->textures.hand, throw_frames,
			throw_holds, 2, false);
	player->current_right_hand_anim = ANIM_HOLD;
}

void	update_player_right_hand_anims(t_player *player, float delta_time)
{
	bool finished;

	finished = anim_update(
			&player->right_hand_anims[player->current_right_hand_anim],
			delta_time);
	if (player->state == STATE_THROW)
		set_player_anim(player->right_hand_anims,
				&player->current_right_hand_anim, ANIM_THROW);
	else if (player->state == STATE_TAKE)
		set_player_anim(player->right_hand_anims,
				&player->current_right_hand_anim, ANIM_TAKE);
	else if (player->state == STATE_HOLD)
		set_player_anim(player->right_hand_anims,
				&player->current_right_hand_anim, ANIM_HOLD);
	else
		set_player_anim(player->right_hand_anims,
				&player->current_right_hand_anim, ANIM_EMPTY);
}
