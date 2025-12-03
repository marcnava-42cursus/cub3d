/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/03 01:37:41 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	render_floor_fill(t_player *player, float player_dir[2], float camera_plane[2], unsigned int y,
		mlx_image_t *img, t_textures *textures)
{
	int		x = 0;
	float	center = 0.5f * img->height;
	int		p = y - center;
	float	row_distance = center / p;
	float	rayDirX0 = player_dir[0] - camera_plane[0];
	float	rayDirY0 = player_dir[1] - camera_plane[1];
	float	rayDirX1 = player_dir[0] + camera_plane[0];
	float	rayDirY1 = player_dir[1] + camera_plane[1];
	float	floorX0 = ((player->x + 0.2f) * WORLDMAP_TILE_SIZE) + row_distance * rayDirX0;
	float	floorY0 = ((player->y + 0.2f) * WORLDMAP_TILE_SIZE) + row_distance * rayDirY0;
	float	floorX1 = ((player->x + 0.2f) * WORLDMAP_TILE_SIZE) + row_distance * rayDirX1;
	float	floorY1 = ((player->y + 0.2f) * WORLDMAP_TILE_SIZE) + row_distance * rayDirY1;
	float	floor_step_x = (floorX1 - floorX0) / (float)img->width;
	float	floor_step_y = (floorY1 - floorY0) / (float)img->width;
	float	floor_x = floorX0;
	float	floor_y = floorY0;

	if (p == 0.0f)
		return ;
	while (x < img->width)
	{
		float tile_x = (floor_x / WORLDMAP_TILE_SIZE) * 64.0f;
		float tile_y = (floor_y / WORLDMAP_TILE_SIZE) * 64.0f;
		float fx = tile_x - floorf(tile_x);
		float fy = tile_y - floorf(tile_y);
		int	tx	= (int)(fx * textures->north->texture.width);	
		int	ty	= (int)(fy * textures->north->texture.height);	
		if (tx < 0)
			tx += textures->north->texture.width;
		if (tx >= 0)
			tx %= textures->north->texture.width;
		if (ty < 0)
			ty += textures->north->texture.height;
		if (ty >= 0)
			ty %= textures->north->texture.height;
		paint_horizontal_line_texture(y, x, img, textures->north, ty, tx);
		floor_x += floor_step_x;
		floor_y += floor_step_y;
		x++;
	}
}

void	render_floors(t_game *game, t_rayhit *rayhits)
{
	unsigned int	i;
	float	player_dir[2];
	float	camera_plane[2];

	//i = game->double_buffer[NEXT]->height / 2;
	i = 0;
	player_dir[0] = cosf(game->cub_data.player.angle);
	player_dir[1] = sinf(game->cub_data.player.angle);
	camera_plane[0] = -player_dir[1] * tanf(PLAYER_FOV / 2.0f);
	camera_plane[1] = player_dir[0] * tanf(PLAYER_FOV / 2.0f);
	while (i < game->double_buffer[NEXT]->height)
	{
		render_floor_fill(&game->cub_data.player, player_dir, camera_plane, i,
				game->double_buffer[NEXT], &game->cub_data.textures);
		i++;
	}
}
