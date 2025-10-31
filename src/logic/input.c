/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/31 12:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"

/**
 * @brief Refreshes the state of all movement and rotation keys
 *
 * This function polls the current state of all keys used for player
 * movement and rotation, updating the game's key state flags.
 * It also handles the ESC key for closing the window.
 *
 * Called every frame in the game loop.
 *
 * @param game Pointer to the game structure
 */
void	refresh_key_states(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	game->key_w_pressed = mlx_is_key_down(game->mlx, MLX_KEY_W);
	game->key_s_pressed = mlx_is_key_down(game->mlx, MLX_KEY_S);
	game->key_a_pressed = mlx_is_key_down(game->mlx, MLX_KEY_A);
	game->key_d_pressed = mlx_is_key_down(game->mlx, MLX_KEY_D);
	game->key_left_pressed = mlx_is_key_down(game->mlx, MLX_KEY_LEFT);
	game->key_right_pressed = mlx_is_key_down(game->mlx, MLX_KEY_RIGHT);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
}

/**
 * @brief Main key event handler for single-press actions
 *
 * This function handles keys that trigger one-time actions (like toggling
 * the map overlay) rather than continuous actions (like movement).
 * It's called by MLX whenever a key event occurs.
 *
 * @param keydata MLX key event data structure
 * @param param Void pointer to the game structure (casted internally)
 */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (!game)
		return ;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(game->mlx);
	else if (keydata.key == MLX_KEY_M && keydata.action == MLX_PRESS)
		toggle_map_overlay(game);
}

/**
 * @brief Callback for mouse cursor movement
 *
 * This function handles mouse movement for camera rotation.
 * The horizontal movement of the mouse rotates the player's view,
 * similar to using the left/right arrow keys.
 *
 * This callback only updates the rotation angle, rendering is handled
 * by the main game loop to avoid excessive render calls.
 *
 * When the cursor approaches the window edges, it's repositioned to the
 * center to allow infinite rotation (mouse wrapping).
 *
 * @param xpos Current X position of the cursor
 * @param ypos Current Y position of the cursor (unused)
 * @param param Void pointer to the game structure (casted internally)
 */
void	cursor_hook(double xpos, double ypos, void *param)
{
	t_game	*game;
	double	delta_x;
	float	rotation_amount;

	(void)ypos;
	game = (t_game *)param;
	if (!game)
		return ;
	if (!game->mouse_initialized)
	{
		game->last_mouse_x = xpos;
		game->mouse_initialized = true;
		return ;
	}
	delta_x = xpos - game->last_mouse_x;
	if (delta_x == 0.0)
		return ;
	rotation_amount = (float)delta_x * game->mouse_sensitivity;
	game->cub_data.player.angle += rotation_amount;
	if (game->cub_data.player.angle > FT_PI)
		game->cub_data.player.angle -= (2.0f * FT_PI);
	else if (game->cub_data.player.angle < -FT_PI)
		game->cub_data.player.angle += (2.0f * FT_PI);
	game->last_mouse_x = xpos;
}
