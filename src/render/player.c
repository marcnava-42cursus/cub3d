/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2025/10/30 23:47:07 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void render_weapon(int pos_x, mlx_image_t *image, xpm_t* weapon)
{
	int	x;
	int y;
	int pos_y;

	pos_y = image->height - HAND_TEXTURE_HEIGHT - WEAPON_TEXTURE_HEIGHT * .5;
	y = 0;
	while (y < WEAPON_TEXTURE_HEIGHT && y + pos_y < image->height)
	{
		x = 0;
		while (x < WEAPON_TEXTURE_WIDTH && x + pos_x < image->width)
		{
			if (weapon != NULL)
				save_pixel_to_image(image, pos_x + x, pos_y + y,
						weapon->texture.pixels[y * weapon->texture.width + x]);
			else
				save_pixel_to_image(image, pos_x + x, pos_y + y, YELLOW);
			x++;
		}
		y++;
	}
}

static void render_player_part(int pos_x, int width, int height,
		mlx_image_t *image, xpm_t *part, unsigned int color)
{
	int	x;
	int y;
	int pos_y;

	pos_y = image->height - height;
	y = 0;
	while (y < height && y + pos_y < image->height)
	{
		x = 0;
		while (x < width && x + pos_x < image->width)
		{
			if (part != NULL)
				save_pixel_to_image(image, pos_x + x, pos_y + y,
						part->texture.pixels[y * part->texture.width + x]);
			else
				save_pixel_to_image(image, pos_x + x, pos_y + y, color);
			x++;
		}
		y++;
	}
}

void	render_player_overlay(t_game *game)
{
	mlx_image_t *buffer;
	t_player_textures textures;

	buffer = game->double_buffer[NEXT];
	textures = game->cub_data.player.textures;
	render_player_part(buffer->width / 2 - HAND_TEXTURE_WIDTH * 2, 	
			HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT, buffer,
			textures.left_hand, RED);
	render_weapon(buffer->width / 2 - HAND_TEXTURE_WIDTH * 1.7, buffer,
			textures.weapon);
	render_player_part(buffer->width / 2 - THUMB_TEXTURE_WIDTH * 3.75,
			THUMB_TEXTURE_WIDTH, THUMB_TEXTURE_HEIGHT, buffer,
			textures.left_thumb, MAGENTA);
	render_player_part(buffer->width / 2 + HAND_TEXTURE_WIDTH,
			HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT, buffer,
			textures.right_hand, RED);
}
