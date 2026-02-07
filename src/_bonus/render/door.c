/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 16:40:09 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 01:06:08 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static void	build_door_bounds(t_rayhit *rayhit, float center,
		float dist_to_proj_plane)
{
	int	slice_height;

	slice_height = (int)(WORLDMAP_TILE_SIZE * dist_to_proj_plane
			/ rayhit->distance);
	rayhit->wall_bounds[0] = center - slice_height / 2;
	rayhit->wall_bounds[1] = center + slice_height / 2;
}

static void	render_door_fill(t_game *game, t_rayhit *rayhit, unsigned int x,
		float center, float dist_to_proj_plane)
{
	int			screen_bounds[2];
	mlx_image_t	*img;

	if (!rayhit->hit)
		return ;
	img = game->double_buffer[NEXT];
	build_door_bounds(rayhit, center, dist_to_proj_plane);
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
		render_texture_line_bonus(rayhit, x, game);
}

void	render_doors(t_game *game, t_rayhit *rayhits, float center,
			float dist_to_proj_plane)
{
	unsigned int	i;

	i = 0;
	while (i < (unsigned int)game->double_buffer[NEXT]->width)
	{
		render_door_fill(game, &rayhits[i], i, center, dist_to_proj_plane);
		i++;
	}
}
