/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors_and_ceilings.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 11:48:38 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/06 18:43:38 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static vertex_t	get_ray_steps(t_game *game, float ray_dir[4],
		float p, float dist_to_proj_plane)
{
	vertex_t	floor_and_steps;
	float		row_distance;

	if (p < 1.0f)
		p = 1.0f;
	row_distance = dist_to_proj_plane * 0.5f / p;
	floor_and_steps.x = game->cub_data.player.x + row_distance * ray_dir[X];
	floor_and_steps.y = game->cub_data.player.y + row_distance * ray_dir[Y];
	floor_and_steps.u = row_distance * (ray_dir[U] - ray_dir[X])
		/ game->double_buffer[NEXT]->width;
	floor_and_steps.v = row_distance * (ray_dir[V] - ray_dir[Y])
		/ game->double_buffer[NEXT]->width;
	return (floor_and_steps);
}

static void	render_fill(t_game *game, int y, xpm_t *texture, vertex_t ray_steps,
		float fog)
{
	float			f[2];
	int				t[2];
	int				pixel[2];

	fog = fog_factor(fog);
	pixel[X] = 0;
	pixel[Y] = (unsigned int)y;
	while (pixel[X] < game->double_buffer[NEXT]->width)
	{
		f[X] = ray_steps.x - floorf(ray_steps.x);
		f[Y] = ray_steps.y - floorf(ray_steps.y);
		t[X] = (int)(f[X] * texture->texture.width);
		t[Y] = (int)(f[Y] * texture->texture.height);
		t[X] = (int)clamp((float)(t[X]), 0.0f, texture->texture.width - 1);
		t[Y] = (int)clamp((float)(t[Y]), 0.0f, texture->texture.height - 1);
		paint_texture_pixel_bonus(pixel, game, texture, t, fog, NULL);
		ray_steps.x += ray_steps.u;
		ray_steps.y += ray_steps.v;
		pixel[X]++;
	}
}

void	render_floors_and_ceilings(t_game *game, float center, float ray_dir[4],
		float dist_to_proj_plane)
{
	int			i;
	vertex_t	ray_steps;
	float		dist[2];
	float		fog;

	i = 0;
	while (i < (int)center)
	{
		ray_steps = get_ray_steps(game, ray_dir, center - i, dist_to_proj_plane);
		dist[X] = (ray_steps.x - game->cub_data.player.x) * WORLDMAP_TILE_SIZE;
		dist[Y] = (ray_steps.y - game->cub_data.player.y) * WORLDMAP_TILE_SIZE;
		fog = sqrtf(dist[X] * dist[X] + dist[Y] * dist[Y]);
		render_fill(game, i, game->cub_data.textures.ceiling,
			ray_steps, fog);
		i++;
	}
	i++;
	while (i < game->double_buffer[NEXT]->height)
	{
		ray_steps = get_ray_steps(game, ray_dir, i - center + 1, dist_to_proj_plane);
		dist[X] = (ray_steps.x - game->cub_data.player.x) * WORLDMAP_TILE_SIZE;
		dist[Y] = (ray_steps.y - game->cub_data.player.y) * WORLDMAP_TILE_SIZE;
		fog = sqrtf(dist[X] * dist[X] + dist[Y] * dist[Y]);
		render_fill(game, i, game->cub_data.textures.floor,
			ray_steps, fog);
		i++;
	}
}
