/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 02:47:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include "render.h"

#include "structs.h"

#include "parser.h"

void	refresh_key_states_bonus(t_game *game);
void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);

#define HEADBOB_AMPLITUDE 5.0f
#define HEADBOB_SPEED 14.0f
#define HEADBOB_RECOVER_SPEED 30.0f
#define JUMP_PITCH_IMPULSE 700.0f
#define GRAVITY 2200.0f

void	install_movement_hooks(t_game *game)
{
	mlx_key_hook(game->mlx, key_hook_bonus, game);
	mlx_mouse_hook(game->mlx, mouse_hook_bonus, game);
	mlx_cursor_hook(game->mlx, cursor_hook_bonus, game);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	if (game->mlx->window && glfwRawMouseMotionSupported())
		glfwSetInputMode((GLFWwindow *)game->mlx->window,
			GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	mlx_loop_hook(game->mlx, update_game_loop_bonus, game);
}

/**
 * @brief Initializes the movement system and sets up input/update hooks
 *
 * This is the main entry point for initializing the entire movement system.
 * It should be called once during game initialization.
 *
 * Initialization steps:
 * 1. Set player's starting angle based on map orientation
 * 2. Initialize movement parameters (speed, etc.) from timing.c
 * 3. Initialize debug grid tracking
 * 4. Render initial game state
 * 5. Print initial debug map and controls
 * 6. Set up MLX input hooks (from input.c)
 * 7. Set up MLX game loop hook
 *
 * @param game Pointer to the game structure
 */
void	init_movement_system_bonus(t_game *game)
{
	if (!game)
		return ;
	init_player_angle(game);
	init_player_parameters(game);
	init_orb_projectile_bonus(game);
	init_movement_runtime(game);
	init_crosshair(game);
	install_movement_hooks(game);
}
