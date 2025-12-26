/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/26 10:06:54 by ivmirand         ###   ########.fr       */
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
	ray_dir[0] = player_dir[0] - camera_plane[0];
	ray_dir[1] = player_dir[1] - camera_plane[1];
	ray_dir[2] = player_dir[0] + camera_plane[0];
	ray_dir[3] = player_dir[1] + camera_plane[1];
	floor_xy_and_steps.x = ((player->x + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[0];
	floor_xy_and_steps.y = ((player->y + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[1];
	floor_xy_and_steps.u = ((player->x + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[2];
	floor_xy_and_steps.v = ((player->y + 0.2f) * WORLDMAP_TILE_SIZE)
		+ row_distance * ray_dir[3];
	floor_xy_and_steps.u = (floor_xy_and_steps.u - floor_xy_and_steps.x)
		/ (float)img->width;
	floor_xy_and_steps.v = (floor_xy_and_steps.v - floor_xy_and_steps.y)
		/ (float)img->width;
	return (floor_xy_and_steps);
}

static void	render_floor_fill(t_player *player, unsigned int y,
		mlx_image_t *img, t_textures *textures, vertex_t floor_xy_and_steps)
{
	int		x;
	float	tile_xy[2];
	int		t_xy[2];

	x = 0;
	while (x < img->width)
	{
		tile_xy[0] = (floor_xy_and_steps.x / WORLDMAP_TILE_SIZE) * 64.0f;
		tile_xy[1] = (floor_xy_and_steps.y / WORLDMAP_TILE_SIZE) * 64.0f;
		t_xy[0]	= (int)((tile_xy[0] - floorf(tile_xy[0]))
				* textures->north->texture.width);	
		t_xy[1]	= (int)((tile_xy[1] - floorf(tile_xy[1]))
				* textures->north->texture.height);	
		if (t_xy[0] < 0)
			t_xy[0] += textures->north->texture.width;
		if (t_xy[0] >= 0)
			t_xy[0] %= textures->north->texture.width;
		if (t_xy[1] < 0)
			t_xy[1] += textures->north->texture.height;
		if (t_xy[1] >= 0)
			t_xy[1] %= textures->north->texture.height;
		paint_horizontal_line_texture(y, x, img, textures->north, t_xy[1], t_xy[0]);
		floor_xy_and_steps.x += floor_xy_and_steps.u;
		floor_xy_and_steps.y += floor_xy_and_steps.v;
		x++;
	}
}

void	render_floors(t_game *game, t_rayhit *rayhits)
{
	unsigned int	i;
	float	player_dir[2];
	float	camera_plane[2];
	vertex_t floor_xy_and_steps;

	i = game->double_buffer[NEXT]->height / 2;
	player_dir[0] = cosf(game->cub_data.player.angle);
	player_dir[1] = sinf(game->cub_data.player.angle);
	camera_plane[0] = -player_dir[1] * tanf(PLAYER_FOV / 2.0f);
	camera_plane[1] = player_dir[0] * tanf(PLAYER_FOV / 2.0f);
	while (i < game->double_buffer[NEXT]->height)
	{
		floor_xy_and_steps = get_floor_ray_directions(&game->cub_data.player,
				player_dir, camera_plane, game->double_buffer[NEXT], i);
		render_floor_fill(&game->cub_data.player, i, game->double_buffer[NEXT],
			&game->cub_data.textures, floor_xy_and_steps);
		i++;
	}
}
