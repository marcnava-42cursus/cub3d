/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ceilings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 00:28:55 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/23 02:16:53 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	get_ray_dir(float angle, float ray_dir[4])
{
	float			player_dir[2];
	float			camera_plane[2];

	player_dir[X] = cosf(angle);
	player_dir[Y] = sinf(angle);
	camera_plane[X] = -player_dir[Y] * tanf(PLAYER_FOV / 2.0f);
	camera_plane[Y] = player_dir[X] * tanf(PLAYER_FOV / 2.0f);
	ray_dir[X] = player_dir[X] - camera_plane[X];
	ray_dir[Y] = player_dir[Y] - camera_plane[Y];
	ray_dir[U] = player_dir[X] + camera_plane[X];
	ray_dir[V] = player_dir[Y] + camera_plane[Y];
}

static vertex_t	get_ceiling_ray_steps(t_player *player, float ray_dir[4],
		mlx_image_t *img, int y)
{
	vertex_t	floor_and_steps;
	float		center;
	float		p;
	float		row_distance;
	float		dist_to_proj_plane;

	center = 0.5f * img->height + player->pitch;
	if (center < 1.0f)
		center = 1.0f;
	if (center > img->height - 2.0f)
		center = img->height - 2.0f;
	p = center - y;
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

static void	render_ceiling_fill(unsigned int y, mlx_image_t *img,
		xpm_t *xpm, vertex_t floor_and_steps)
{
	unsigned int	x;
	float			f[2];
	int				t[2];

	x = 0;
	while (x < img->width)
	{
		f[X] = floor_and_steps.x - floor(floor_and_steps.x);
		f[Y] = floor_and_steps.y - floor(floor_and_steps.y);
		t[X] = (int)(f[X] * xpm->texture.width);
		t[Y] = (int)(f[Y] * xpm->texture.height);
		if (t[X] < 0)
			t[X] = 0;
		if (t[X] >= (int)xpm->texture.width)
			t[X] = xpm->texture.width - 1;
		if (t[Y] < 0)
			t[Y] = 0;
		if (t[Y] >= (int)xpm->texture.height)
			t[Y] = xpm->texture.height - 1;
		paint_horizontal_line_texture(y, x, img, xpm, t[Y], t[X]);
		floor_and_steps.x += floor_and_steps.u;
		floor_and_steps.y += floor_and_steps.v;
		x++;
	}
}

void	render_ceilings(t_game *game)
{
	int	i;
	float			ray_dir[4];
	vertex_t		floor_and_steps;
	float			center;

	center = game->double_buffer[NEXT]->height * 0.5f + game->cub_data.player.pitch;
	if (center < 1.0f)
		center = 1.0f;
	if (center > game->double_buffer[NEXT]->height - 2.0f)
		center = game->double_buffer[NEXT]->height - 2.0f;
	i = 0;
	get_ray_dir(game->cub_data.player.angle, ray_dir);
	while (i < (int)center)
	{
		floor_and_steps = get_ceiling_ray_steps(&game->cub_data.player, ray_dir,
				game->double_buffer[NEXT], i);
		render_ceiling_fill(i, game->double_buffer[NEXT],
			game->cub_data.textures.north, floor_and_steps);
		i++;
	}
}
