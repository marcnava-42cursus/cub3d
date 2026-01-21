/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/15 18:00:41 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "structs.h"

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
	{
		printf("\n");
		mlx_close_window(game->mlx);
	}
}

void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods,
			void *param)
{
	t_game	*game;

	(void)button;
	(void)action;
	(void)mods;
	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
}

/**
 * @brief Callback for mouse cursor movement
 *
 * Mouse input accumulates deltas from cursor events and is applied
 * once per frame with smoothing to reduce jitter on slow frames.
 *
 * @param xpos Current X position of the cursor
 * @param ypos Current Y position of the cursor (unused)
 * @param param Void pointer to the game structure (casted internally)
 */
void	cursor_hook(double xpos, double ypos, void *param)
{
	t_game	*game;
	double	delta_x;

	(void)ypos;
	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
	if (!game->mouse_initialized)
	{
		game->last_mouse_x = xpos;
		game->mouse_initialized = true;
		return ;
	}
	delta_x = xpos - game->last_mouse_x;
	game->last_mouse_x = xpos;
	game->mouse_delta_accumulated += (float)delta_x * game->mouse_sensitivity;
}

/**
 * @brief Processes accumulated mouse movement and applies rotation
 *
 * This function is called once per frame to apply all accumulated mouse
 * movement as rotation. This ensures smooth synchronized rotation with
 * keyboard movement.
 *
 * @param game Pointer to the game structure
 * @return bool True if the mouse rotated the camera
 */
bool	process_mouse_rotation(t_game *game)
{
	float	rotation_amount;
	float	smoothing;

	if (!game || !game->mlx || game->mouse_delta_accumulated == 0.0f)
		return (false);
	if (game->mlx->delta_time <= 0.0)
		return (false);
	smoothing = MOUSE_SMOOTHING * (float)game->mlx->delta_time;
	if (smoothing > 1.0f)
		smoothing = 1.0f;
	rotation_amount = game->mouse_delta_accumulated * smoothing;
	game->mouse_delta_accumulated -= rotation_amount;
	if (rotation_amount > -EPSILON && rotation_amount < EPSILON)
	{
		game->mouse_delta_accumulated = 0.0f;
		return (false);
	}
	game->cub_data.player.angle += rotation_amount;
	game->cub_data.player.angle = normalize_angle(game->cub_data.player.angle);
	return (true);
}
