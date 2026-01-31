/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_right_hand_anims.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:24:26 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/31 13:51:56 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

static void	init_empty_and_hold(t_player *player)
{
	static const unsigned int	empty_frames[1] = {5};
	static const unsigned int	empty_holds[1] = {1};
	static const unsigned int	hold_frames[1] = {5};
	static const unsigned int	hold_holds[1] = {1};

	anim_init(&player->right_hand_anims[ANIM_EMPTY], &player->textures.hand,
		true);
	store_anim_frame_data(&player->right_hand_anims[ANIM_EMPTY], empty_frames,
		empty_holds, 1);
	anim_init(&player->right_hand_anims[ANIM_HOLD], &player->textures.hand,
		true);
	store_anim_frame_data(&player->right_hand_anims[ANIM_HOLD], hold_frames,
		hold_holds, 1);
}

static void	init_take_and_throw(t_player *player)
{
	static const unsigned int	take_frames[4] = {4, 6, 7};
	static const unsigned int	take_holds[4] = {1, 1, 2};
	static const unsigned int	throw_frames[2] = {6, 7};
	static const unsigned int	throw_holds[2] = {1, 2};

	anim_init(&player->right_hand_anims[ANIM_TAKE], &player->textures.hand,
		false);
	store_anim_frame_data(&player->right_hand_anims[ANIM_TAKE], take_frames,
		take_holds, 3);
	anim_init(&player->right_hand_anims[ANIM_THROW], &player->textures.hand,
		false);
	store_anim_frame_data(&player->right_hand_anims[ANIM_THROW], throw_frames,
		throw_holds, 2);
}

void	init_player_right_hand_anims(t_player *player)
{
	player->right_hand_anims = ft_calloc(4, sizeof(t_anim));
	if (!player->right_hand_anims)
		return ;
	init_empty_and_hold(player);
	init_take_and_throw(player);
	player->current_right_hand_anim = ANIM_HOLD;
}

void	set_player_right_hand_anims(t_player *player)
{
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
