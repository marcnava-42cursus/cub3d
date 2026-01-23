/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:51:39 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/23 10:28:13 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	build_wall_bounds(t_rayhit *rayhit, mlx_image_t *img, t_player * player, float center)
{
	int			slice_height;
	float		dist_to_proj_plane;

	dist_to_proj_plane = (img->width * 0.5f) / tanf(PLAYER_FOV / 2.0f);
	slice_height = (int)(WORLDMAP_TILE_SIZE * dist_to_proj_plane / rayhit->distance);
	rayhit->wall_bounds[0] = center - slice_height / 2;
	rayhit->wall_bounds[1] = center + slice_height / 2;
}

static void	render_wall_fill(t_rayhit *rayhit, unsigned int x, mlx_image_t *img,
		t_textures *textures, t_player *player, float center)
{
	int			screen_bounds[2];

	if (!rayhit->hit)
		return ;
	build_wall_bounds(rayhit, img, player, center);
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
		render_texture_line(*rayhit, x, screen_bounds, img, textures);
	rayhit->wall_bounds[0] = screen_bounds[0];
	rayhit->wall_bounds[1] = screen_bounds[1];
}

void	render_walls(t_game *game, t_rayhit *rayhits, float center)
{
	unsigned int	i;

	i = 0;
	while (i < game->double_buffer[NEXT]->width)
	{
		render_wall_fill(&rayhits[i], i, game->double_buffer[NEXT],
			&game->cub_data.textures, &game->cub_data.player, center);
		i++;
	}
	add_wall_outlines(rayhits, game->double_buffer[NEXT], &game->cub_data.map,
			center);
}
