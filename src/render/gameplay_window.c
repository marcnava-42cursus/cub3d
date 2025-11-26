/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_window.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 10:57:37 by ivmirand          #+#    #+#             */
/*   Updated: 2025/11/18 18:59:33 by marcnava         ###   ########.fr       */
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
	player_position.x = ((float)cub_data->player.x + 0.2f) * WORLDMAP_TILE_SIZE;
	player_position.y = ((float)cub_data->player.y + 0.2f) * WORLDMAP_TILE_SIZE;
	rayhit = raycast_world(&cub_data->map, player_position, ray_angle,
			MAX_DIST);
	fisheye_correction(&rayhit, camera_x);
	return (rayhit);
}

void	render_gameplay_window(t_game *game, unsigned int buffer_width)
{
	unsigned int	i;
	t_rayhit		rayhits[buffer_width];

    render_bg(game);
	i = 0;
	while (i < buffer_width)
	{
		rayhits[i] = cast_ray_for_column(&game->cub_data, i, buffer_width);
		i++;
	}
	render_floors(game, rayhits);
#ifdef BONUS
	render_walls(game);
#else
	render_walls(game, rayhits);
#endif
	render_player_overlay(game);
}
