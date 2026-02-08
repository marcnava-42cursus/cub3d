/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 04:20:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_BONUS_H
# define LOGIC_BONUS_H

# define GLFW_INCLUDE_NONE
# include <GLFW/glfw3.h>
# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "logic.h"
# include "config_bonus.h"

# define EPSILON 0.01f

struct	s_controller_bind;
typedef struct s_cub_data				t_cub_data;
typedef struct s_rayhit					t_rayhit;
typedef struct s_floor					t_floor;
typedef struct s_elevator_target		t_elevator_target;

// Collision bonus
bool	is_cell_blocking_bonus(t_game *game, int cell_x, int cell_y);
bool	collides_with_wall_bonus(t_game *game, float x, float y);
void	attempt_move_bonus(t_game *game, float step_x, float step_y);

// Input bonus
void	refresh_key_states_bonus(t_game *game);
void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);

// Movement bonus
void	update_game_loop_bonus(void *param);
void	init_movement_system_bonus(t_game *game);
void	trigger_jump_bonus(t_game *game);

// Movement internals (modular split)
int		is_elevator_char_logic(char c);
int		find_elevator_slot(t_cub_data *data, char id);
bool	is_elevator_open(t_game *game, char id);
int		get_elevator_coords(const t_floor *floor, char id, int *x, int *y);
t_floor	*get_target_floor(t_game *game, int slot);
bool	load_target_floor_textures(t_game *game, t_floor *to);
void	apply_floor_switch(t_game *game, t_floor *to, int tx, int ty);
bool	resolve_floor_destination(t_game *game, char id,
			t_elevator_target *target, double *now);
bool	switch_floor(t_game *game, char id);
float	recover_to_zero(float value, float amount);
float	compute_headbob_offset(t_game *game, bool is_moving, bool step_started,
			float previous_offset);
bool	apply_headbob_pitch(t_game *game, bool is_moving, bool step_started,
			float prev_headbob);
void	read_move_axes(t_game *game, float *forward, float *strafe);
bool	apply_translation_input(t_game *game, float forward, float strafe);
bool	apply_rotation_input(t_game *game);
bool	apply_pitch_input(t_game *game);
bool	process_movement_input(t_game *game);
char	get_player_cell(t_game *game);
void	try_switch_on_elevator(t_game *game, char cell, double now);
void	handle_movement_rendering(t_game *game);
void	reset_headbob_state(t_game *game);
void	reset_jump_state(t_game *game);
void	clear_pitch_offsets(t_game *game);
vertex_t	capture_player_position(t_game *game);
bool	player_translated_since(t_game *game, vertex_t previous);
bool	update_step_audio_and_headbob(t_game *game, bool player_translated,
			float prev_headbob);
bool	update_jump_pitch(t_game *game, float prev_jump);
bool	handle_modal_loop(t_game *game);
bool	throttle_fps_loop(t_game *game, double *next_tick, int *last_limit);
bool	handle_modal_overlay_loop(t_game *game);
bool	update_motion_frame(t_game *game, float prev_headbob, float prev_jump);
void	init_player_angle(t_game *game);
void	init_movement_runtime(t_game *game);
void	install_movement_hooks(t_game *game);

// Inventory
bool	player_has_block(const t_player *player);
bool	store_block_in_inventory(t_player *player, char block);
char	consume_inventory_block(t_player *player);

// Orb projectile
void	init_orb_projectile_bonus(t_game *game);
bool	orb_projectile_is_active(const t_game *game);
bool	orb_projectile_start_take(t_game *game, int cell_x, int cell_y,
			char block);
bool	orb_projectile_start_place(t_game *game, int target_x, int target_y,
			char block);
bool	orb_projectile_start_elevator_place(t_game *game,
			const int target_cell[2], char block, int elevator_slot);
bool	orb_projectile_update(t_game *game, float delta_time);
bool	orb_projectile_needs_redraw(const t_game *game);
void	orb_projectile_mark_drawn(t_game *game);
bool	orb_projectile_spawn_ghost(t_game *game, int target_x, int target_y);
void	orb_projectile_clear_ghost(t_game *game);
void	orb_projectile_apply_elevator_arrival_bonus(t_game *game);
void	orb_projectile_place_block_on_arrival_bonus(t_game *game);

// Audio bonus
bool	audio_system_init(void);
void	audio_system_shutdown(void);
void	audio_orb_start(void);
void	audio_orb_stop(void);
void	audio_orb_update_volume(const t_game *game);
bool	audio_step_set_loop(bool enabled);
bool	audio_step_update_loop(bool player_is_moving);

// Config modal UI
void	set_config_modal_visible(t_game *game, bool visible);
void	toggle_config_modal(t_game *game);
bool	is_config_modal_open(const t_game *game);
void	update_config_modal(t_game *game);
void	config_modal_handle_key(t_game *game, mlx_key_data_t keydata);

// World map overlay
void	toggle_map_overlay_bonus(t_game *game);

// Elevator helpers
int		get_elevator_index(char elevator_id);

// Controller (raw)
void	controller_init_bonus(t_game *game);
void	controller_update_bonus(t_game *game);
bool	controller_handle_rebind_bonus(t_game *game);
bool	controller_poll_state(t_game *game, GLFWgamepadstate *state);
void	controller_calibrate_axes(t_game *game,
			const GLFWgamepadstate *state);
float	controller_axis_delta(const t_game *game,
			const GLFWgamepadstate *state, int axis);
void	controller_store_raw_state(t_game *game,
			const GLFWgamepadstate *state);
bool	controller_action_active(t_game *game, int action,
			const GLFWgamepadstate *state, float deadzone);
bool	controller_action_pressed(t_game *game,
			const GLFWgamepadstate *state, int action);
bool	controller_button_pressed(t_game *game,
			const GLFWgamepadstate *state, int button);
bool	controller_menu_hold_quit(t_game *game,
			const GLFWgamepadstate *state);
void	controller_update_menu(t_game *game,
			const GLFWgamepadstate *state);

// Controller internals (modular split)
float	controller_action_value(t_game *game, int action,
			const GLFWgamepadstate *state, float deadzone);
bool	controller_prepare_update(t_game *game, GLFWgamepadstate *state,
			bool active[CONFIG_MODAL_CONTROL_COUNT], float *deadzone);
void	controller_try_toggle_menu(t_game *game,
			bool active[CONFIG_MODAL_CONTROL_COUNT]);
bool	controller_handle_modal_state(t_game *game,
			const GLFWgamepadstate *state,
			bool active[CONFIG_MODAL_CONTROL_COUNT]);
void	controller_handle_gameplay_actions(t_game *game,
			bool active[CONFIG_MODAL_CONTROL_COUNT]);
void	controller_store_prev_actions(t_game *game,
			bool active[CONFIG_MODAL_CONTROL_COUNT]);
void	controller_menu_sync_settings_selection(t_game *game);
void	controller_menu_advance_section(t_game *game, int delta);
bool	controller_menu_handle_left_column(t_game *game, const bool in[7]);
bool	controller_menu_handle_controls(t_game *game, bool up, bool down,
			bool confirm);
void	controller_menu_handle_settings(t_game *game, const bool in[7]);
void	controller_refresh_texts(t_game *game);
t_controller_bind	controller_bind_button(int id);
t_controller_bind	controller_bind_axis(int id, int dir);
void	controller_set_default_binds(t_game *game);
bool	controller_detect_pressed_button(t_game *game,
			const GLFWgamepadstate *state, t_controller_bind *bind);
bool	controller_detect_pressed_axis(t_game *game,
			const GLFWgamepadstate *state, float deadzone,
			t_controller_bind *bind);
bool	controller_find_new_bind(t_game *game,
			const GLFWgamepadstate *state, float deadzone,
			t_controller_bind *bind);

// Test / Prototype
void	test_break_wall_in_front(t_game *game);
void	place_breakable_block(t_game *game);
void	update_creating_block_state(t_game *game);

// State swapper internals
int		get_elevator_slot_bonus(t_cub_data *data, char id);
bool	state_is_elevator_bonus(char c);
bool	state_target_inside_map_bonus(t_game *game, int x, int y);
bool	state_target_intersects_player_bonus(t_game *game, int x, int y);
bool	state_is_valid_target_cell_bonus(t_game *game, int x, int y);
bool	state_modify_interactive_cell(t_game *game, int x, int y);
bool	state_get_attachment_coords_bonus(t_game *game, t_rayhit *hit,
			int *x, int *y);
bool	state_handle_elevator_orb_place_bonus(t_game *game,
			t_rayhit *hit, char cell);
bool	state_get_place_hit_bonus(t_game *game, t_rayhit *hit, char *cell);
bool	state_begin_block_creation_bonus(t_game *game,
			int target_x, int target_y, char cell);
bool	state_can_place_block_now_bonus(t_game *game);
bool	state_resolve_place_target_bonus(t_game *game, t_rayhit *hit,
			char *cell, int target[2]);
bool	state_handle_elevator_orb_take_bonus(t_game *game,
			t_rayhit *hit, char cell);
void	state_handle_breakable_cell_bonus(t_game *game, t_rayhit *hit,
			char cell);

#endif
