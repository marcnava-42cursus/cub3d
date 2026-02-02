/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/02 14:02:38 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	render_left_hand(mlx_image_t *buffer, int buffer_x_center,
		t_atlas *atlas, t_player *player)
{
	int		coord[2];
	float	scale[2];

	scale[X] = (float)buffer->width / (float)PLAYER_REF_WIDTH;
	scale[Y] = (float)buffer->height / (float)PLAYER_REF_HEIGHT;
	coord[X] = buffer_x_center - (HAND_TEXTURE_WIDTH * 2.0f * scale[X]);
	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT * scale[Y];
	paint_current_frame_to_image(buffer, atlas, coord,
		player->left_hand_anims[player->current_left_hand_anim].current_frame,
		0.5f);
}

static void	render_weapon(mlx_image_t *buffer, int buffer_x_center,
		t_atlas *atlas, int current_frame[2], t_player *player)
{
	int				coord[2];
	float			scale[2];

	scale[X] = (float)buffer->width / (float)PLAYER_REF_WIDTH;
	scale[Y] = (float)buffer->height / (float)PLAYER_REF_HEIGHT;
	if (player->state != STATE_HOLD || !player->inventory)
		return ;
	coord[X] = buffer_x_center - (HAND_TEXTURE_WIDTH * 1.7 * scale[X]);
	coord[Y] = buffer->height - (HAND_TEXTURE_HEIGHT * scale[Y])
		- (WEAPON_TEXTURE_HEIGHT * 0.5f) * scale[Y];
	paint_current_frame_to_image(buffer, atlas, coord, current_frame, 0.5f);
}

static void	render_right_hand(mlx_image_t *buffer, int buffer_x_center,
		t_atlas *atlas, t_player *player)
{
	int		coord[2];
	float	scale[2];

	scale[X] = (float)buffer->width / (float)PLAYER_REF_WIDTH;
	scale[Y] = (float)buffer->height / (float)PLAYER_REF_HEIGHT;
	coord[X] = buffer_x_center + HAND_TEXTURE_WIDTH * scale[X];
	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT * scale[Y];
	paint_hori_flip_current_frame_to_image(buffer, atlas, coord,
		player->right_hand_anims[
		player->current_right_hand_anim].current_frame, 0.5f);
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
	render_weapon(buffer, buffer_x_center,
			&game->cub_data.effects.orb_atlas,
			game->cub_data.effects.orb_anims[
			game->cub_data.effects.current_orb_anim].current_frame,
			player);
	render_right_hand(buffer, buffer_x_center, &textures->hand, player);
}
//paint_current_frame_to_image(buffer, &textures->thumb, hand_thumb,
//	current_frame);
