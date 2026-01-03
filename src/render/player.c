/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/03 11:56:43 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	render_player_overlay(t_game *game)
{
	mlx_image_t			*buffer;
	int					buffer_x_center;
	int					hand_thumb[2];
	t_player_textures	*textures;

	buffer = game->double_buffer[NEXT];
	textures = &game->cub_data.player.textures;
	buffer_x_center = buffer->width / 2;
	hand_thumb[X] = buffer_x_center - HAND_TEXTURE_WIDTH * 2;
	hand_thumb[Y] = buffer->height - HAND_TEXTURE_HEIGHT;
	paint_current_frame_to_image(buffer, &textures->left_hand, hand_thumb[X],
		hand_thumb[Y]);
	if (game->cub_data.player.inventory)
		paint_current_frame_to_image(buffer, &textures->weapon,
			buffer_x_center - HAND_TEXTURE_WIDTH * 1.7,
			buffer->height - HAND_TEXTURE_HEIGHT
			- WEAPON_TEXTURE_HEIGHT * 0.5f);
	paint_current_frame_to_image(buffer, &textures->left_thumb, hand_thumb[X],
		hand_thumb[Y]);
	paint_hori_flip_current_frame_to_image(buffer, &textures->left_hand,
		buffer_x_center + HAND_TEXTURE_WIDTH, hand_thumb[Y]);
}
