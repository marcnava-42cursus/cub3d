/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_window_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 10:57:37 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/06 17:28:43 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static float	calculate_center(float height, float pitch)
{
	float	center;
	float	half_height;
	float	min_height;
	float	max_height;

	half_height = height * 0.5f;
	min_height = 0.0f;
	max_height = height - 1.0f;
	center = half_height + pitch;
	center = clamp(center, min_height, max_height);
	return (center);
}

static void	fisheye_correction(t_rayhit *rayhit, float camera_x)
{
	float	angle_from_center;

	if (rayhit->hit)
	{
		angle_from_center = atanf(camera_x * tanf(PLAYER_FOV / 2.0f));
		rayhit->distance *= cosf(angle_from_center);
	}
}

static t_rayhit	cast_ray_for_column(t_cub_data *cub_data, int x,
		int image_width)
{
	t_rayhit	rayhit;
	vertex_t	player_position;
	float		camera_x;

	camera_x = 2.0f * x / (float)image_width - 1.0f;
	player_position.x = ((float)cub_data->player.x) * WORLDMAP_TILE_SIZE;
	player_position.y = ((float)cub_data->player.y) * WORLDMAP_TILE_SIZE;
	rayhit = raycast_world(&cub_data->map, player_position,
			cub_data->player.angle + atanf(camera_x * tanf(PLAYER_FOV / 2.0f)),
			MAX_TILE_RENDER * WORLDMAP_TILE_SIZE);
	fisheye_correction(&rayhit, camera_x);
	return (rayhit);
}

static void	get_ray_dir(float angle, float ray_dir[4])
{
	float			player_dir[2];
	float			camera_plane[2];

	player_dir[X] = cosf(angle);
	player_dir[Y] = sinf(angle);
	camera_plane[X] = -player_dir[Y] * tanf(PLAYER_FOV / 2.0f);
	camera_plane[Y] = player_dir[X] * tanf(PLAYER_FOV / 2.0f);
	ray_dir[X] = player_dir[X] - camera_plane[X];
	ray_dir[Y] = player_dir[Y] - camera_plane[Y];
	ray_dir[U] = player_dir[X] + camera_plane[X];
	ray_dir[V] = player_dir[Y] + camera_plane[Y];
}

void	render_gameplay_window(t_game *game, unsigned int buffer_width)
{
	unsigned int	i;
	t_rayhit		*rayhits;
	float			center;
	float			ray_dir[4];
	float			dist_to_proj_plane;

	render_bg(game);
	rayhits = ft_calloc(buffer_width, sizeof(t_rayhit));
	i = 0;
	while (i < buffer_width)
	{
		rayhits[i] = cast_ray_for_column(&game->cub_data, i, buffer_width);
		i++;
	}
	dist_to_proj_plane = (buffer_width * 0.5f) / tanf(PLAYER_FOV / 2.0f);
	center = calculate_center(game->double_buffer[NEXT]->height,
			game->cub_data.player.pitch);
	get_ray_dir(game->cub_data.player.angle, ray_dir);
	render_floors_and_ceilings(game, center, ray_dir, dist_to_proj_plane);
	render_walls(game, rayhits, center, dist_to_proj_plane);
	//render_doors(game, rayhits, center, dist_to_proj_plane);
	render_orb(game, rayhits, center, ray_dir);
	render_player_overlay(game);
	free(rayhits);
}
