/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_anim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:16:11 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/15 21:00:53 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	set_player_anim(t_player *player, t_anim *anim_ptr, int *current_anim,
			int new_anim)
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
	update_player_left_hand_anims(player, delta_time);
	update_player_right_hand_anims(player, delta_time);
}

void	free_player_anims(t_player *player)
{
	free(player->left_hand_anims);
	free(player->right_hand_anims);
}
