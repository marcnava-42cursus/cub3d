/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:51:39 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/29 23:47:37 by ivmirand         ###   ########.fr       */
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

static t_rayhit	cast_ray_for_column(t_cub_data *cub_data, int x, int image_width)
{
	const float	MAX_DIST = 2000.0f;
	t_rayhit	rayhit;
	vertex_t	player_position;
	float		ray_angle;
	float		camera_x;

	camera_x = 2.0f * x / (float)image_width - 1.0f;
	ray_angle = cub_data->player.angle
		+ atanf(camera_x * tanf(PLAYER_FOV / 2.0f));
	player_position.x = ((float)cub_data->player.x + 0.5f) * WORLDMAP_TILE_SIZE;
	player_position.y = ((float)cub_data->player.y + 0.5f) * WORLDMAP_TILE_SIZE;
	rayhit = raycast_world(&cub_data->map, player_position, ray_angle,
			MAX_DIST);
	fisheye_correction(&rayhit, camera_x);
	return (rayhit);
}

static void	render_wall_fill(t_rayhit rayhit, unsigned int x, mlx_image_t *img,
		t_textures *textures)
{
	int			slice_height;
	int			screen_bounds[2];
	int			wall_bounds[2];
	float		wall_height;
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
	wall_height = WORLDMAP_TILE_SIZE * (1.25f + aspect_scale * 0.5f);
	slice_height = (int)(wall_height * dist_to_proj_plane / rayhit.distance);

	// Calculate full wall slice bounds (may extend beyond image)
	wall_bounds[0] = -slice_height / 2 + (int)img->height / 2;
	wall_bounds[1] = slice_height / 2 + (int)img->height / 2;
	
	// Copy wall bounds to screen bounds for clipping
	screen_bounds[0] = wall_bounds[0];
	screen_bounds[1] = wall_bounds[1];
	
	if (screen_bounds[0] < 0)
		screen_bounds[0] = 0;
	if (screen_bounds[1] >= (int)img->height)
		screen_bounds[1] = (int)img->height - 1;
	
	if (screen_bounds[0] > screen_bounds[1]
		|| screen_bounds[1] < 0 || screen_bounds[0] >= (int)img->height)
		return;
	
	if (screen_bounds[0] >= 0 && screen_bounds[0] < (int)img->height)
		save_pixel_to_image(img, x, (unsigned int)screen_bounds[0], BLACK);
	if (screen_bounds[1] >= 0 && screen_bounds[1] < (int)img->height
		&& screen_bounds[1] != screen_bounds[0])
		save_pixel_to_image(img, x, (unsigned int)screen_bounds[1], BLACK);
	
	if (screen_bounds[0] < (int)img->height - 1)
		screen_bounds[0]++;
	if (screen_bounds[1] > 0 && screen_bounds[1] > screen_bounds[0])
		screen_bounds[1]--;
	
	if (screen_bounds[0] <= screen_bounds[1])
		render_texture_line(rayhit, x, screen_bounds, wall_bounds, img, textures);
}

void	render_walls(t_game *game)
{
	unsigned int	i;
	t_rayhit		rayhits[MAX_WINDOW_WIDTH];

	i = 0;
	while (i < game->double_buffer[NEXT]->width && i < MAX_WINDOW_WIDTH)
	{
		rayhits[i] = cast_ray_for_column(&game->cub_data, i,
				game->double_buffer[NEXT]->width);
		render_wall_fill(rayhits[i], i, game->double_buffer[NEXT],
			&game->cub_data.textures);
		i++;
	}
	add_wall_outlines(rayhits, game->double_buffer[NEXT]);
}
