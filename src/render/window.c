/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:27:46 by ivmirand          #+#    #+#             */
/*   Updated: 2025/10/07 19:00:00 by codex-cli       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

bool	window_init(t_game *game)
{
    int	monitor_width;
    int	monitor_height;
    unsigned int window_width;
    unsigned int window_height;

    monitor_width = 0;
    monitor_height = 0;
    game->mlx = mlx_init(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT, "cub3D", false);
    if (!game->mlx)
        return (false);
    mlx_get_monitor_size(0, &monitor_width, &monitor_height);
    mlx_set_window_size(game->mlx, MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);
    mlx_set_window_pos(game->mlx, (monitor_width - MAX_WINDOW_WIDTH) / 2,
                (monitor_height - MAX_WINDOW_HEIGHT) / 2);

    // Create double buffers used by raycaster pipeline
    game->double_buffer[NEXT] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
                MAX_WINDOW_HEIGHT);
    game->double_buffer[CURRENT] = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
                MAX_WINDOW_HEIGHT);
    mlx_image_to_window(game->mlx, game->double_buffer[NEXT], 0, 0);
    mlx_image_to_window(game->mlx, game->double_buffer[CURRENT], 0, 0);
    game->resolution_scale = 0.5f;

    // Create 2D map layers on top
    window_width = MAX_WINDOW_WIDTH;
    window_height = MAX_WINDOW_HEIGHT;
    game->map_layer = mlx_new_image(game->mlx, window_width, window_height);
    game->player_layer = mlx_new_image(game->mlx, window_width, window_height);
    if (!game->map_layer || !game->player_layer)
        return (false);
    for (unsigned int i = 0; i < window_width * window_height; i++)
        ((uint32_t*)game->player_layer->pixels)[i] = 0x00000000;
    game->last_player_x = -1.0f;
    game->last_player_y = -1.0f;
    game->last_player_angle = 0.0f;
    game->key_w_pressed = false;
    game->key_s_pressed = false;
    game->key_a_pressed = false;
    game->key_d_pressed = false;
    game->key_left_pressed = false;
    game->key_right_pressed = false;
    if (mlx_image_to_window(game->mlx, game->map_layer, 0, 0) < 0)
        return (false);
    game->map_layer_attached = true;
    if (mlx_image_to_window(game->mlx, game->player_layer, 0, 0) < 0)
        return (false);
    game->player_layer_attached = true;

    // Initialize minimap overlay
    minimap_init(game);
    return (true);
}

void	render_gameplay_window(t_game *game)
{
    render_bg(game);
    render_walls(game);
}

void	window_free(t_game *game)
{
    free_map_textures(game);
    if (game->map_layer)
        mlx_delete_image(game->mlx, game->map_layer);
    if (game->player_layer)
        mlx_delete_image(game->mlx, game->player_layer);
    if (game->double_buffer[NEXT])
        mlx_delete_image(game->mlx, game->double_buffer[NEXT]);
    if (game->double_buffer[CURRENT])
        mlx_delete_image(game->mlx, game->double_buffer[CURRENT]);
    minimap_free(game->mlx, &game->minimap);
    mlx_terminate(game->mlx);
}

