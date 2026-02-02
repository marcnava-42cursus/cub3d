/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/02 22:40:17 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_BONUS_H
# define LOGIC_BONUS_H

# define GLFW_INCLUDE_NONE
# include <GLFW/glfw3.h>
# include "logic.h"
# include "config_bonus.h"

# define EPSILON 0.01f

struct	s_controller_bind;

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
bool	orb_projectile_update(t_game *game, float delta_time);
bool	orb_projectile_needs_redraw(const t_game *game);
void	orb_projectile_mark_drawn(t_game *game);

// Config modal UI
void	set_config_modal_visible(t_game *game, bool visible);
void	toggle_config_modal(t_game *game);
bool	is_config_modal_open(const t_game *game);
void	update_config_modal(t_game *game);
void	config_modal_handle_key(t_game *game, mlx_key_data_t keydata);

// World map overlay
void	toggle_map_overlay_bonus(t_game *game);

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
			const struct s_controller_bind *bind,
			const GLFWgamepadstate *state, float deadzone);
bool	controller_action_pressed(t_game *game,
			const GLFWgamepadstate *state, int action);
bool	controller_button_pressed(t_game *game,
			const GLFWgamepadstate *state, int button);
bool	controller_menu_hold_quit(t_game *game,
			const GLFWgamepadstate *state);
void	controller_update_menu(t_game *game,
			const GLFWgamepadstate *state);

bool	is_cell_blocking_bonus(t_game *game, int cell_x, int cell_y);
bool	collides_with_wall_bonus(t_game *game, float x, float y);
void	attempt_move_bonus(t_game *game, float step_x, float step_y);
void	refresh_key_states_bonus(t_game *game);
void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);
void	update_game_loop_bonus(void *param);
void	init_movement_system_bonus(t_game *game);
			
// Test / Prototype
void	test_break_wall_in_front(t_game *game);
void	place_breakable_block(t_game *game);

#endif
