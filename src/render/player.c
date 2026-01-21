/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/15 20:57:51 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	render_left_hand(mlx_image_t *buffer, int buffer_x_center,
		t_atlas *atlas, t_player *player)
{
	int	coord[2];

	coord[X] = buffer_x_center - HAND_TEXTURE_WIDTH * 2;
	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT;
	paint_current_frame_to_image(buffer, atlas, coord,
			player->left_hand_anims[player->current_left_hand_anim].current_frame);
}

static void	render_weapon(mlx_image_t *buffer, int buffer_x_center,
		t_atlas *atlas, t_player *player)
{
	int				coord[2];
	unsigned int	current_frame[2];

	if (player->state != STATE_HOLD || !player->inventory)
		return ;
	coord[X] = buffer_x_center - HAND_TEXTURE_WIDTH * 1.7;
	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT
		- WEAPON_TEXTURE_HEIGHT * 0.5f;
	current_frame[X] = 0;
	current_frame[Y] = 0;
	paint_current_frame_to_image(buffer, atlas, coord, current_frame);
}

static void	render_right_hand(mlx_image_t *buffer, int buffer_x_center,
		t_atlas *atlas, t_player *player)
{
	int				coord[2];
	unsigned int	current_frame[2];

	coord[X] = buffer_x_center + HAND_TEXTURE_WIDTH;
	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT;
	paint_hori_flip_current_frame_to_image(buffer, atlas, coord,
			player->right_hand_anims[player->current_right_hand_anim].current_frame);
}

void	render_player_overlay(t_game *game)
{
	mlx_image_t			*buffer;
	int					buffer_x_center;
	t_player			*player;
	t_player_textures	*textures;

	buffer = game->double_buffer[NEXT];
	buffer_x_center = buffer->width / 2;
	player = &game->cub_data.player;
	textures = &player->textures;
	render_left_hand(buffer, buffer_x_center, &textures->hand, player);
	render_weapon(buffer, buffer_x_center, &textures->weapon, player);
	render_right_hand(buffer, buffer_x_center, &textures->hand, player);
}
//paint_current_frame_to_image(buffer, &textures->thumb, hand_thumb,
//	current_frame);
