/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 00:04:24 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include "render.h"
#include "structs.h"
#include "parser.h"

// Prototypes for bonus functions used
/* void	render_player_dynamic_advanced(t_game *game); */
/* void	render_map_2d_initial_advanced(t_game *game); */
void	refresh_key_states_advanced(t_game *game);
void	key_hook_advanced(mlx_key_data_t keydata, void *param);
void	mouse_hook_advanced(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_advanced(double xpos, double ypos, void *param);
bool	process_mouse_rotation_advanced(t_game *game);

#define HEADBOB_AMPLITUDE 5.0f
#define HEADBOB_SPEED 14.0f
#define HEADBOB_RECOVER_SPEED 30.0f
#define JUMP_PITCH_IMPULSE 700.0f
#define GRAVITY 2200.0f

static int	is_elevator_char_logic(char c)
{
	return (get_elevator_index(c) >= 0);
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

static bool	is_elevator_open(t_game *game, char id)
{
	int	index;

	if (!game)
		return (false);
	index = get_elevator_index(id);
	if (index < 0 || index >= ELEVATOR_STATE_SLOTS)
		return (false);
	return (game->cub_data.map.elevator_states[index] == ELEVATOR_OPENED);
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

static t_floor	*get_target_floor(t_game *game, int slot)
{
	t_floor	*from;
	t_floor	*to;

	from = game->cub_data.current_floor;
	to = game->cub_data.elevator_floor_a[slot];
	if (to == from)
		to = game->cub_data.elevator_floor_b[slot];
	if (!to || to == from)
		return (NULL);
	return (to);
}

static bool	load_target_floor_textures(t_game *game, t_floor *to)
{
	game->cub_data.textures = to->textures;
	game->cub_data.floor_color = to->floor_color;
	game->cub_data.ceiling_color = to->ceiling_color;
	if (to->textures_loaded)
		return (true);
	if (!load_textures(&game->cub_data.textures))
	{
		printf("Error: Failed to load textures for floor %s\n", to->path);
		return (false);
	}
	to->textures = game->cub_data.textures;
	to->textures_loaded = true;
	return (true);
}

static void	apply_floor_switch(t_game *game, t_floor *to, int tx, int ty)
{
	game->cub_data.current_floor = to;
	game->cub_data.map = to->map;
	game->cub_data.player_floor_index = to->index;
	if (game->cub_data.player_floor_path)
		free(game->cub_data.player_floor_path);
	game->cub_data.player_floor_path = ft_strdup(to->path);
	game->cub_data.player.x = (float)tx + 0.5f;
	game->cub_data.player.y = (float)ty + 0.5f;
	game->last_grid_x = tx;
	game->last_grid_y = ty;
}

static bool	resolve_floor_destination(t_game *game, char id,
				t_elevator_target *target, double *now)
{
	int	slot;

	if (!game || !game->cub_data.current_floor)
		return (false);
	slot = find_elevator_slot(&game->cub_data, id);
	if (slot < 0)
		return (false);
	*now = mlx_get_time();
	target->floor = get_target_floor(game, slot);
	if (!target->floor)
		return (false);
	if (!get_elevator_coords(target->floor, id, &target->x, &target->y))
	{
		printf("Error: Elevator '%c' missing target coords in %s\n",
			id, target->floor->path);
		return (false);
	}
	return (true);
}

static bool	switch_floor(t_game *game, char id)
{
	t_elevator_target	target;
	double				now;

	if (!resolve_floor_destination(game, id, &target, &now))
		return (false);
	if (!load_target_floor_textures(game, target.floor))
		return (false);
	apply_floor_switch(game, target.floor, target.x, target.y);
	game->last_teleport_time = now;
	game->last_teleport_id = id;
	game->movement_lock_until = now + 1.0;
	printf("Elevator '%c': moved to floor %s (index %d) at (%d, %d)\n",
		id, target.floor->path, target.floor->index, target.x, target.y);
	return (true);
}

static float	recover_to_zero(float value, float amount)
{
	if (value > amount)
		return (value - amount);
	if (value < -amount)
		return (value + amount);
	return (0.0f);
}

static float	compute_headbob_offset(t_game *game, bool is_moving,
				bool step_started, float previous_offset)
{
	float	new_offset;

	if (is_moving)
	{
		if (step_started)
			game->headbob_phase = FT_PI_2;
		new_offset = sinf(game->headbob_phase) * HEADBOB_AMPLITUDE;
		game->headbob_phase += HEADBOB_SPEED * (float)game->mlx->delta_time;
		while (game->headbob_phase > TWO_PI)
			game->headbob_phase -= TWO_PI;
		return (new_offset);
	}
	new_offset = recover_to_zero(previous_offset,
			HEADBOB_RECOVER_SPEED * (float)game->mlx->delta_time);
	if (new_offset == 0.0f)
		game->headbob_phase = 0.0f;
	return (new_offset);
}

static bool	apply_headbob_pitch(t_game *game, bool is_moving, bool step_started,
		float previous_offset)
{
	float	new_offset;
	float	max_pitch;

	if (!game || !game->mlx)
		return (false);
	new_offset = compute_headbob_offset(game, is_moving, step_started,
			previous_offset);
	game->headbob_offset = new_offset;
	game->cub_data.player.pitch += new_offset;
	max_pitch = game->double_buffer[NEXT]->height * 0.35f;
	game->cub_data.player.pitch = clamp(game->cub_data.player.pitch,
			-max_pitch, max_pitch);
	return (fabsf(new_offset - previous_offset) > 0.0001f);
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
	(void)game;
	/*
	if (!game)
		return ;
	if (game->map_2d_visible)
		render_player_dynamic_advanced(game);
	*/
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
static void	read_move_axes(t_game *game, float *forward, float *strafe)
{
	*forward = 0.0f;
	*strafe = 0.0f;
	if (game->key_w_pressed)
		*forward += 1.0f;
	if (game->key_s_pressed)
		*forward -= 1.0f;
	if (game->key_d_pressed)
		*strafe += 1.0f;
	if (game->key_a_pressed)
		*strafe -= 1.0f;
	*forward += game->controller.move_forward;
	*strafe += game->controller.move_strafe;
	*forward = clamp(*forward, -1.0f, 1.0f);
	*strafe = clamp(*strafe, -1.0f, 1.0f);
}

static bool	apply_translation_input(t_game *game, float forward, float strafe)
{
	float	move_x;
	float	move_y;
	float	len;
	float	speed;
	float	angle;

	if (forward == 0.0f && strafe == 0.0f)
		return (false);
	angle = game->cub_data.player.angle;
	move_x = cosf(angle) * forward + cosf(angle + FT_PI_2) * strafe;
	move_y = sinf(angle) * forward + sinf(angle + FT_PI_2) * strafe;
	len = sqrtf(move_x * move_x + move_y * move_y);
	if (len > 1.0f)
	{
		move_x /= len;
		move_y /= len;
	}
	speed = game->move_speed * (float)game->mlx->delta_time;
	ATTEMPT_MOVE(game, move_x * speed, move_y * speed);
	return (true);
}

static bool	apply_rotation_input(t_game *game)
{
	bool	moved;
	float	turn;

	moved = false;
	if (game->key_left_pressed)
		moved = (rotate_player(game, false), true);
	if (game->key_right_pressed)
		moved = (rotate_player(game, true), true);
	turn = game->controller.turn;
	if (turn > EPSILON || turn < -EPSILON)
	{
		game->cub_data.player.angle += game->rot_speed
			* (float)game->mlx->delta_time * turn;
		game->cub_data.player.angle = normalize_angle(
				game->cub_data.player.angle);
		moved = true;
	}
	return (moved);
}

static bool	apply_pitch_input(t_game *game)
{
	bool	moved;
	float	look;
	float	max_pitch;
	float	pitch_delta;

	moved = false;
	if (game->key_up_pressed)
		moved = (pitch_player(game, true), true);
	if (game->key_down_pressed)
		moved = (pitch_player(game, false), true);
	look = game->controller.look;
	if (look > EPSILON || look < -EPSILON)
	{
		pitch_delta = 600.0f * (float)game->mlx->delta_time * look;
		game->cub_data.player.pitch += pitch_delta;
		max_pitch = game->double_buffer[NEXT]->height * 0.35f;
		game->cub_data.player.pitch = clamp(game->cub_data.player.pitch,
				-max_pitch, max_pitch);
		moved = true;
	}
	return (moved);
}

static bool	process_movement_input(t_game *game)
{
	bool	moved;
	float	forward;
	float	strafe;

	if (!game)
		return (false);
	moved = false;
	read_move_axes(game, &forward, &strafe);
	moved = apply_translation_input(game, forward, strafe) || moved;
	moved = apply_rotation_input(game) || moved;
	moved = apply_pitch_input(game) || moved;
	return (moved);
}

static char	get_player_cell(t_game *game)
{
	int	x;
	int	y;

	if (!game || !game->cub_data.map.grid)
		return (' ');
	x = (int)floor(game->cub_data.player.x);
	y = (int)floor(game->cub_data.player.y);
	if (y < 0 || y >= game->cub_data.map.height)
		return (' ');
	if (x < 0 || x >= (int)ft_strlen(game->cub_data.map.grid[y]))
		return (' ');
	return (game->cub_data.map.grid[y][x]);
}

static void	try_switch_on_elevator(t_game *game, char cell, double now)
{
	if (!is_elevator_char_logic(cell))
		return ;
	if (now < game->movement_lock_until || !is_elevator_open(game, cell))
		return ;
	switch_floor(game, cell);
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
	char	cell;
	double	now;

	if (!game)
		return ;
	now = mlx_get_time();
	cell = get_player_cell(game);
	try_switch_on_elevator(game, cell, now);
	/*
	if (game->map_2d_visible)
		update_player_position(game);
	*/
}

static void	reset_headbob_state(t_game *game)
{
	game->headbob_offset = 0.0f;
	game->headbob_phase = 0.0f;
}

static void	reset_jump_state(t_game *game)
{
	game->jump_active = false;
	game->jump_offset = 0.0f;
	game->jump_velocity = 0.0f;
}

static vertex_t	capture_player_position(t_game *game)
{
	vertex_t	snapshot;

	snapshot.x = game->cub_data.player.x;
	snapshot.y = game->cub_data.player.y;
	return (snapshot);
}

static bool	player_translated_since(t_game *game,
				vertex_t prev)
{
	return (fabsf(game->cub_data.player.x - prev.x) > 0.0001f
		|| fabsf(game->cub_data.player.y - prev.y) > 0.0001f);
}

static bool	update_step_audio_and_headbob(t_game *game, bool player_translated,
				float prev_headbob)
{
	bool	step_started;

	step_started = audio_step_update_loop(player_translated);
	return (apply_headbob_pitch(game, player_translated, step_started,
			prev_headbob));
}

static bool	update_jump_pitch(t_game *game, float prev_jump)
{
	float	max_pitch;

	if (!game || !game->mlx)
		return (false);
	if (game->jump_active)
	{
		game->jump_velocity -= GRAVITY * (float)game->mlx->delta_time;
		game->jump_offset += game->jump_velocity * (float)game->mlx->delta_time;
		if (game->jump_offset <= 0.0f && game->jump_velocity < 0.0f)
			reset_jump_state(game);
	}
	game->cub_data.player.pitch += game->jump_offset;
	max_pitch = game->double_buffer[NEXT]->height * 0.35f;
	game->cub_data.player.pitch = clamp(game->cub_data.player.pitch,
			-max_pitch, max_pitch);
	return (fabsf(game->jump_offset - prev_jump) > 0.0001f);
}

void	trigger_jump_advanced(t_game *game)
{
	if (!game || !game->mlx || game->jump_active)
		return ;
	game->jump_active = true;
	game->jump_offset = 0.0f;
	game->jump_velocity = JUMP_PITCH_IMPULSE;
}

static bool	handle_modal_loop(t_game *game)
{
	if (!is_config_modal_open(game))
		return (false);
	audio_step_update_loop(false);
	reset_headbob_state(game);
	reset_jump_state(game);
	controller_handle_rebind_advanced(game);
	controller_update_advanced(game);
	update_config_modal(game);
	return (true);
}

static bool	throttle_fps_loop(t_game *game, double *next_tick, int *last_limit)
{
	double	now;
	double	min_step;
	int		fps_limit;

	fps_limit = config_fps_limit_value(game->menu.options.fps_limit_index);
	if (fps_limit != *last_limit)
	{
		*next_tick = 0.0;
		*last_limit = fps_limit;
	}
	if (fps_limit <= 0)
	{
		*next_tick = 0.0;
		return (false);
	}
	now = mlx_get_time();
	min_step = 1.0 / (double)fps_limit;
	if (*next_tick <= 0.0)
		*next_tick = now + min_step;
	if (now < *next_tick)
		return (true);
	*next_tick += min_step;
	while (now >= *next_tick)
		*next_tick += min_step;
	return (false);
}

static bool	update_motion_frame(t_game *game, float prev_headbob, float prev_jump)
{
	bool				moved;
	bool				mouse_rotated;
	bool				player_translated;
	bool				headbob_changed;
	bool				jump_changed;
	vertex_t			prev;

	if (game->mlx->delta_time <= 0.0)
	{
		reset_headbob_state(game);
		return (false);
	}
	prev = capture_player_position(game);
	moved = process_movement_input(game);
	mouse_rotated = process_mouse_rotation_advanced(game);
	player_translated = player_translated_since(game, prev);
	headbob_changed = update_step_audio_and_headbob(game, player_translated,
			prev_headbob);
	jump_changed = update_jump_pitch(game, prev_jump);
	if (moved || mouse_rotated || headbob_changed || jump_changed)
		handle_movement_rendering(game);
	return (true);
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
void	update_game_loop_advanced(void *param)
{
	t_game			*game;
	float			prev_headbob;
	float			prev_jump;
	static double	next_tick = 0.0;
	static int		last_limit = -2;

	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
	prev_headbob = game->headbob_offset;
	prev_jump = game->jump_offset;
	if (prev_headbob != 0.0f)
		game->cub_data.player.pitch -= prev_headbob;
	if (prev_jump != 0.0f)
		game->cub_data.player.pitch -= prev_jump;
	if (handle_modal_loop(game))
		return ;
	if (throttle_fps_loop(game, &next_tick, &last_limit))
		return ;
	refresh_key_states_advanced(game);
	if (is_config_modal_open(game))
	{
		audio_step_update_loop(false);
		reset_headbob_state(game);
		reset_jump_state(game);
		update_config_modal(game);
		return ;
	}
	fps_overlay_update(game);
	update_motion_frame(game, prev_headbob, prev_jump);
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

static void	init_movement_runtime(t_game *game)
{
	game->movement_lock_until = 0.0;
	game->last_teleport_time = -10.0;
	game->last_teleport_id = '\0';
	game->mouse_initialized = false;
	game->mouse_delta_accumulated = 0.0f;
	game->mouse_delta_accumulated_y = 0.0f;
	game->mouse_sensitivity = config_mouse_sens_value(
			game->menu.options.mouse_sens);
	game->last_mouse_x = 0.0;
	game->last_mouse_y = 0.0;
	game->headbob_phase = 0.0f;
	game->headbob_offset = 0.0f;
	game->jump_active = false;
	game->jump_offset = 0.0f;
	game->jump_velocity = 0.0f;
	game->last_player_angle = game->cub_data.player.angle;
	game->last_grid_x = -1;
	game->last_grid_y = -1;
}

static void	install_movement_hooks(t_game *game)
{
	mlx_key_hook(game->mlx, key_hook_advanced, game);
	mlx_mouse_hook(game->mlx, mouse_hook_advanced, game);
	mlx_cursor_hook(game->mlx, cursor_hook_advanced, game);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	if (game->mlx->window && glfwRawMouseMotionSupported())
		glfwSetInputMode((GLFWwindow *)game->mlx->window,
			GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	mlx_loop_hook(game->mlx, update_game_loop_advanced, game);
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
void	init_movement_system_advanced(t_game *game)
{
	if (!game)
		return ;
	init_player_angle(game);
	init_player_parameters(game);
	init_orb_projectile_advanced(game);
	init_movement_runtime(game);
	init_crosshair(game);
	install_movement_hooks(game);
}
