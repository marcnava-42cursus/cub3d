/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   living_block.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 22:58:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/08 13:22:58 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static bool	get_wall_x(t_rayhit *rayhit, t_game *game, int x_y_packed[3],
		float *wall_x)
{
	if ((unsigned int)x_y_packed[0] >= game->double_buffer[NEXT]->width)
		return (false);
	if (x_y_packed[1] >= x_y_packed[2])
		return (false);
	if (rayhit->side == 0)
		*wall_x = rayhit->position.y / WORLDMAP_TILE_SIZE;
	else
		*wall_x = rayhit->position.x / WORLDMAP_TILE_SIZE;
	*wall_x = *wall_x - floorf(*wall_x);
	return (true);
}

static void	render_living_block_creation_line(t_rayhit *rayhit, unsigned int x,
		t_game *game)
{
	xpm_t		*texture;
	float		x_offset_step[3];
	int			x_y_packed[3];
	t_vert_line	vert_line;

	texture = NULL;
	pack_x_ys(x, rayhit->wall_bounds, x_y_packed);
	x_offset_step[1] = (float)(x_y_packed[2] - x_y_packed[1] + 1);
	if (!get_wall_x(rayhit, game, x_y_packed, &x_offset_step[0]))
		return ;
	x_offset_step[0] = x_offset_step[0]
		* (float)game->cub_data.block.atlas.frame_width;
	if ((rayhit->side == 0 && rayhit->face == NORTH)
		|| (rayhit->side == 1 && rayhit->face == WEST))
		x_offset_step[0] = game->cub_data.block.atlas.frame_width
			- x_offset_step[0] - 1;
	x_offset_step[2] = (float)game->cub_data.block.atlas.frame_height
		/ x_offset_step[1];
	x_offset_step[1] = (rayhit->wall_bounds[0] - x_y_packed[1])
		* x_offset_step[2];
	pack_tex_and_anim_for_vert_line(texture, &game->cub_data.block.anims[0],
		&vert_line);
	pack_coords_and_fog_for_vert_line(x_y_packed, x_offset_step,
		fog_factor(rayhit->distance), &vert_line);
	paint_vertical_line_texture_bonus(game, &vert_line);
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
