/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:51:39 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/21 02:17:36 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_rayhit cast_ray_for_column(t_cub_data *cub_data, int x)
{
	const float MAX_DIST = 2000.0f;
	t_rayhit	rayhit;
	vertex_t	player_position;
	float		ray_angle;
	float		camera_x;

	camera_x = 2.0f * x / (float)MAX_WINDOW_WIDTH - 1.0f;
	ray_angle =
		cub_data->player.angle + atanf(camera_x * tanf(PLAYER_FOV / 2.0f));
	
	player_position.x = ((float)cub_data->player.x + 0.5f) * WORLDMAP_TILE_SIZE;
	player_position.y = ((float)cub_data->player.y + 0.5f) * WORLDMAP_TILE_SIZE;
	rayhit =
		raycast_world(&cub_data->map, player_position, ray_angle, MAX_DIST);
	return (rayhit);
}

//on all int[2] the [0] is the start and the [1] is the end
static void render_wall_fill(t_rayhit rayhit, int x, mlx_image_t *img,
		t_textures *textures)
{
	int		slice_height;
	int		draw[2];
	int		original_draw[2];
	
	if (!rayhit.hit)
		return ;
	slice_height =
		(int)(WORLDMAP_TILE_SIZE * MAX_WINDOW_HEIGHT / rayhit.distance);
	original_draw[0] = -slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	original_draw[1] = slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	draw[0] = original_draw[0];
	if (draw[0] < 0)
		draw[0] = 0;
	draw[1] = original_draw[1];
	if (draw[1] >= MAX_WINDOW_HEIGHT)
		draw[1] = MAX_WINDOW_HEIGHT - 1;
	if (original_draw[0] >= 0 && draw[0] < MAX_WINDOW_HEIGHT - 1)
	{
		mlx_put_pixel(img, x, draw[0], WHITE);
		if (draw[0] + 1 <= draw[1])
			mlx_put_pixel(img, x, draw[0] + 1, WHITE);
	}
	if (original_draw[0] < MAX_WINDOW_HEIGHT && draw[1] > 0)
	{
		mlx_put_pixel(img, x, draw[1], WHITE);
		if (draw[1] - 1 >= draw[0])
			mlx_put_pixel(img, x, draw[1] - 1, WHITE);
	}
	if (original_draw[0] >= 0 && draw[0] < MAX_WINDOW_HEIGHT - 1)
		draw[0] += 2;
	if (original_draw[1] < MAX_WINDOW_HEIGHT && draw[1] > 0)
		draw[1] -= 2;
	render_texture_line(rayhit, x, draw, img, textures);
	//if (rayhit.face == NORTH)
	//	paint_vertical_line(x, draw, img, LIGHT_GREY);
	//if (rayhit.face == SOUTH)
	//	paint_vertical_line(x, draw, img, BLACK);
	//if (rayhit.face == EAST)
	//	paint_vertical_line(x, draw, img, DARK_GREY);
	//if (rayhit.face == WEST)
	//	paint_vertical_line(x, draw, img, MEDIUM_GREY);
}

void render_walls(t_cub_data *cub_data, mlx_image_t *img, mlx_t *mlx)
{
	int			i;
	t_rayhit	rayhits[MAX_WINDOW_WIDTH];
	
	i = 0;
	while (i < MAX_WINDOW_WIDTH)
	{
		rayhits[i] = cast_ray_for_column(cub_data, i);
		render_wall_fill(rayhits[i], i, img, &cub_data->textures);
		i++;
	}
	add_wall_outlines(rayhits, img);
	mlx_image_to_window(mlx, img, 0, 0);
}
