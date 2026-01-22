/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_tile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:47:44 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/02 00:38:34 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

#ifdef BONUS
# define ORB_MINIMAP_COLOR 0xFF00FFFF
#endif

static uint32_t	get_minimap_wall_color(char tile)
{
	if (tile == '1')
		return (WHITE);
	if (tile == '2')
		return (LIGHT_GREY);
	return (0);
}

static void	render_minimap_tile(mlx_image_t *tile, int tile_size, int color)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < (unsigned int)tile_size)
	{
		x = 0;
		while (x < (unsigned int)tile_size)
		{
			save_pixel_to_image(tile, x, y, color);
			x++;
		}
		y++;
	}
}

void	render_minimap_walls(t_minimap *minimap)
{
	int		pixel[2];
	float	player_angle_inverse;
	float	cos_pai;
	float	sin_pai;
	float	d[2];
	int		world_pixel[2];
	float	world[2];
	int		tile_color;

	player_angle_inverse = minimap->player->angle + (FT_PI / 2.0f);
	cos_pai = cosf(player_angle_inverse);
	sin_pai = sinf(player_angle_inverse);
	pixel[Y] = 0;
	while (pixel[Y] < (int)minimap->bg->height)
	{
		pixel[X] = 0;
		while (pixel[X] < (int)minimap->bg->width)
		{
			if (is_inside_minimap_circle(pixel))
			{
				d[X] = (float)pixel[X] - (MINIMAP_WIDTH / 2.0f);
				d[Y] = (float)pixel[Y] - (MINIMAP_HEIGHT / 2.0f);
				world[X] = d[X] * cos_pai - d[Y] * sin_pai;
				world[Y] = d[X] * sin_pai + d[Y] * cos_pai;
				world[X] = minimap->player->x + (world[X] / (float)MINIMAP_TILE_SIZE);
				world[Y] = minimap->player->y + (world[Y] / (float)MINIMAP_TILE_SIZE);
				world_pixel[X] = (int)floorf(world[X]);
				world_pixel[Y] = (int)floorf(world[Y]);
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
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	render_minimap_player(t_minimap *minimap)
{
	render_minimap_tile(minimap->player_sprite, MINIMAP_PLAYER_SIZE, RED);
}

#ifdef BONUS
void	render_minimap_orb(t_game *game)
{
	t_minimap	*minimap;
	vertex_t	world;
	vertex_t	pos;
	int			orb_size;
	int			start_x;
	int			start_y;
	int			pixel[2];
	int			x;
	int			y;

	if (!game || !game->orb.active)
		return ;
	minimap = &game->minimap;
	world.x = game->orb.x * WORLDMAP_TILE_SIZE;
	world.y = game->orb.y * WORLDMAP_TILE_SIZE;
	world.z = 0.0f;
	world.u = 0.0f;
	world.v = 0.0f;
	world.tex = 0;
	pos = world_to_minimap_vertex(minimap, world);
	orb_size = MINIMAP_PLAYER_SIZE / 2;
	if (orb_size < 3)
		orb_size = 3;
	start_x = (int)roundf(pos.x) - orb_size / 2;
	start_y = (int)roundf(pos.y) - orb_size / 2;
	y = 0;
	while (y < orb_size)
	{
		x = 0;
		while (x < orb_size)
		{
			pixel[X] = start_x + x;
			pixel[Y] = start_y + y;
			if (pixel[X] >= 0 && pixel[X] < (int)minimap->bg->width
				&& pixel[Y] >= 0 && pixel[Y] < (int)minimap->bg->height
				&& is_inside_minimap_circle(pixel))
				save_pixel_to_image(minimap->bg, pixel[X], pixel[Y],
					ORB_MINIMAP_COLOR);
			x++;
		}
		y++;
	}
}
#endif
