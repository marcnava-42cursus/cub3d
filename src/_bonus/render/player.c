/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 03:03:22 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static void	render_left_hand(mlx_image_t *buffer, int buffer_x_center,
		t_player *player)
{
	int		coord[2];

	coord[X] = buffer_x_center - (HAND_TEXTURE_WIDTH * 2.0f);
	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT;
	frame_to_image(buffer, &player->hand_atlas, coord,
		player->left_hand_anims[player->current_left_hand_anim].current_frame);
}

//static void	render_left_thumb(mlx_image_t *buffer, int buffer_x_center,
//		t_player *player)
//{
//	int		coord[2];
//
//	coord[X] = buffer_x_center - (HAND_TEXTURE_WIDTH * 2.0f);
//	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT;
//	current_frame_to_image(buffer, &player->thumb_atlas, coord,
//		player->left_hand_anims[player->current_left_hand_anim].current_frame);
//}

static void	render_weapon(mlx_image_t *buffer, int buffer_x_center,
		t_effects *effects, t_player *player)
{
	int				coord[2];

	if (player->state != STATE_HOLD || !player->inventory)
		return ;
	coord[X] = buffer_x_center - (HAND_TEXTURE_WIDTH * 1.7);
	coord[Y] = buffer->height - (HAND_TEXTURE_HEIGHT)
		- (WEAPON_TEXTURE_HEIGHT * 0.5f);
	frame_to_image(buffer, &effects->orb_atlas, coord,
		effects->orb_anims[effects->current_orb_anim].current_frame);
}

static void	render_right_hand(mlx_image_t *buffer, int buffer_x_center,
		t_player *player)
{
	int		coord[2];

	coord[X] = buffer_x_center + HAND_TEXTURE_WIDTH;
	coord[Y] = buffer->height - HAND_TEXTURE_HEIGHT;
	hori_flip_frame_to_image(buffer, &player->hand_atlas, coord,
		player->right_hand_anims[
		player->current_right_hand_anim].current_frame);
}

void	render_player_overlay(t_game *game)
{
	mlx_image_t			*buffer;
	t_player			*player;
	t_effects			*effects;
	int					buffer_x_center;

	buffer = game->double_buffer[NEXT];
	player = &game->cub_data.player;
	effects = &game->cub_data.effects;
	buffer_x_center = buffer->width / 2;
	render_left_hand(buffer, buffer_x_center, player);
	render_weapon(buffer, buffer_x_center, effects, player);
	render_right_hand(buffer, buffer_x_center, player);
}
//render_left_thumb(buffer, buffer_x_center, player);
