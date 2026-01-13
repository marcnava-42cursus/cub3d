/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_anim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:16:11 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/04 15:44:27 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	init_empty_left_hand(t_game *game, int start_pos[2], xpm_t **frames)
{
	t_atlas *left_hand_atlas;

	left_hand_atlas = &game->cub_data.player.textures.left_hand;
	anim_init();
}
