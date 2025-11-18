/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2025/11/18 18:24:11 by ivmirand         ###   ########.fr       */
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
				| pixels[pixel_index + 3];
			return (pixel_color);
		}
	}
	return (MAGENTA);
}

// Textured vertical line painter - x[0] is start and x[1] is end
static void paint_horizontal_line_texture(unsigned int y, unsigned int x, mlx_image_t *img,
		xpm_t *texture, int tex_y, float tex_x)
{
	uint32_t pixel_color;
	
	pixel_color = sample_texture_pixel(texture, tex_y, tex_x);
	save_pixel_to_image(img, x, y, pixel_color);
}


static void	render_floor_fill(t_rayhit rayhit, unsigned int x, mlx_image_t *img,
		t_textures *textures)
{
	int		y;
	float	dist_to_proj_plane;
	float	weight;
	float	curr_floor_x;
	float	curr_floor_y;
	int		floor_tex_x;
	int		floor_tex_y;
	
	y = img->height / 2;
	while (y < img->height)
	{
		dist_to_proj_plane = (float)img->height / (2.0f * y - img->height);
		weight = dist_to_proj_plane / rayhit.distance;
		curr_floor_x = weight * rayhit.cell_x;
		curr_floor_y = weight * rayhit.cell_y;
		floor_tex_x = (int)(curr_floor_x * textures->north->texture.width);
		floor_tex_y = (int)(curr_floor_y * textures->north->texture.height);
		paint_horizontal_line_texture(y, x, img, textures->north, floor_tex_y, floor_tex_x);
		y++;
	}
}

void	render_floors(t_game *game, t_rayhit *rayhits)
{
	unsigned int	i;

	i = 0;
	while (i < game->double_buffer[NEXT]->width)
	{
		render_floor_fill(rayhits[i], i, game->double_buffer[NEXT],
			&game->cub_data.textures);
		i++;
	}
}
