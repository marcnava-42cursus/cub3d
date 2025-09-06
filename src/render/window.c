/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:27:46 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/06 17:47:03 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "render.h"

bool	window_init(t_game *game)
{
	int			monitor_width;
	int			monitor_height;
	
	monitor_width = 0;
	monitor_height = 0;
	game->mlx = mlx_init(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT, "cub3D", true);
	if (!game->mlx)
		return (false);
	mlx_get_monitor_size(0, &monitor_width, &monitor_height);
	mlx_set_window_size(game->mlx, MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);
	mlx_set_window_pos(game->mlx, (monitor_width - MAX_WINDOW_WIDTH)/2, 
			(monitor_height - MAX_WINDOW_HEIGHT)/2);
	game->rc_buf_zero = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->rc_buf_one = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->bg_buf_zero = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	return (true);
}

void	render_bg(t_game *game, int sky, int ground)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
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

void	window_free(t_game *game)
{
	mlx_delete_image(game->mlx, game->rc_buf_zero);
	mlx_delete_image(game->mlx, game->rc_buf_one);
	mlx_delete_image(game->mlx, game->bg_buf_zero);
	mlx_terminate(game->mlx);
}
