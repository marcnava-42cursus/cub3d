/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/28 22:33:58 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static vertex_t	get_floor_ray_steps(t_player *player, float ray_dir[4],
		mlx_image_t *img, int y, float center, float dist_to_proj_plane)
{
	vertex_t	floor_and_steps;
	float		p;
	float		row_distance;

	p = y - center;
	if (p < 1.0f)
		p = 1.0f;
	row_distance = dist_to_proj_plane * 0.5f / p;
	floor_and_steps.x = player->x + row_distance * ray_dir[X];
	floor_and_steps.y = player->y + row_distance * ray_dir[Y];
	floor_and_steps.u = row_distance * (ray_dir[U] - ray_dir[X]) / img->width;
	floor_and_steps.v = row_distance * (ray_dir[V] - ray_dir[Y]) / img->width;
	return (floor_and_steps);
}

static void	render_floor_fill(unsigned int y, mlx_image_t *img,
		t_textures *textures, vertex_t floor_and_steps, float fog)
{
	unsigned int	x;
	float			f[2];
	int				t[2];

	x = 0;
	while (x < img->width)
	{
		f[X] = floor_and_steps.x - floorf(floor_and_steps.x);
		f[Y] = floor_and_steps.y - floorf(floor_and_steps.y);
		t[X] = (int)(f[X] * textures->floor->texture.width);
		t[Y] = (int)(f[Y] * textures->floor->texture.height);
		t[X] = (int)clamp((float)t[X], 0.0f,
				textures->floor->texture.width - 1);
		t[Y] = (int)clamp((float)t[Y], 0.0f,
				textures->floor->texture.height - 1);
		paint_horizontal_line_texture_bonus(y, x, img, textures->floor,
			textures->fog, t[Y], t[X], fog);
		floor_and_steps.x += floor_and_steps.u;
		floor_and_steps.y += floor_and_steps.v;
		x++;
	}
}

void	render_floors(t_game *game, float center, float ray_dir[4], float dist_to_proj_plane)
{
	unsigned int	i;
	vertex_t		floor_and_steps;
	float			dist[2];
	float			fog;

	i = (unsigned int)center + 1;
	while (i < game->double_buffer[NEXT]->height)
	{
		floor_and_steps = get_floor_ray_steps(&game->cub_data.player, ray_dir,
				game->double_buffer[NEXT], i, center, dist_to_proj_plane);
		dist[X] = (floor_and_steps.x - game->cub_data.player.x)
			* WORLDMAP_TILE_SIZE;
		dist[Y] = (floor_and_steps.y - game->cub_data.player.y)
			* WORLDMAP_TILE_SIZE;
		fog = sqrtf(dist[X] * dist[X] + dist[Y] * dist[Y]);
		fog = fog_factor(fog);
		render_floor_fill(i, game->double_buffer[NEXT],
			&game->cub_data.textures, floor_and_steps, fog);
		i++;
	}
}
