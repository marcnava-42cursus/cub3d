/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:20:30 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/02 12:49:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdbool.h>
# include "libft.h"
# include "structs.h"
# include "parser.h"
# include "render.h"

// Funciones principales del juego
int	init_game(t_game *game, const char *map_file);
void	cleanup_game(t_game *game);
int	run_game(t_game *game);

// Sistema de movimiento
void	init_movement_system(t_game *game);
void	print_map_2d(t_game *game);
void	move_forward(t_game *game, bool forward);
void	move_strafe(t_game *game, bool right);
void	rotate_player(t_game *game, bool right);
void	update_player_position(t_game *game);
void	key_hook(mlx_key_data_t keydata, void *param);
void	update_game_loop(void *param);

#endif
