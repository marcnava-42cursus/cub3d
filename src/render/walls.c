/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:51:39 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/19 15:27:31 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void paint_vertical_line(int x, int y_start, int y_end, mlx_image_t *img)
{
	int	i;
	
	if (x < 0 || x >= (int)img->width)
		return ;
	
	// Clamp y coordinates to image bounds
	if (y_start < 0)
		y_start = 0;
	if (y_end >= (int)img->height)
		y_end = (int)img->height - 1;
	
	if (y_start >= y_end)
		return ;
	
	i = y_start;
	while (i <= y_end)
	{
		mlx_put_pixel(img, x, i, BLACK);
		i++;
	}
}

static t_rayhit cast_ray_for_column(t_cub_data *cub_data, int x)
{
	const float MAX_DIST = 2000.0f;
	t_rayhit	rayhit;
	vertex_t	player_position;
	float		ray_angle;
	float		camera_x;

	camera_x = 2.0f * x / (float)MAX_WINDOW_WIDTH - 1.0f;
	ray_angle =
		cub_data->player.angle + atanf(-camera_x * tanf(PLAYER_FOV / 2.0f));
	
	player_position.x = ((float)cub_data->player.x + 0.5f) * WORLDMAP_TILE_SIZE;
	player_position.y = ((float)cub_data->player.y + 0.5f) * WORLDMAP_TILE_SIZE;
	rayhit =
		raycast_world(&cub_data->map, player_position, ray_angle, MAX_DIST);
	return (rayhit);
}

static void render_wall_fill(t_rayhit rayhit, int x, mlx_image_t *img)
{
	int		slice_height;
	int		draw_start;
	int		draw_end;
	int		original_draw_start;
	int		original_draw_end;
	int		fill_start;
	int		fill_end;
	
	if (!rayhit.hit)
		return ;
	
	slice_height =
		(int)(WORLDMAP_TILE_SIZE * MAX_WINDOW_HEIGHT / rayhit.distance);
	
	original_draw_start = -slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	original_draw_end = slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	
	draw_start = original_draw_start;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = original_draw_end;
	if (draw_end >= MAX_WINDOW_HEIGHT)
		draw_end = MAX_WINDOW_HEIGHT - 1;
	
	if (original_draw_start >= 0 && draw_start < MAX_WINDOW_HEIGHT - 1)
	{
		mlx_put_pixel(img, x, draw_start, WHITE);
		if (draw_start + 1 <= draw_end)
			mlx_put_pixel(img, x, draw_start + 1, WHITE);
	}
	if (original_draw_end < MAX_WINDOW_HEIGHT && draw_end > 0)
	{
		mlx_put_pixel(img, x, draw_end, WHITE);
		if (draw_end - 1 >= draw_start)
			mlx_put_pixel(img, x, draw_end - 1, WHITE);
	}
	
	fill_start = draw_start;
	fill_end = draw_end;
	if (original_draw_start >= 0 && draw_start < MAX_WINDOW_HEIGHT - 1)
		fill_start = draw_start + 2;
	if (original_draw_end < MAX_WINDOW_HEIGHT && draw_end > 0)
		fill_end = draw_end - 2;
	if (fill_start <= fill_end)
		paint_vertical_line(x, fill_start, fill_end, img);
}

void render_walls(t_cub_data *cub_data, mlx_image_t *img, mlx_t *mlx)
{
	int			i;
	t_rayhit	rayhits[MAX_WINDOW_WIDTH];
	
	i = 0;
	while (i < MAX_WINDOW_WIDTH)
	{
		rayhits[i] = cast_ray_for_column(cub_data, i);
		render_wall_fill(rayhits[i], i, img);
		i++;
	}
	add_wall_outlines(rayhits, img);
	mlx_image_to_window(mlx, img, 0, 0);
}
