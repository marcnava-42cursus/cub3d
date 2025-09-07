/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:27:46 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/07 02:19:43 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "render.h"

bool	window_init(t_game *game)
{
	int			monitor_width;
	int			monitor_height;
	
	monitor_width = 0;
	monitor_height = 0;
	game->mlx = mlx_init(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return (false);
	mlx_get_monitor_size(0, &monitor_width, &monitor_height);
	mlx_set_window_size(game->mlx, MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);
	mlx_set_window_pos(game->mlx, (monitor_width - MAX_WINDOW_WIDTH)/2, 
			(monitor_height - MAX_WINDOW_HEIGHT)/2);
	game->bg_buf_zero = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->rc_buf_zero = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->rc_buf_one = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->atmos_buf_zero = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	minimap_init(game);
	return (true);
}

void	render_bg(t_game *game)
{
	unsigned int	x;
	unsigned int	y;
	int				sky;
	int				ground;

	y = 0;
	sky = t_color_to_int(&game->cub_data.ceiling_color, 255);
	ground = t_color_to_int(&game->cub_data.floor_color, 255);
	while (y < game->bg_buf_zero->height / 2)
	{
		x = 0;
		while (x < game->bg_buf_zero->width)
		{
			mlx_put_pixel(game->bg_buf_zero, x, y, sky);
			x++;
		}
		y++;
	}
	while (y < game->bg_buf_zero->height)
	{
		x = 0;
		while (x < game->bg_buf_zero->width)
		{
			mlx_put_pixel(game->bg_buf_zero, x, y, ground);
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->bg_buf_zero, 0, 0);
}

void	render_atmospheric_perspective(t_game *game)
{
	unsigned int	x;
	unsigned int	y;

	y = game->atmos_buf_zero->height / 6;
	while (y < game->atmos_buf_zero->height / 3)
	{
		x = 0;
		while (x < game->atmos_buf_zero->width)
		{
			if (x % 8 == 0 && y % 8 == 0)
				mlx_put_pixel(game->atmos_buf_zero, x, y, WHITE);
			x++;
		}
		y++;
	}
	while (y < game->atmos_buf_zero->height / 2)
	{
		x = 0;
		while (x < game->atmos_buf_zero->width)
		{
			if (x % 4 == 0 && y % 4 == 0)
				mlx_put_pixel(game->atmos_buf_zero, x, y, WHITE);
			x++;
		}
		y++;
	}
	while (y < game->atmos_buf_zero->height - (game->atmos_buf_zero->height / 3))
	{
		x = 0;
		while (x < game->atmos_buf_zero->width)
		{
			if (x % 2 == 0 && y % 2 == 0)
				mlx_put_pixel(game->atmos_buf_zero, x, y, BLACK);
			x++;
		}
		y++;
	}
	while (y < game->atmos_buf_zero->height - (game->atmos_buf_zero->height / 6))
	{
		x = 0;
		while (x < game->atmos_buf_zero->width)
		{
			if (x % 3 == 0 && y % 3 == 0)
				mlx_put_pixel(game->atmos_buf_zero, x, y, BLACK);
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->atmos_buf_zero, 0, 0);
}

void	window_free(t_game *game)
{
	mlx_delete_image(game->mlx, game->rc_buf_zero);
	mlx_delete_image(game->mlx, game->rc_buf_one);
	mlx_delete_image(game->mlx, game->bg_buf_zero);
	minimap_free(game->mlx, &game->minimap);
	mlx_terminate(game->mlx);
}
