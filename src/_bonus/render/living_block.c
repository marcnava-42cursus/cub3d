/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   living_block.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 22:58:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 22:07:14 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static void	render_living_block_creation_line(t_rayhit *rayhit, unsigned int x,
		t_game *game)
{
	xpm_t	*texture;
	float	x_offset_step[3];
	float	line_height;
	float	fog;
	float	wall_x;
	int		x_y_packed[3];

	texture = NULL;
	pack_x_ys(x, rayhit->wall_bounds, x_y_packed);
	line_height = (float)(x_y_packed[2] - x_y_packed[1] + 1);
	if ((unsigned int)x_y_packed[0] >= game->double_buffer[NEXT]->width)
		return ;
	if (x_y_packed[1] >= x_y_packed[2])
		return ;
	if (rayhit->side == 0)
		wall_x = rayhit->position.y / WORLDMAP_TILE_SIZE;
	else
		wall_x = rayhit->position.x / WORLDMAP_TILE_SIZE;
	wall_x = wall_x - floorf(wall_x);
	x_offset_step[0] = wall_x * (float)game->cub_data.block.atlas.frame_width;
	if ((rayhit->side == 0 && rayhit->face == NORTH)
		|| (rayhit->side == 1 && rayhit->face == WEST))
		x_offset_step[0] = game->cub_data.block.atlas.frame_width
			- x_offset_step[0] - 1;
	x_offset_step[2] = (float)game->cub_data.block.atlas.frame_height
		/ line_height;
	x_offset_step[1] = (rayhit->wall_bounds[0] - x_y_packed[1])
		* x_offset_step[2];
	fog = fog_factor(rayhit->distance);
	paint_vertical_line_texture_bonus(x_y_packed, game, texture,
		x_offset_step, fog, &game->cub_data.block.anims[0]);
}

static void	render_living_block_creations(t_game *game, t_rayhit *rayhit, int i)
{
	int			screen_bounds[2];
	mlx_image_t	*img;

	if (!rayhit->hit
		|| game->cub_data.map.grid[rayhit->cell[Y]][rayhit->cell[X]] != '3')
		return ;
	img = game->double_buffer[NEXT];
	screen_bounds[0] = rayhit->wall_bounds[0];
	screen_bounds[1] = rayhit->wall_bounds[1];
	if (screen_bounds[0] < 0)
		screen_bounds[0] = 0;
	if (screen_bounds[1] > (int)img->height)
		screen_bounds[1] = (int)img->height;
	if (screen_bounds[0] > screen_bounds[1]
		|| screen_bounds[1] < 0 || screen_bounds[0] >= (int)img->height)
		return ;
	rayhit->wall_bounds[0] = screen_bounds[0];
	rayhit->wall_bounds[1] = screen_bounds[1];
	if (screen_bounds[0] <= screen_bounds[1])
		render_living_block_creation_line(rayhit, i, game);
}

void	render_living_block(t_game *game, t_rayhit *rayhits, float center,
		float dist_to_proj_plane)
{
	unsigned int	i;

	i = 0;
	while (i < (unsigned int)game->double_buffer[NEXT]->width)
	{
		build_wall_bounds(&rayhits[i], center, dist_to_proj_plane);
		render_living_block_creations(game, &rayhits[i], i);
		i++;
	}
}
