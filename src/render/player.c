/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/04 16:56:43 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	render_player_overlay(t_game *game)
{
	mlx_image_t			*buffer;
	int					buffer_x_center;
	int					hand_thumb[2];
	int					current_frame[2] = {0,0};
	int					rh_frame[2] = {2,1};
	int					weapon[2];
	int					flip_hand_thumb[2];
	t_player_textures	*textures;

	buffer = game->double_buffer[NEXT];
	textures = &game->cub_data.player.textures;
	buffer_x_center = buffer->width / 2;
	hand_thumb[X] = buffer_x_center - HAND_TEXTURE_WIDTH * 2;
	hand_thumb[Y] = buffer->height - HAND_TEXTURE_HEIGHT;
	weapon[X] = buffer_x_center - HAND_TEXTURE_WIDTH * 1.7;
	weapon[Y] = buffer_x_center - HAND_TEXTURE_WIDTH - WEAPON_TEXTURE_HEIGHT * 0.5f;
	flip_hand_thumb[X] = buffer_x_center + HAND_TEXTURE_WIDTH; 
	flip_hand_thumb[Y] = hand_thumb[Y];
	if (game->cub_data.player.inventory)
	{
		paint_current_frame_to_image(buffer, &textures->left_hand, hand_thumb, 
			current_frame);
		paint_current_frame_to_image(buffer, &textures->weapon, weapon,
				current_frame);
	}
	else
		paint_current_frame_to_image(buffer, &textures->left_hand, hand_thumb, 
			rh_frame);
	//paint_current_frame_to_image(buffer, &textures->left_thumb, hand_thumb,
	//	current_frame);
	paint_hori_flip_current_frame_to_image(buffer, &textures->left_hand,
			flip_hand_thumb, rh_frame);
}
