/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/03 02:33:00 by marcnava         ###   ########.fr       */
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
			{
				uint32_t pixel_color = sample_texture_pixel(weapon, x, y);
				if (pixel_color != 0x00000000)
					save_pixel_to_image(image, pos_x + x, pos_y + y,
							pixel_color);
			}
			else
				save_pixel_to_image(image, pos_x + x, pos_y + y, YELLOW);
			x++;
		}
		y++;
	}
}

static void render_player_part(int pos_x, mlx_image_t *image, xpm_t *part,
		unsigned int color)
{
	int	x;
	int y;
	int pos_y;

	pos_y = image->height - part->texture.height;
	y = 0;
	while (y < part->texture.height && y + pos_y < image->height)
	{
		x = 0;
		while (x < part->texture.width && x + pos_x < image->width)
		{
			if (part != NULL)
			{
				uint32_t pixel_color = sample_texture_pixel(part, x, y);
				if (pixel_color != 0x00000000)
					save_pixel_to_image(image, pos_x + x, pos_y + y,
							pixel_color);
			}
			else
				save_pixel_to_image(image, pos_x + x, pos_y + y, color);
			x++;
		}
		y++;
	}
}

static void render_player_part_flip(int pos_x, mlx_image_t *image, xpm_t *part,
		unsigned int color)
{
	int	x;
	int	x_print;
	int y;
	int pos_y;

	pos_y = image->height - part->texture.height;
	y = 0;
	while (y < part->texture.height && y + pos_y < image->height)
	{
		x = part->texture.width - 1;
		x_print = 0;
		while (x >= 0 && x + pos_x < image->width)
		{
			if (part != NULL)
			{
				uint32_t pixel_color = sample_texture_pixel(part, x, y);
				if (pixel_color != 0x00000000)
					save_pixel_to_image(image, pos_x + x_print, pos_y + y,
							pixel_color);
			}
			else
				save_pixel_to_image(image, pos_x + x_print, pos_y + y, color);
			x--;
			x_print++;
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
	render_player_part(buffer->width / 2 - HAND_TEXTURE_WIDTH * 2, 	buffer, 
			textures.left_hand, RED);
	if (game->cub_data.player.inventory)
		render_weapon(buffer->width / 2 - HAND_TEXTURE_WIDTH * 1.7, buffer,
			textures.weapon);
	render_player_part(buffer->width / 2 - HAND_TEXTURE_WIDTH * 2, buffer,
			textures.left_thumb, MAGENTA);
	render_player_part_flip(buffer->width / 2 + HAND_TEXTURE_WIDTH, buffer,
			textures.left_hand, RED);
}
