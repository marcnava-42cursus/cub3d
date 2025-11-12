/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2025/11/12 23:04:49 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	fisheye_correction(t_rayhit *rayhit, float camera_x)
{
	float angle_from_center;

	if (rayhit->hit)
	{
		angle_from_center = atanf(camera_x * tanf(PLAYER_FOV / 2.0f));
		rayhit->distance *= cosf(angle_from_center);
	}
}

static t_rayhit	cast_ray_for_row(t_cub_data *cub_data, int y, int image_height)
{
	const float	MAX_DIST = 2000.0f;
	t_rayhit	rayhit;
	vertex_t	player_position;
	float		ray_angle;
	float		camera_y;

	camera_y = 2.0f * y / (float)image_height - 1.0f;
	ray_angle = cub_data->player.angle
		+ atanf(camera_y * tanf(PLAYER_FOV / 2.0f));
	player_position.x = ((float)cub_data->player.x + 0.2f) * WORLDMAP_TILE_SIZE;
	player_position.y = ((float)cub_data->player.y + 0.2f) * WORLDMAP_TILE_SIZE;
	rayhit = raycast_world(&cub_data->map, player_position, ray_angle,
			MAX_DIST);
	fisheye_correction(&rayhit, camera_y);
	return (rayhit);
}

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

// Textured vertical line painter - x[0] is start and x[1] is end
static void paint_horizontal_line_texture(unsigned int y, int x[2], mlx_image_t *img,
		xpm_t *texture, int tex_y, float tex_pos, float tex_step)
{
	int	current_x;
	float	current_tex_pos;
	
	if (y >= img->height)
		return ;
	if (x[1] >= (int)img->width)
		x[1] = (int)img->width - 1;
	if (x[0] < 0)
		x[0] = 0;
	if (x[0] >= x[1])
		return ;
	current_x = x[0];
	current_tex_pos = tex_pos;
	while (current_x <= x[1])
	{
		uint32_t pixel_color = sample_texture_pixel(texture, tex_y, current_tex_pos);
		save_pixel_to_image(img, y, (unsigned int)current_x, pixel_color);
		current_x++;
		current_tex_pos += tex_step;
	}
}

static void	render_texture_hori(t_rayhit rayhit, unsigned int y, int x[2],
		int original_x[2], mlx_image_t *img, t_textures *textures)
{
	uint32_t	color;
	xpm_t		*texture;
	int			tex_y;
	int			original_line_width;
	float		wall_y;
	float		step;
	float		tex_offset;

	texture = textures->north;

	if (y >= img->height)
		return ;
	if (x[1] >= (int)img->width)
		x[1] = (int)img->width - 1;
	if (x[0] >= x[1])
		return ;

	if (texture)
	{
		if (rayhit.side == 0)
			wall_y = rayhit.position.x / WORLDMAP_TILE_SIZE;
		else
			wall_y = rayhit.position.y / WORLDMAP_TILE_SIZE;
		wall_y = wall_y - floorf(wall_y);

		tex_y = (int)(wall_y * (float)texture->texture.width);
		if ((rayhit.side == 0 && rayhit.face == NORTH)
			|| (rayhit.side == 1 && rayhit.face == WEST))
			tex_y = texture->texture.width - tex_y - 1;

		// Calculate texture step and starting position for Y using original bounds
		original_line_width = original_x[1] - original_x[0] + 1;
		step = (float)texture->texture.width / (float)original_line_width;
		tex_offset = (x[0] - original_x[0]) * step;

		paint_horizontal_line_texture(y, x, img, texture, tex_y, tex_offset, step);
	}
}

static void	render_floor_fill(t_rayhit rayhit, unsigned int y, mlx_image_t *img,
		t_textures *textures)
{
	int			slice_width;
	int			screen_bounds[2];
	int			floor_bounds[2];
	float		floor_width;
	float		dist_to_proj_plane;
	float		current_aspect_ratio;
	float		aspect_scale;

	if (!rayhit.hit)
		return ;
	
	// Calculate distance to projection plane for proper perspective
	// Scale wall height dynamically based on image aspect ratio for proper cube proportions
	current_aspect_ratio = (float)img->width / (float)img->height;
	aspect_scale = current_aspect_ratio / BASE_ASPECT_RATIO;
	dist_to_proj_plane = (float)img->height / (2.0f * tanf(PLAYER_FOV / 2.0f));

	// Dynamic scaling with taller base
	floor_width = WORLDMAP_TILE_SIZE * (1.25f + aspect_scale * 0.5f);
	slice_width = (int)(floor_width * dist_to_proj_plane / rayhit.distance);

	// Calculate full wall slice bounds (may extend beyond image)
	floor_bounds[0] = -slice_width / 2 + (int)img->width / 2;
	floor_bounds[1] = slice_width / 2 + (int)img->width / 2;
	
	// Copy wall bounds to screen bounds for clipping
	screen_bounds[0] = floor_bounds[0];
	screen_bounds[1] = floor_bounds[1];
	
	if (screen_bounds[0] < 0)
		screen_bounds[0] = 0;
	if (screen_bounds[1] >= (int)img->width)
		screen_bounds[1] = (int)img->width - 1;
	
	if (screen_bounds[0] > screen_bounds[1]
		|| screen_bounds[1] < 0 || screen_bounds[0] >= (int)img->width)
		return;
	
	if (screen_bounds[0] < (int)img->width - 1)
		screen_bounds[0]++;
	if (screen_bounds[1] > 0 && screen_bounds[1] > screen_bounds[0])
		screen_bounds[1]--;
	
	if (screen_bounds[0] <= screen_bounds[1])
		render_texture_hori(rayhit, y, screen_bounds, floor_bounds, img, textures);
}

void	render_floors(t_game *game)
{
	unsigned int	i;
	t_rayhit		rayhits[MAX_WINDOW_WIDTH];

	i = 0;
	while (i < game->double_buffer[NEXT]->height && i < MAX_WINDOW_HEIGHT)
	{
		rayhits[i] = cast_ray_for_row(&game->cub_data, i,
				game->double_buffer[NEXT]->height);
		render_floor_fill(rayhits[i], i, game->double_buffer[NEXT],
			&game->cub_data.textures);
		i++;
	}
}
