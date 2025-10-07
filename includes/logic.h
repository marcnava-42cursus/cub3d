/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:07:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/07 19:51:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_H
# define LOGIC_H

// Forward declare main game struct
typedef struct s_game t_game;

// Input
void    refresh_key_states(t_game *game);
void    key_hook(mlx_key_data_t keydata, void *param);

// Timing
void    update_delta_time(t_game *game);
void    init_player_parameters(t_game *game);

// Movement
void    move_forward(t_game *game, bool forward);
void    move_strafe(t_game *game, bool right);
void    rotate_player(t_game *game, bool right);

// Collision
bool    is_cell_blocking(t_game *game, int cell_x, int cell_y);
bool    collides_with_wall(t_game *game, float x, float y);
void    attempt_move(t_game *game, float step_x, float step_y);

// Loop / Update
void    update_player_position(t_game *game);
void    update_game_loop(void *param);
void    init_movement_system(t_game *game);

#endif
