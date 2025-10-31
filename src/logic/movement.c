/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/31 12:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"
#include <math.h>

/**
 * @file movement.c
 * @brief Main movement system controller and coordinator
 *
 * This file serves as the central entry point for the movement system.
 * It coordinates between different subsystems:
 * - input.c: Handles keyboard input and key state management
 * - move.c: Handles WASD movement (forward/backward/strafe)
 * - rotation.c: Handles arrow key camera rotation
 * - debug_map.c: Handles terminal debug output
 * - timing.c: Handles delta time calculations
 *
 * The main responsibilities of this file are:
 * 1. Initialize the movement system and set up hooks
 * 2. Coordinate the game loop (update_game_loop)
 * 3. Process all movement inputs and trigger appropriate handlers
 * 4. Update rendering after movement
 */

/* ************************************************************************** */
/*                          HELPER FUNCTIONS                                  */
/* ************************************************************************** */

/**
 * @brief Updates only the player position display
 *
 * This is called when the map overlay is visible and the player moves.
 *
 * @param game Pointer to the game structure
 */
void	update_player_position(t_game *game)
{
	if (!game)
		return ;
	if (game->map_2d_visible)
		render_player_dynamic(game);
}

/**
 * @brief Processes movement input and updates game state
 *
 * This function checks all movement-related key states and calls the
 * appropriate movement/rotation functions from move.c and rotation.c.
 *
 * @param game Game state structure
 * @return bool True if any movement occurred
 */
static bool	process_movement_input(t_game *game)
{
	bool	moved;

	if (!game)
		return (false);
	moved = false;
	if (game->key_w_pressed && (move_forward(game, true), 1))
		moved = true;
	if (game->key_s_pressed && (move_forward(game, false), 1))
		moved = true;
	if (game->key_a_pressed && (move_strafe(game, false), 1))
		moved = true;
	if (game->key_d_pressed && (move_strafe(game, true), 1))
		moved = true;
	if (game->key_left_pressed && (rotate_player(game, false), 1))
		moved = true;
	if (game->key_right_pressed && (rotate_player(game, true), 1))
		moved = true;
	return (moved);
}

/**
 * @brief Handles rendering updates after movement
 *
 * This function updates all rendering components when the player moves:
 * 1. Updates player position on 2D map overlay (if visible)
 * 2. Re-renders the 3D view with double buffering
 * 3. Updates terminal debug output
 *
 * @param game Game state structure
 */
static void	handle_movement_rendering(t_game *game)
{
	int	current_grid_x;
	int	current_grid_y;

	if (!game)
		return ;
	if (game->map_2d_visible)
		update_player_position(game);
	render_double_buffer(game);
	current_grid_x = (int)floor(game->cub_data.player.x);
	current_grid_y = (int)floor(game->cub_data.player.y);
	handle_debug_map_update(game, current_grid_x, current_grid_y);
}

/* ************************************************************************** */
/*                          MAIN FUNCTIONS                                    */
/* ************************************************************************** */

/**
 * @brief Main game loop function for continuous movement processing
 *
 * This function is called every frame by MLX and serves as the central
 * coordinator for the game loop. It:
 * 1. Updates delta time (from timing.c)
 * 2. Refreshes key states (from input.c)
 * 3. Processes movement input (delegates to move.c and rotation.c)
 * 4. Updates rendering if movement occurred
 *
 * @param param Void pointer to game structure (casted internally)
 */
void	update_game_loop(void *param)
{
	t_game	*game;
	bool	moved;

	game = (t_game *)param;
	if (!game)
		return ;
	update_delta_time(game);
	refresh_key_states(game);
	if (game->delta_time <= 0.0)
		return ;
	moved = process_movement_input(game);
	if (moved)
		handle_movement_rendering(game);
}

/**
 * @brief Initializes the player's starting angle based on orientation
 *
 * Converts the cardinal direction (N, S, E, W) from the map file
 * into a radian angle for the 3D engine.
 *
 * @param game Game state structure
 */
static void	init_player_angle(t_game *game)
{
	if (!game)
		return ;
	if (game->cub_data.player.orientation == NORTH)
		game->cub_data.player.angle = -FT_PI / 2.0f;
	else if (game->cub_data.player.orientation == SOUTH)
		game->cub_data.player.angle = FT_PI / 2.0f;
	else if (game->cub_data.player.orientation == EAST)
		game->cub_data.player.angle = 0.0f;
	else if (game->cub_data.player.orientation == WEST)
		game->cub_data.player.angle = FT_PI;
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
void	init_movement_system(t_game *game)
{
	if (!game)
		return ;
	init_player_angle(game);
	init_player_parameters(game);
	game->last_grid_x = -1;
	game->last_grid_y = -1;
	render_map_2d_initial(game);
	print_map_2d(game);
	print_controls();
	mlx_key_hook(game->mlx, key_hook, game);
	mlx_loop_hook(game->mlx, update_game_loop, game);
}
