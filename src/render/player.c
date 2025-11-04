/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:04 by ivmirand          #+#    #+#             */
/*   Updated: 2025/11/04 19:01:09 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

//stolen from texture_mapping.c
static uint32_t	sample_texture_pixel(xpm_t *texture, int tex_x, float tex_pos)
{
	int			tex_y;
	int			pixel_index;
	uint8_t		*pixels;
	uint32_t	pixel_color;

	tex_y = (int)tex_pos;
	if (tex_y >= (int)texture->texture.height)
		tex_y = texture->texture.height - 1;

	if (tex_x >= 0 && tex_x < (int)texture->texture.width
		&& tex_y >= 0 && tex_y < (int)texture->texture.height)
	{
		pixel_index = (tex_y * texture->texture.width + tex_x)
			* texture->texture.bytes_per_pixel;
		if (pixel_index < (int)(texture->texture.width
			* texture->texture.height
			* texture->texture.bytes_per_pixel))
		{
			pixels = texture->texture.pixels;
			pixel_color = (pixels[pixel_index] << 24)
				| (pixels[pixel_index + 1] << 16)
				| (pixels[pixel_index + 2] << 8)
				| pixels[pixel_index + 3];
			return (pixel_color);
		}
	}
	return (MAGENTA);
}

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
	render_weapon(buffer->width / 2 - HAND_TEXTURE_WIDTH * 1.7, buffer,
			textures.weapon);
	//render_player_part(buffer->width / 2 - THUMB_TEXTURE_WIDTH * 3.75, buffer,
	//		textures.left_thumb, MAGENTA);
	render_player_part_flip(buffer->width / 2 + HAND_TEXTURE_WIDTH, buffer,
			textures.left_hand, RED);
}
