/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint_texture_pixel_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:48:29 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 01:04:34 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static uint32_t	sample_atlas_frame_pixel(t_atlas *atlas,
		unsigned int current_frame[2], int tex_x, float tex_pos)
{
	int			tex_y;
	int			pixel_index;
	uint8_t		*pixels;
	int			src[2];

	tex_y = (int)tex_pos;
	tex_x = (int)clamp((float)tex_x, 0.0f, (float)(atlas->frame_width - 1));
	tex_y = (int)clamp((float)tex_y, 0.0f, (float)(atlas->frame_height - 1));
	src[X] = (current_frame[X] * 128.0f) + tex_x;
	src[Y] = (current_frame[Y] * 128.0f) + tex_y;
	if (src[X] < 0 || src[X] >= (int)atlas->xpm->texture.width
		|| src[Y] < 0 || src[Y] >= (int)atlas->xpm->texture.height)
		return (MAGENTA);
	pixels = atlas->xpm->texture.pixels;
	pixel_index = (src[Y] * atlas->xpm->texture.width + src[X])
		* atlas->xpm->texture.bytes_per_pixel;
	return ((pixels[pixel_index] << 24)
		| (pixels[pixel_index + 1] << 16)
		| (pixels[pixel_index + 2] << 8)
		| pixels[pixel_index + 3]);
}

static uint32_t	sample_texture_pixel(xpm_t *texture, int tex_x, float tex_pos)
{
	int			tex_y;
	int			pixel_index;
	uint8_t		*pixels;
	uint32_t	pixel_color;

	tex_y = (int)tex_pos;
	tex_y = (int)clamp((float)tex_y, 0.0f,
			(float)(texture->texture.height - 1));
	if (tex_x >= 0 && tex_x < (int)texture->texture.width
		&& tex_y >= 0 && tex_y < (int)texture->texture.height)
	{
		pixel_index = (tex_y * texture->texture.width + tex_x)
			* texture->texture.bytes_per_pixel;
		if (pixel_index < (int)(texture->texture.width * texture->texture.height
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

static uint32_t	lerp_pixel_and_fog_bonus(int coord[2],
		uint32_t pixel_color, xpm_t *fog_texture, float fog_factor)
{
	int			fog_pixel[2];
	uint32_t	fog_sample;
	float		fog_mod;
	float		noise;
	float		strength;

	if (fog_texture)
	{
		fog_pixel[X] = (int)(coord[X] & (fog_texture->texture.width - 1));
		fog_pixel[Y] = (int)(coord[Y] & (fog_texture->texture.height - 1));
		fog_sample = sample_texture_pixel(fog_texture, fog_pixel[X],
				fog_pixel[Y]);
		noise = ((fog_sample >> 24) & 0xFF) * (1.0f / 255.0f);
		noise -= 0.5f;
		strength = 0.05f + 0.15f * fog_factor;
		fog_mod = fog_factor + noise * (strength * fog_factor);
		fog_mod = clamp(fog_mod, 0.0f, 0.95f);
		fog_sample = rgba_color_lerp(pixel_color, FOG_COLOR, fog_mod);
	}
	else
		fog_sample = rgba_color_lerp(pixel_color, FOG_COLOR, fog_factor);
	return (fog_sample);
}

void	paint_texture_pixel_bonus(int pixel[2], t_game *game,
		xpm_t *texture, int tex_coord[2], float fog, t_anim *anim)
{
	uint32_t	pixel_color;
	xpm_t		*fog_texture;
	uint32_t	fog_sample;
	mlx_image_t	*img;

	if (anim)
		pixel_color = sample_atlas_frame_pixel(
				anim->atlas, anim->current_frame, tex_coord[X], tex_coord[Y]);
	else
		pixel_color = sample_texture_pixel(texture, tex_coord[X], tex_coord[Y]);
	fog_texture = game->cub_data.textures.fog;
	fog_sample = lerp_pixel_and_fog_bonus(pixel, pixel_color,
			fog_texture, fog);
	img	= game->double_buffer[NEXT];
	safe_put_pixel(img, pixel[X], pixel[Y], fog_sample);
}
