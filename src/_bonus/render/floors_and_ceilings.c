/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors_and_ceilings.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 11:48:38 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 19:49:46 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

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

static float	sqrtf_fog_factor(vertex_t *ray_steps, t_player *player)
{
	float		dist[2];

	dist[X] = (ray_steps->x - player->x) * WORLDMAP_TILE_SIZE;
	dist[Y] = (ray_steps->y - player->y) * WORLDMAP_TILE_SIZE;
	return (sqrtf(dist[X] * dist[X] + dist[Y] * dist[Y]));
}

static void	render_fill_floor(t_game *game, int y, vertex_t ray_steps,
		float fog)
{
	float		f[2];
	int			t[2];
	int			pixel[2];
	uint32_t	pixel_color;
	xpm_t		*texture;

	fog = fog_factor(fog);
	pixel[X] = 0;
	pixel[Y] = (unsigned int)y;
	texture = game->cub_data.textures.floor;
	while (pixel[X] < (int)game->double_buffer[NEXT]->width)
	{
		f[X] = ray_steps.x - floorf(ray_steps.x);
		f[Y] = ray_steps.y - floorf(ray_steps.y);
		t[X] = (int)(f[X] * texture->texture.width);
		t[Y] = (int)(f[Y] * texture->texture.height);
		t[X] = (int)clamp((float)(t[X]), 0.0f, texture->texture.width - 1);
		t[Y] = (int)clamp((float)(t[Y]), 0.0f, texture->texture.height - 1);
		pixel_color = get_pixel_color_bonus(NULL, texture, t);
		paint_pixel_color_bonus(game, pixel, pixel_color, fog);
		ray_steps.x += ray_steps.u;
		ray_steps.y += ray_steps.v;
		pixel[X]++;
	}
}

static void	render_fill_ceiling(t_game *game, int y, vertex_t ray_steps,
		float fog)
{
	float		f[2];
	int			t[2];
	int			pixel[2];
	uint32_t	pixel_color;
	xpm_t		*texture;

	fog = fog_factor(fog);
	pixel[X] = 0;
	pixel[Y] = (unsigned int)y;
	texture = game->cub_data.textures.ceiling;
	while (pixel[X] < (int)game->double_buffer[NEXT]->width)
	{
		f[X] = ray_steps.x - floorf(ray_steps.x);
		f[Y] = ray_steps.y - floorf(ray_steps.y);
		t[X] = (int)(f[X] * texture->texture.width);
		t[Y] = (int)(f[Y] * texture->texture.height);
		t[X] = (int)clamp((float)(t[X]), 0.0f, texture->texture.width - 1);
		t[Y] = (int)clamp((float)(t[Y]), 0.0f, texture->texture.height - 1);
		pixel_color = get_pixel_color_bonus(NULL, texture, t);
		paint_pixel_color_bonus(game, pixel, pixel_color, fog);
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
	float		fog;

	i = 0;
	while (i < (int)center)
	{
		ray_steps = get_ray_steps(game, ray_dir, center - i,
				dist_to_proj_plane);
		fog = sqrtf_fog_factor(&ray_steps, &game->cub_data.player);
		render_fill_ceiling(game, i, ray_steps, fog);
		i++;
	}
	i++;
	while (i < (int)game->double_buffer[NEXT]->height)
	{
		ray_steps = get_ray_steps(game, ray_dir, i - center + 1,
				dist_to_proj_plane);
		fog = sqrtf_fog_factor(&ray_steps, &game->cub_data.player);
		render_fill_floor(game, i, ray_steps, fog);
		i++;
	}
}
