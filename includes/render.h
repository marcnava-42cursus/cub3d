/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:24:48 by ivmirand          #+#    #+#             */
/*   Updated: 2025/10/30 08:16:27 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <math.h>
# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "structs.h"
# include "libft.h"

# define MAX_WINDOW_WIDTH 1920
# define MAX_WINDOW_HEIGHT 1080
# define MINIMAP_TILE_SIZE 32
# define MINIMAP_PLAYER_SIZE 16
# define MINIMAP_RADIUS 4
# define MINIMAP_RAYS 120
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64
# define WORLDMAP_TILE_SIZE 64.0f

# define BASE_ASPECT_RATIO (16.0f / 9.0f)
# define MINIMAP_DIAMETER (2.0f * MINIMAP_RADIUS + 1.0f)
# define MINIMAP_WIDTH (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_HEIGHT (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_WNDW_X (MAX_WINDOW_WIDTH - MINIMAP_WIDTH)
# define MINIMAP_WNDW_Y (MAX_WINDOW_HEIGHT - MINIMAP_HEIGHT)
# define MINIMAP_CNTR_X (MINIMAP_WNDW_X + MINIMAP_WIDTH / 2 - MINIMAP_PLAYER_SIZE / 2)
# define MINIMAP_CNTR_Y (MINIMAP_WNDW_Y + MINIMAP_HEIGHT / 2 - MINIMAP_PLAYER_SIZE / 2)
# define PLAYER_FOV (66.0f * FT_PI / 180.0f)
# define TAU (2.0f * FT_PI)

# define RED 0xFF0000FF
# define GREEN 0x00FF00FF
# define BLUE 0x0000FFFF
# define YELLOW 0xFFFF00FF
# define WHITE 0xFFFFFFFF
# define MAGENTA 0xFF00FFFF
# define LIGHT_GREY 0xCCCCCCFF
# define MEDIUM_GREY 0x555555FF
# define DARK_GREY 0x333333FF
# define BLACK 0x000000FF

// Tile size used for 2D map (world tiles are WORLDMAP_TILE_SIZE)
# define TILE_SIZE 64

/*------------------------------ BACKGROUND.C --------------------------------*/
void	render_bg(t_game* game);

/*------------------------------- BRESENHAM.C --------------------------------*/
void	bresenham(vertex_t *start, vertex_t *end, mlx_image_t *img, int color);

/*----------------------------- DOUBLE_BUFFER.C ------------------------------*/
void	render_double_buffer(t_game *game);

/*-------------------------------- MINIMAP.C ---------------------------------*/
void	minimap_init(t_game *game);
void	render_minimap_bg(t_minimap *minimap);
void	render_minimap_tiles(t_map *map, t_minimap *minimap);
void	render_minimap_player(t_minimap *minimap);
void	render_minimap_player_vision(t_minimap *minimap);
void	minimap_free(mlx_t *mlx, t_minimap *minimap);

/*-------------------------------- RAYCAST.C ---------------------------------*/
t_rayhit	raycast_world(const t_map *map, vertex_t start, float angle,
				float max_distance);

// Bonus version
# ifdef BONUS
t_rayhit	raycast_world_bonus(const t_map *map, vertex_t start, float angle,
				float max_distance);
#  define raycast_world raycast_world_bonus
# endif

/*--------------------------------- WALLS.C ----------------------------------*/
void	render_walls(t_game *game);

# ifdef BONUS
void	render_walls_bonus(t_game *game);
#  define render_walls render_walls_bonus
# endif

/*------------------------------- OUTLINES.C --------------------------------*/
void	add_wall_outlines(t_rayhit *rayhits, mlx_image_t *img);

/*---------------------------- TEXTURE_MAPPING.C -----------------------------*/
void	render_texture_line(t_rayhit rayhit, unsigned int x, int y[2],
		int original_y[2], mlx_image_t *img, t_textures *textures);
void 	paint_vertical_line_texture(unsigned int x, int y[2], mlx_image_t *img,
			xpm_t *texture, int tex_x, float tex_pos, float tex_step);

# ifdef BONUS
void	render_texture_line_bonus(t_rayhit rayhit, unsigned int x, int y[2],
		int original_y[2], mlx_image_t *img, t_textures *textures, const t_map *map);
# endif

/*--------------------------------- WINDOW.C ---------------------------------*/
bool	window_init(t_game *game);
void	render_gameplay_window(t_game *game);
void	window_free(t_game *game);
void    set_map_overlay_visible(t_game *game, bool visible);
void    toggle_map_overlay(t_game *game);

/*--------------------------------- UTILS.C ----------------------------------*/
int		t_color_to_int(t_color *color, int alpha);
int		clamp(int value, int min, int max);
float	normalize_angle(float angle);
void 	paint_vertical_line_color(unsigned int x, int y[2], mlx_image_t *img,
			uint32_t color);
void	save_pixel_to_image(mlx_image_t *image, unsigned int x, unsigned int y,
			unsigned int color);

/*---------------------------- 2D MAP RENDERING ------------------------------*/
// Texture loading
int	load_map_textures(t_game *game);
void	free_map_textures(t_game *game);

// 2D Map rendering
void	render_map_2d(t_game *game);
void	render_map_2d_initial(t_game *game);
void	render_map_tiles_static(t_game *game);
void	render_player_dynamic(t_game *game);

#endif
