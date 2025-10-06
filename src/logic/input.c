/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex                                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by codex             #+#    #+#             */
/*   Updated: 2025/10/02 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void    refresh_key_states(t_game *game)
{
    game->key_w_pressed = mlx_is_key_down(game->mlx, MLX_KEY_W);
    game->key_s_pressed = mlx_is_key_down(game->mlx, MLX_KEY_S);
    game->key_a_pressed = mlx_is_key_down(game->mlx, MLX_KEY_A);
    game->key_d_pressed = mlx_is_key_down(game->mlx, MLX_KEY_D);
    game->key_left_pressed = mlx_is_key_down(game->mlx, MLX_KEY_LEFT);
    game->key_right_pressed = mlx_is_key_down(game->mlx, MLX_KEY_RIGHT);
    if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
        mlx_close_window(game->mlx);
}

void    key_hook(mlx_key_data_t keydata, void *param)
{
    t_game  *game;

    game = (t_game *)param;
    if (!game)
        return ;
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window(game->mlx);
}

