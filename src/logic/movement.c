/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/15 16:46:29 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "parser.h"

static int	is_elevator_char_logic(char c)
{
	const char	*set = "!\"·$%&/()=?¿";
	int			i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	find_elevator_slot(t_cub_data *data, char id)
{
	int	i;

	i = 0;
	while (i < data->elevator_id_count)
	{
		if (data->elevator_ids[i] == id)
			return (i);
		i++;
	}
	return (-1);
}

static int	get_elevator_coords(const t_floor *floor, char id, int *x, int *y)
{
	int	i;

	if (!floor)
		return (0);
	i = 0;
	while (i < floor->elevator_count)
	{
		if (floor->elevator_ids[i] == id)
		{
			*x = floor->elevator_x[i];
			*y = floor->elevator_y[i];
			return (1);
		}
		i++;
	}
	return (0);
}

static bool	switch_floor(t_game *game, char id)
{
	int		slot;
	t_floor	*from;
	t_floor	*to;
	int		tx;
	int		ty;
	double	now;

	if (!game || !game->cub_data.current_floor)
		return (false);
	slot = find_elevator_slot(&game->cub_data, id);
	if (slot < 0)
		return (false);
	now = mlx_get_time();
	from = game->cub_data.current_floor;
	to = game->cub_data.elevator_floor_a[slot];
	if (to == from)
		to = game->cub_data.elevator_floor_b[slot];
	if (!to || to == from)
		return (false);
	if (!get_elevator_coords(to, id, &tx, &ty))
	{
		printf("Error: Elevator '%c' missing target coords in %s\n",
			id, to->path);
		return (false);
	}
	game->cub_data.textures = to->textures;
	game->cub_data.floor_color = to->floor_color;
	game->cub_data.ceiling_color = to->ceiling_color;
	if (!to->textures_loaded)
	{
		if (!load_textures(&game->cub_data.textures))
		{
			printf("Error: Failed to load textures for floor %s\n", to->path);
			return (false);
		}
		to->textures = game->cub_data.textures;
		to->textures_loaded = true;
	}
	game->cub_data.current_floor = to;
	game->cub_data.map = to->map;
	game->cub_data.player_floor_index = to->index;
	if (game->cub_data.player_floor_path)
		free(game->cub_data.player_floor_path);
	game->cub_data.player_floor_path = ft_strdup(to->path);
	game->cub_data.player.x = (float)tx + 0.5f;
	game->cub_data.player.y = (float)ty + 0.5f;
	game->last_grid_x = (int)tx;
	game->last_grid_y = (int)ty;
	game->last_teleport_time = now;
	game->last_teleport_id = id;
	game->movement_lock_until = now + 1.0;
	printf("Elevator '%c': moved to floor %s (index %d) at (%d, %d)\n",
		id, to->path, to->index, tx, ty);
	return (true);
}

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
	if (game->key_w_pressed)
	{
		move_forward(game, true);
		moved = true;
	}
	if (game->key_s_pressed)
	{
		move_forward(game, false);
		moved = true;
	}
	if (game->key_a_pressed)
	{
		move_strafe(game, false);
		moved = true;
	}
	if (game->key_d_pressed)
	{
		move_strafe(game, true);
		moved = true;
	}
	if (game->key_left_pressed)
	{
		rotate_player(game, false);
		moved = true;
	}
	if (game->key_right_pressed)
	{
		rotate_player(game, true);
		moved = true;
	}
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
	char	cell;
	double	now;

	if (!game)
		return ;
	now = mlx_get_time();
	current_grid_x = (int)floor(game->cub_data.player.x);
	current_grid_y = (int)floor(game->cub_data.player.y);
	cell = ' ';
	if (game->cub_data.map.grid && current_grid_y >= 0
		&& current_grid_y < game->cub_data.map.height
		&& current_grid_x >= 0
		&& current_grid_x < (int)ft_strlen(game->cub_data.map.grid[current_grid_y]))
		cell = game->cub_data.map.grid[current_grid_y][current_grid_x];
	if (is_elevator_char_logic(cell) && now >= game->movement_lock_until)
	{
		if (switch_floor(game, cell))
		{
			current_grid_x = (int)floor(game->cub_data.player.x);
			current_grid_y = (int)floor(game->cub_data.player.y);
		}
	}
	if (game->map_2d_visible)
		update_player_position(game);
	handle_debug_map_update(game, current_grid_x, current_grid_y);
}

/**
 * @brief Main game loop function for continuous movement processing
 *
 * This function is called every frame by MLX and serves as the central
 * coordinator for the game loop. It:
 * 1. Uses MLX delta time for frame-independent movement
 * 2. Refreshes key states (from input.c)
 * 3. Processes movement input (delegates to move.c and rotation.c)
 * 4. Processes mouse rotation
 * 5. Updates rendering if movement occurred
 *
 * @param param Void pointer to game structure (casted internally)
 */
void	update_game_loop(void *param)
{
	t_game	*game;
	bool	moved;
	bool	mouse_rotated;

	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
	refresh_key_states(game);
	if (game->mlx->delta_time <= 0.0)
		return ;
	moved = process_movement_input(game);
	mouse_rotated = process_mouse_rotation(game);
	if (moved || mouse_rotated)
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
	game->movement_lock_until = 0.0;
	game->last_teleport_time = -10.0;
	game->last_teleport_id = '\0';
	game->mouse_initialized = false;
	game->mouse_delta_accumulated = 0.0f;
	game->mouse_sensitivity = 0.001f;
	game->last_mouse_x = 0.0;
	game->last_player_angle = game->cub_data.player.angle;
	game->last_grid_x = -1;
	game->last_grid_y = -1;
	render_map_2d_initial(game);
	print_map_2d(game);
	print_controls();
	init_crosshair(game);
	mlx_key_hook(game->mlx, key_hook, game);
	mlx_mouse_hook(game->mlx, mouse_hook, game);
	mlx_cursor_hook(game->mlx, cursor_hook, game);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	mlx_loop_hook(game->mlx, update_game_loop, game);
}
