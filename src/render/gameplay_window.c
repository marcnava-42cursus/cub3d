/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_window.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 10:57:37 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/27 18:37:13 by ivmirand         ###   ########.fr       */
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
	min_height = 1.0f;
	max_height = height - 2.0f;
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
			MAX_RENDER_DISTANCE);
	fisheye_correction(&rayhit, camera_x);
	return (rayhit);
}

void	render_gameplay_window(t_game *game, unsigned int buffer_width)
{
	unsigned int	i;
	t_rayhit		*rayhits;
	float			center;
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
	render_walls(game, rayhits, center, dist_to_proj_plane);
	free(rayhits);
}
