/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:24:48 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/06 17:48:38 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "structs.h"

# define MAX_WINDOW_WIDTH 1920
# define MAX_WINDOW_HEIGHT 1080

# define RED 0xFF0000FF
# define GREEN 0x00FF00FF
# define BLUE 0x0000FFFF

# define TILE_SIZE 64

// Window functions
bool	window_init(t_game *game);
void	render_bg(t_game *game, int sky, int ground);
void	window_free(t_game *game);

// Texture loading
int	load_map_textures(t_game *game);
void	free_map_textures(t_game *game);

// 2D Map rendering
void	render_map_2d(t_game *game);
void	render_map_2d_initial(t_game *game);
void	render_map_tiles_static(t_game *game);
void	render_player_dynamic(t_game *game);

#endif
