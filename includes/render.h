/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:24:48 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/18 13:31:46 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <math.h>
# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "structs.h"

# define MAX_WINDOW_WIDTH 1920
# define MAX_WINDOW_HEIGHT 1080
# define PLAYER_FOV (66.0f * M_PI / 180.0f)
# define MINIMAP_TILE_SIZE 32
# define MINIMAP_PLAYER_SIZE 16
# define MINIMAP_RADIUS 4
# define MINIMAP_DIAMETER (2.0f * MINIMAP_RADIUS + 1.0f)
# define MINIMAP_WIDTH (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_HEIGHT (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_WNDW_X (MAX_WINDOW_WIDTH - MINIMAP_WIDTH)
# define MINIMAP_WNDW_Y (MAX_WINDOW_HEIGHT - MINIMAP_HEIGHT)
# define MINIMAP_CNTR_X (MINIMAP_WNDW_X + MINIMAP_RADIUS * MINIMAP_TILE_SIZE + (MINIMAP_TILE_SIZE - MINIMAP_PLAYER_SIZE) / 2)
# define MINIMAP_CNTR_Y (MINIMAP_WNDW_Y + MINIMAP_RADIUS * MINIMAP_TILE_SIZE + (MINIMAP_TILE_SIZE - MINIMAP_PLAYER_SIZE) / 2)
# define MINIMAP_RAYS 120
# define WORLDMAP_TILE_SIZE 64.0f

# define RED 0xFF0000FF
# define GREEN 0x00FF00FF
# define BLUE 0x0000FFFF
# define YELLOW 0xFFFF00FF
# define WHITE 0xFFFFFFFF
# define BLACK 0x000000FF

/*------------------------------ BACKGROUND.C --------------------------------*/
void	render_bg(t_game* game);

/*------------------------------- BRESENHAM.C --------------------------------*/
void	bresenham(vertex_t *start, vertex_t *end, mlx_image_t *img, int color);

/*-------------------------------- MINIMAP.C ---------------------------------*/
void	minimap_init(t_game *game);
void	render_minimap_bg(mlx_t *mlx, t_minimap *minimap);
void	render_minimap_tiles(mlx_t *mlx, t_map *map, t_minimap *minimap);
void	render_minimap_player(mlx_t *mlx, t_minimap *minimap);
void	render_minimap_player_vision(mlx_t *mlx, t_minimap *minimap);
void	minimap_free(mlx_t *mlx, t_minimap *minimap);

/*-------------------------------- RAYCAST.C ---------------------------------*/
t_rayhit	raycast_world(const t_map *map, vertex_t start, float angle, float max_distance);

/*--------------------------------- WINDOW.C ---------------------------------*/
bool	window_init(t_game *game);
void	render_window(t_game *game);
void	window_free(t_game *game);

/*--------------------------------- UTILS.C ----------------------------------*/
int		t_color_to_int(t_color *color, int alpha);
int		clamp(int value, int min, int max);
float	normalize_angle(float angle);
#endif
