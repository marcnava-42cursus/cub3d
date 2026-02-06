/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:51:39 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/06 17:31:51 by ivmirand         ###   ########.fr       */
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

static void	render_wall_fill(t_game *game, t_rayhit *rayhit, unsigned int x,
		float center, float dist_to_proj_plane)
{
	int			screen_bounds[2];
	mlx_image_t	*img;

	if (!rayhit->hit)
		return ;
	img = game->double_buffer[NEXT];
	build_wall_bounds(rayhit, center, dist_to_proj_plane);
	screen_bounds[0] = rayhit->wall_bounds[0];
	screen_bounds[1] = rayhit->wall_bounds[1];
	if (screen_bounds[0] < 0)
		screen_bounds[0] = 0;
	if (screen_bounds[1] > (int)img->height)
		screen_bounds[1] = (int)img->height;
	if (screen_bounds[0] > screen_bounds[1]
		|| screen_bounds[1] < 0 || screen_bounds[0] >= (int)img->height)
		return ;
	if (screen_bounds[0] <= screen_bounds[1])
		render_texture_line_bonus(rayhit, x, screen_bounds, game);
	rayhit->wall_bounds[0] = screen_bounds[0];
	rayhit->wall_bounds[1] = screen_bounds[1];
}

void	render_walls(t_game *game, t_rayhit *rayhits, float center,
		float dist_to_proj_plane)
{
	unsigned int	i;

	i = 0;
	while (i < (unsigned int)game->double_buffer[NEXT]->width)
	{
		render_wall_fill(game, &rayhits[i], i, center, dist_to_proj_plane);
		i++;
	}
	//add_wall_outlines(rayhits, game->double_buffer[NEXT], &game->cub_data.map,
	//	center, dist_to_proj_plane);
	render_absorb_effects(game, rayhits, center, dist_to_proj_plane);
}
