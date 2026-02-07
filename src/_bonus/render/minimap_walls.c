/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_walls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:47:44 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 22:42:06 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static uint32_t	get_minimap_wall_color(char tile)
{
	if (tile == '1')
		return (WHITE);
	if (tile == '2')
		return (LIGHT_GREY);
	return (0);
}

static void	get_world_pixel(int pixel[2], t_player *player, int world_pixel[2],
		float cos_sin_pai[2])
{
	float	d[2];
	float	world[2];

	d[X] = (float)pixel[X] - (MINIMAP_WIDTH / 2.0f);
	d[Y] = (float)pixel[Y] - (MINIMAP_HEIGHT / 2.0f);
	world[X] = d[X] * cos_pai - d[Y] * sin_pai;
	world[Y] = d[X] * sin_pai + d[Y] * cos_pai;
	world[X] = player->x + (world[X] / (float)MINIMAP_TILE_SIZE);
	world[Y] = player->y + (world[Y] / (float)MINIMAP_TILE_SIZE);
	world_pixel[X] = (int)floorf(world[X]);
	world_pixel[Y] = (int)floorf(world[Y]);
}

static void	paint_pixel_to_minimap(t_minimap *minimap, int pixel[2],
		float cos_sin_pai[2])
{
	int		world_pixel[2];

	get_world_pixel(pixel, &minimap->player, world_pixel, cos_sin_pai);
	if (world_pixel[Y] >= 0 && world_pixel[Y] < minimap->map->height
		&& minimap->map->grid[world_pixel[Y]]
		&& world_pixel[X] >= 0 && world_pixel[X]
		< (int)ft_strlen(minimap->map->grid[world_pixel[Y]]))
	{
		tile_color = get_minimap_wall_color(
				minimap->map->grid[world_pixel[Y]][world_pixel[X]]);
		if (tile_color != 0)
			save_pixel_to_image(minimap->bg, pixel[X], pixel[Y],
				tile_color);
	}
}

void	render_minimap_walls(t_minimap *minimap)
{
	int		pixel[2];
	float	cos_sin_pai[2];
	float	player_angle_inverse;
	int		tile_color;

	player_angle_inverse = minimap->player->angle + (FT_PI / 2.0f);
	cos_sin_pai[0] = cosf(player_angle_inverse);
	cos_sin_pai[1] = sinf(player_angle_inverse);
	pixel[Y] = 0;
	while (pixel[Y] < (int)minimap->bg->height)
	{
		pixel[X] = 0;
		while (pixel[X] < (int)minimap->bg->width)
		{
			if (is_inside_minimap_circle(pixel[X], pixel[Y]))
				paint_pixel_to_minimap(minimap, pixel, cos_sin_pai);
			pixel[X]++;
		}
		pixel[Y]++;
	}
}
