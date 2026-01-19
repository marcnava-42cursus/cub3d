/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/03 12:39:15 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

// heap allocated
static float	*get_ray_dir(float angle)
{
	float			player_dir[2];
	float			camera_plane[2];
	float			*ray_dir;

	ray_dir = ft_calloc(4, sizeof(float));
	if (ray_dir == NULL)
		return (NULL);
	player_dir[X] = cosf(angle);
	player_dir[Y] = sinf(angle);
	camera_plane[X] = -player_dir[Y] * tanf(PLAYER_FOV / 2.0f);
	camera_plane[Y] = player_dir[X] * tanf(PLAYER_FOV / 2.0f);
	ray_dir[X] = player_dir[X] - camera_plane[X];
	ray_dir[Y] = player_dir[Y] - camera_plane[Y];
	ray_dir[U] = player_dir[X] + camera_plane[X];
	ray_dir[V] = player_dir[Y] + camera_plane[Y];
	return (ray_dir);
}

static vertex_t	get_floor_ray_steps(t_player *player, float ray_dir[4],
		mlx_image_t *img, int y)
{
	vertex_t	floor_and_steps;
	float		center;
	float		row_distance;

	center = 0.5f * img->height;
	row_distance = center / (y - center);
	floor_and_steps.x = (player->x) + row_distance * ray_dir[X];
	floor_and_steps.y = (player->y) + row_distance * ray_dir[Y];
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
		return (textures->north);
	if (cell_x < 0 || cell_x >= (int)ft_strlen(map->grid[cell_y]))
		return (textures->north);
	cell = map->grid[cell_y][cell_x];
	custom = find_custom_floor_texture(textures->custom, cell);
	if (custom)
		return (custom);
	return (textures->north);
}

static void	render_floor_fill(unsigned int y, mlx_image_t *img,
		const t_map *map, t_textures *textures, vertex_t floor_and_steps)
{
	unsigned int	x;
	float			tile[2];
	int				t[2];
	xpm_t			*xpm;

	x = 0;
	while (x < img->width)
	{
		xpm = get_floor_texture(map, textures, floor_and_steps.x,
				floor_and_steps.y);
		if (!xpm)
			return ;
		tile[X] = floor_and_steps.x;
		tile[Y] = floor_and_steps.y;
		t[X] = (int)((tile[X] - floorf(tile[X])) * xpm->texture.width)
			& (xpm->texture.width - 1);
		t[Y] = (int)((tile[Y] - floorf(tile[Y])) * xpm->texture.height)
			& (xpm->texture.height - 1);
		paint_horizontal_line_texture(y, x, img, xpm, t[Y], t[X]);
		floor_and_steps.x += floor_and_steps.u;
		floor_and_steps.y += floor_and_steps.v;
		x++;
	}
}

// must free ray_dir
void	render_floors(t_game *game)
{
	unsigned int	i;
	float			*ray_dir;
	vertex_t		floor_and_steps;

	i = game->double_buffer[NEXT]->height / 2;
	ray_dir = get_ray_dir(game->cub_data.player.angle);
	while (i < game->double_buffer[NEXT]->height)
	{
		floor_and_steps = get_floor_ray_steps(&game->cub_data.player, ray_dir,
				game->double_buffer[NEXT], i);
		render_floor_fill(i, game->double_buffer[NEXT], &game->cub_data.map,
			&game->cub_data.textures, floor_and_steps);
		i++;
	}
	free(ray_dir);
}
