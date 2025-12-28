/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/28 12:38:02 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static vertex_t	get_floor_ray_directions(t_player *player, float player_dir[2],
		float camera_plane[2], mlx_image_t *img, int y)
{
	float		ray_dir[4];
	vertex_t	floor_xy_and_steps;
	float		center;
	float		row_distance;

	center = 0.5f * img->height;
	row_distance = center / (y - center);
	ray_dir[X] = player_dir[X] - camera_plane[X];
	ray_dir[Y] = player_dir[Y] - camera_plane[Y];
	ray_dir[U] = player_dir[X] + camera_plane[X];
	ray_dir[V] = player_dir[Y] + camera_plane[Y];
	floor_xy_and_steps.x = ((player->x + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[X];
	floor_xy_and_steps.y = ((player->y + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[Y];
	floor_xy_and_steps.u = ((player->x + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[U];
	floor_xy_and_steps.v = ((player->y + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[V];
	floor_xy_and_steps.u = (floor_xy_and_steps.u - floor_xy_and_steps.x)
		/ (float)img->width;
	floor_xy_and_steps.v = (floor_xy_and_steps.v - floor_xy_and_steps.y)
		/ (float)img->width;
	return (floor_xy_and_steps);
}

static void	render_floor_fill(unsigned int y, mlx_image_t *img,
		xpm_t *xpm, vertex_t floor_xy_and_steps)
{
	unsigned int	x;
	float			tile[2];
	int				t[2];

	x = 0;
	while (x < img->width)
	{
		tile[X] = (floor_xy_and_steps.x / WORLDMAP_TILE_SIZE) * 64.0f;
		tile[Y] = (floor_xy_and_steps.y / WORLDMAP_TILE_SIZE) * 64.0f;
		t[X] = (int)((tile[X] - floorf(tile[X])) * xpm->texture.width);
		t[Y] = (int)((tile[Y] - floorf(tile[Y])) * xpm->texture.height);
		if (t[X] < 0)
			t[X] += xpm->texture.width;
		if (t[X] >= 0)
			t[X] %= xpm->texture.width;
		if (t[Y] < 0)
			t[Y] += xpm->texture.height;
		if (t[Y] >= 0)
			t[Y] %= xpm->texture.height;
		paint_horizontal_line_texture(y, x, img, xpm, t[Y], t[X]);
		floor_xy_and_steps.x += floor_xy_and_steps.u;
		floor_xy_and_steps.y += floor_xy_and_steps.v;
		x++;
	}
}

void	render_floors(t_game *game)
{
	unsigned int	i;
	float			player_dir[2];
	float			camera_plane[2];
	vertex_t		floor_xy_and_steps;

	i = game->double_buffer[NEXT]->height / 2;
	player_dir[X] = cosf(game->cub_data.player.angle);
	player_dir[Y] = sinf(game->cub_data.player.angle);
	camera_plane[X] = -player_dir[Y] * tanf(PLAYER_FOV / 2.0f);
	camera_plane[Y] = player_dir[X] * tanf(PLAYER_FOV / 2.0f);
	while (i < game->double_buffer[NEXT]->height)
	{
		floor_xy_and_steps = get_floor_ray_directions(&game->cub_data.player,
				player_dir, camera_plane, game->double_buffer[NEXT], i);
		render_floor_fill(i, game->double_buffer[NEXT],
			game->cub_data.textures.north, floor_xy_and_steps);
		i++;
	}
}
