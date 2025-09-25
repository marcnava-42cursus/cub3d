/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:27:46 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/25 14:11:11 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

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
	mlx_set_window_pos(game->mlx, (monitor_width - MAX_WINDOW_WIDTH) / 2, 
			(monitor_height - MAX_WINDOW_HEIGHT) / 2);
	game->render_buf[BACKGROUND] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->render_buf[ATMOSPHERE] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->render_buf[FG_CURRENT] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->render_buf[FG_NEXT] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	minimap_init(game);
	return (true);
}

void	render_window(t_game *game)
{
	render_bg(game);
	render_walls(game);
	render_minimap_bg(game->mlx, &game->minimap);
	render_minimap_player_vision(game->mlx, &game->minimap);
	render_minimap_tiles(game->mlx, &game->cub_data.map, &game->minimap);
	render_minimap_player(game->mlx, &game->minimap);
}

void	window_free(t_game *game)
{
	mlx_delete_image(game->mlx, game->render_buf[BACKGROUND]);
	mlx_delete_image(game->mlx, game->render_buf[ATMOSPHERE]);
	mlx_delete_image(game->mlx, game->render_buf[FG_CURRENT]);
	mlx_delete_image(game->mlx, game->render_buf[FG_NEXT]);
	minimap_free(game->mlx, &game->minimap);
	mlx_terminate(game->mlx);
}
