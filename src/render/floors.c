/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/02 20:41:04 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	render_floor_fill(t_player *player, t_rayhit rayhit, unsigned int x,
		mlx_image_t *img, t_textures *textures)
{
	int		y;
	float		current_aspect_ratio;
	float		aspect_scale;
	float	dist_to_proj_plane;
	float	weight;
	float	curr_floor_x;
	float	curr_floor_y;
	int		floor_tex_x;
	int		floor_tex_y;
	
	current_aspect_ratio = (float)img->width / (float)img->height;
	aspect_scale = current_aspect_ratio / BASE_ASPECT_RATIO;
	y = rayhit.wall_bounds[1] + 1;
	if (y <= (int)(img->height * 0.5f))
		y = (int)(img->height + 1);
	while (y < img->height)
	{
		dist_to_proj_plane = ((float)img->height / (2.0f * tanf(PLAYER_FOV / 2.0f)) * (1.25f + aspect_scale * 0.5f)) / (y - img->height * 0.5f);
		if (dist_to_proj_plane <= 0.0f)
		{
			y++;
			continue;
		}
		weight = dist_to_proj_plane / rayhit.distance;
		if (weight <= 0.0f)
		{
			y++;
			continue;
		}
		curr_floor_x = player->x * (1.0f - weight) + rayhit.position.x * weight;
		curr_floor_y = player->y * (1.0f - weight) + rayhit.position.y * weight;
		float fx = (curr_floor_x / 8.0f) - floorf(curr_floor_x);
		float fy = (curr_floor_y / 8.0f) - floorf(curr_floor_y);
		floor_tex_x = (int)(fx * textures->north->texture.width);
		floor_tex_y = (int)(fy * textures->north->texture.height);
		floor_tex_x = (floor_tex_x % textures->north->texture.width + textures->north->texture.width) % textures->north->texture.width;
		floor_tex_y = (floor_tex_y % textures->north->texture.height + textures->north->texture.height) % textures->north->texture.height;
		paint_horizontal_line_texture(y, x, img, textures->north, floor_tex_y, floor_tex_x);
		y++;
	}
}

void	render_floors(t_game *game, t_rayhit *rayhits)
{
	unsigned int	i;

	i = 0;
	while (i < game->double_buffer[NEXT]->width)
	{
		render_floor_fill(&game->cub_data.player, rayhits[i], i,
				game->double_buffer[NEXT], &game->cub_data.textures);
		i++;
	}
}
