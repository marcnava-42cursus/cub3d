/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:50:36 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/21 02:14:23 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void render_texture_line(t_rayhit rayhit, int x, int y[2],
	mlx_image_t *img, t_textures *textures)
{
	int		current_y;
	int		color;
	xpm_t	*texture;
	uint32_t	pixel_color;
	int		tex_x, tex_y;

	// Select texture based on wall face
	if (rayhit.face == NORTH)
	{
		texture = textures->north;
		color = LIGHT_GREY;
	}
	else if (rayhit.face == SOUTH)
	{
		texture = textures->south;
		color = BLACK;
	}
	else if (rayhit.face == EAST)
	{
		texture = textures->east;
		color = DARK_GREY;
	}
	else if (rayhit.face == WEST)
	{
		texture = textures->west;
		color = MEDIUM_GREY;
	}
	else
	{
		texture = NULL;
		color = WHITE;
	}
	
	// Bounds checking
	if (x < 0 || x >= (int)img->width)
		return ;
	if (y[0] < 0)
		y[0] = 0;
	if (y[1] >= (int)img->height)
		y[1] = (int)img->height - 1;
	if (y[0] >= y[1])
		return ;

	current_y = y[0];
	if (!texture)
	{
		// No texture loaded, use solid color
		while (current_y <= y[1])
		{
			mlx_put_pixel(img, x, current_y, color);
			current_y++;
		}
	}
	else
	{
		// Sample from texture
		while (current_y <= y[1])
		{
			// Calculate texture coordinates (simple mapping for now)
			tex_x = x % texture->texture.width;
			tex_y = current_y % texture->texture.height;
			
			// Get pixel from texture
			if (tex_x >= 0 && tex_x < (int)texture->texture.width &&
				tex_y >= 0 && tex_y < (int)texture->texture.height)
			{
				// Get pixel color from texture bytes
				int pixel_index = (tex_y * texture->texture.width + tex_x) * texture->texture.bytes_per_pixel;
				if (pixel_index < (int)(texture->texture.width * texture->texture.height * texture->texture.bytes_per_pixel))
				{
					// Convert bytes to color (assuming RGBA format)
					uint8_t *pixels = texture->texture.pixels;
					pixel_color = (pixels[pixel_index] << 24) |     // R
								  (pixels[pixel_index + 1] << 16) | // G
								  (pixels[pixel_index + 2] << 8) |  // B
								  pixels[pixel_index + 3];         // A
				}
				else
					pixel_color = color; // Fallback
			}
			else
				pixel_color = color; // Fallback
			
			mlx_put_pixel(img, x, current_y, pixel_color);
			current_y++;
		}
	}
}
