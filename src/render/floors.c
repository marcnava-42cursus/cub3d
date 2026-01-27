/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/26 22:00:31 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static vertex_t	get_floor_ray_steps(t_player *player, float ray_dir[4],
		mlx_image_t *img, int y, float center)
{
	vertex_t	floor_and_steps;
	float		p;
	float		row_distance;
	float		dist_to_proj_plane;

	p = y - center;
	if (p < 1.0f) 
		p = 1.0f;
	dist_to_proj_plane = (img->width * 0.5f) / tanf(PLAYER_FOV / 2.0f);
	row_distance = dist_to_proj_plane * 0.5f / p;
	floor_and_steps.x = player->x + row_distance * ray_dir[X];
	floor_and_steps.y = player->y + row_distance * ray_dir[Y];
	floor_and_steps.u = row_distance * (ray_dir[U] - ray_dir[X]) / img->width;
	floor_and_steps.v = row_distance * (ray_dir[V] - ray_dir[Y]) / img->width;
	return (floor_and_steps);
}

static xpm_t	*find_custom_floor_texture(t_custom_texture *custom, char cell)
{
	t_custom_texture	*current;
	char				id[3];
	char				faces[4];
	int					i;

	if (!custom)
		return (NULL);
	faces[0] = 'N';
	faces[1] = 'S';
	faces[2] = 'E';
	faces[3] = 'W';
	i = 0;
	while (i < 4)
	{
		id[0] = cell;
		id[1] = faces[i];
		id[2] = '\0';
		current = custom;
		while (current)
		{
			if (current->texture && ft_strcmp(current->id, id) == 0)
				return (current->texture);
			current = current->next;
		}
		i++;
	}
	return (NULL);
}

static xpm_t	*get_floor_texture(const t_map *map, t_textures *textures,
		float world_x, float world_y)
{
	int		cell_x;
	int		cell_y;
	char	cell;
	xpm_t	*custom;

	if (!textures)
		return (NULL);
	cell_x = (int)floorf(world_x);
	cell_y = (int)floorf(world_y);
	if (!map || !map->grid || cell_y < 0 || cell_y >= map->height)
		return (textures->floor);
	if (cell_x < 0 || cell_x >= (int)ft_strlen(map->grid[cell_y]))
		return (textures->floor);
	cell = map->grid[cell_y][cell_x];
	custom = find_custom_floor_texture(textures->custom, cell);
	if (custom)
		return (custom);
	return (textures->floor);
}

static void	render_floor_fill(unsigned int y, mlx_image_t *img,
		const t_map *map, t_textures *textures, vertex_t floor_and_steps, float fog)
{
	unsigned int	x;
	float			f[2];
	int				t[2];
	xpm_t			*xpm;

	x = 0;
	while (x < img->width)
	{
		xpm = get_floor_texture(map, textures, floor_and_steps.x,
				floor_and_steps.y);
		if (!xpm)
			return ;
		f[X] = floor_and_steps.x - floor(floor_and_steps.x);
		f[Y] = floor_and_steps.y - floor(floor_and_steps.y);
		t[X] = (int)(f[X] * xpm->texture.width);
		t[Y] = (int)(f[Y] * xpm->texture.height);
		t[X] = (int)clamp((float)t[X], 0.0f, xpm->texture.width - 1); 
		t[Y] = (int)clamp((float)t[Y], 0.0f, xpm->texture.height - 1); 
		paint_horizontal_line_texture(y, x, img, xpm, t[Y], t[X], fog);
		floor_and_steps.x += floor_and_steps.u;
		floor_and_steps.y += floor_and_steps.v;
		x++;
	}
}

float	*render_floors(t_game *game, float center, float ray_dir[4])
{
	unsigned int	i;
	vertex_t		floor_and_steps;
	float			dist[2];
	float			fog;

	i = (unsigned int)center + 1;
	while (i < game->double_buffer[NEXT]->height)
	{
		floor_and_steps = get_floor_ray_steps(&game->cub_data.player, ray_dir,
				game->double_buffer[NEXT], i, center);
		dist[X] = (floor_and_steps.x - game->cub_data.player.x) * WORLDMAP_TILE_SIZE;
		dist[Y] = (floor_and_steps.y - game->cub_data.player.y) * WORLDMAP_TILE_SIZE;
		fog = sqrtf(dist[X] * dist[X] + dist[Y] * dist[Y]);
		fog = fog_factor(fog);
		fog = fog * fog;
		render_floor_fill(i, game->double_buffer[NEXT], &game->cub_data.map,
			&game->cub_data.textures, floor_and_steps, fog);
		i++;
	}
	return (ray_dir);
}
