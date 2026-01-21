/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:27:46 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/24 10:35:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

// Prototypes
bool	world_map_init_bonus(t_game *game);
void	free_map_textures_bonus(t_game *game);
void	minimap_init(t_game *game); // Shared
void	minimap_free(mlx_t *mlx, t_minimap *minimap); // Shared

bool	window_init_bonus(t_game *game)
{
	int	monitor_width;
	int	monitor_height;

	monitor_width = 0;
	monitor_height = 0;
	game->mlx = mlx_init(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return (false);
	mlx_get_monitor_size(0, &monitor_width, &monitor_height);
	mlx_set_window_size(game->mlx, MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);
	mlx_set_window_pos(game->mlx, (monitor_width - MAX_WINDOW_WIDTH) / 2,
		(monitor_height - MAX_WINDOW_HEIGHT) / 2);
	game->double_buffer[NEXT] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	game->double_buffer[CURRENT] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	mlx_image_to_window(game->mlx, game->double_buffer[NEXT], 0, 0);
	mlx_image_to_window(game->mlx, game->double_buffer[CURRENT], 0, 0);
	game->resolution_scale = 0.5f;
	if (!world_map_init_bonus(game))
		return (false);
	minimap_init(game);
	return (true);
}

void	window_free_bonus(t_game *game)
{
	free_map_textures_bonus(game);
	if (game->map_layer)
		mlx_delete_image(game->mlx, game->map_layer);
	if (game->player_layer)
		mlx_delete_image(game->mlx, game->player_layer);
	if (game->double_buffer[NEXT])
		mlx_delete_image(game->mlx, game->double_buffer[NEXT]);
	if (game->double_buffer[CURRENT])
		mlx_delete_image(game->mlx, game->double_buffer[CURRENT]);
	if (game->crosshair)
		mlx_delete_image(game->mlx, game->crosshair);

	// Bonus specific cleanup
	if (game->menu.modal)
		mlx_delete_image(game->mlx, game->menu.modal);
	if (game->menu.quit_label)
		mlx_delete_image(game->mlx, game->menu.quit_label);

	minimap_free(game->mlx, &game->minimap);
	mlx_terminate(game->mlx);
}
