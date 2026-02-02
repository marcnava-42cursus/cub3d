/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_anim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:16:11 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/31 00:32:13 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

static void	player_state_machine(t_player *player)
{
	if (player->state == STATE_EMPTY)
	{
		if (player->inventory)
			player->state = STATE_HOLD;
		else
			player->state = STATE_TAKE;
	}
	else if (player->state == STATE_TAKE)
	{
		if (player->inventory)
			player->state = STATE_HOLD;
		else
			player->state = STATE_TAKE;
	}
	else if (player->state == STATE_HOLD)
		player->state = STATE_THROW;
	else if (player->state == STATE_THROW)
		player->state = STATE_EMPTY;
}

void	set_player_anim(t_anim *anim_ptr, int *current_anim, int new_anim)
{
	if (*current_anim == new_anim)
		return ;
	*current_anim = new_anim;
	anim_start(&anim_ptr[*current_anim]);
}

void	init_player_anims(t_player *player)
{
	init_player_left_hand_anims(player);
	init_player_right_hand_anims(player);
}

void	update_player_anims(t_player *player, float delta_time)
{
	if (anim_update(&player->left_hand_anims[player->current_left_hand_anim],
			delta_time))
		player_state_machine(player);
	anim_update(&player->right_hand_anims[player->current_right_hand_anim],
		delta_time);
	set_player_left_hand_anims(player);
	set_player_right_hand_anims(player);
}

void	free_player_anims(t_player *player)
{
	free(player->left_hand_anims);
	free(player->right_hand_anims);
}
