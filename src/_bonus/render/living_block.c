/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   living_block.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 22:58:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/06 23:34:31 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	build_wall_bounds(t_rayhit *rayhit, float center,
		float dist_to_proj_plane)
{
	int	slice_height;

	slice_height = (int)(WORLDMAP_TILE_SIZE * dist_to_proj_plane
			/ rayhit->distance);
	rayhit->wall_bounds[0] = center - slice_height / 2;
	rayhit->wall_bounds[1] = center + slice_height / 2;
}

static void	render_living_block_creation_line(t_rayhit *rayhit, unsigned int x,
		t_game *game)
{
	xpm_t				*texture;
	int					tex_x;
	float				line_height;
	float				step;
	float				tex_offset;
	float				fog;
	float				wall_x;
	int					y_unclipped[2];

	y_unclipped[0] = rayhit->wall_bounds[0];
	y_unclipped[1] = rayhit->wall_bounds[1];
	line_height = (float)(y_unclipped[1] - y_unclipped[0] + 1);
	if (x >= game->double_buffer[NEXT]->width)
		return ;
	if (y_unclipped[0] >= y_unclipped[1])
		return ;
	if (rayhit->side == 0)
		wall_x = rayhit->position.y / WORLDMAP_TILE_SIZE;
	else
		wall_x = rayhit->position.x / WORLDMAP_TILE_SIZE;
	wall_x = wall_x - floorf(wall_x);
	tex_x = (int)(wall_x * (float)game->cub_data.block.atlas.frame_width);
	if ((rayhit->side == 0 && rayhit->face == NORTH)
		|| (rayhit->side == 1 && rayhit->face == WEST))
		tex_x = game->cub_data.block.atlas.frame_width - tex_x - 1;
	step = (float)game->cub_data.block.atlas.frame_height / line_height;
	tex_offset = (rayhit->wall_bounds[0] - y_unclipped[0]) * step;
	fog = fog_factor(rayhit->distance);
	paint_vertical_line_texture_bonus(x, rayhit->wall_bounds, game, texture,
		tex_x, tex_offset, step, fog, &game->cub_data.block.anims[0]);
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
