/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:24:48 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/13 13:28:20 by ivmirand         ###   ########.fr       */
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
# define MINIMAP_TILE_RADIUS 4
# define MINIMAP_RAYS 15
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64
# define HAND_TEXTURE_WIDTH 192
# define HAND_TEXTURE_HEIGHT 192
# define THUMB_TEXTURE_WIDTH 96
# define THUMB_TEXTURE_HEIGHT 160
# define WEAPON_TEXTURE_WIDTH 128
# define WEAPON_TEXTURE_HEIGHT 128
# define WORLDMAP_TILE_SIZE 64.0f
# define MAX_RENDER_DISTANCE 2000.0f

# define BASE_ASPECT_RATIO (16.0f / 9.0f)
# define CURRENT_ASPECT_RATIO (MAX_WINDOW_WIDTH / MAX_WINDOW_HEIGHT)
# define ASPECT_SCALE (CURRENT_ASPECT_RATIO / BASE_ASPECT_RATIO)
# define MINIMAP_DIAMETER (2.0f * MINIMAP_TILE_RADIUS + 1.0f)
# define MINIMAP_WIDTH (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_HEIGHT (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_WNDW_X (MAX_WINDOW_WIDTH - MINIMAP_WIDTH)
# define MINIMAP_WNDW_Y 0
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
# define ORANGE 0xFF7F00FF
# define TRANSPARENT_BLUE 0x0000FF44

// Tile size used for 2D map (world tiles are WORLDMAP_TILE_SIZE)
# define TILE_SIZE 64

/*----------------------------- TEXTURE_ATLAS.C ------------------------------*/
void		paint_current_frame_to_image(mlx_image_t *img, t_atlas *atlas,
				int coord[2], int current_frame[2]);
void		paint_hori_flip_current_frame_to_image(mlx_image_t *img, t_atlas *atlas,
				int coord[2], int current_frame[2]);
void		atlas_init(t_atlas *atlas, char *xpm_path, unsigned int frame_width,
				unsigned int frame_height);
void		atlas_free(t_atlas *atlas);

/*-------------------------- TEXTURE_ATLAS_UTILS.C ---------------------------*/
uint8_t		*get_pixel_start(unsigned int stride, int current_frame[2],
			t_atlas *atlas);
uint32_t	get_corrected_color_from_pixel(uint8_t *row, unsigned int x,
			t_atlas *atlas);

/*------------------------------ BACKGROUND.C --------------------------------*/
void		render_bg(t_game* game);

/*------------------------------- BRESENHAM.C --------------------------------*/
void	bresenham(vertex_t *start, vertex_t *end, mlx_image_t *img, int color);
void	bresenham_clipped(vertex_t *start, vertex_t *end, mlx_image_t *img,
			int color);

/*----------------------------- DOUBLE_BUFFER.C ------------------------------*/
void	render_double_buffer(t_game *game);

/*-------------------------------- MINIMAP.C ---------------------------------*/
void	minimap_init(t_game *game);
void	render_minimap_bg(t_minimap *minimap);
void	minimap_free(mlx_t *mlx, t_minimap *minimap);

/*------------------------- MINIMAP_PLAYER_VISION.C --------------------------*/
void	render_minimap_player_vision(t_minimap *minimap);

/*----------------------------- SCANLINE_FILL.C ------------------------------*/
void	fill_triangle_scanline(mlx_image_t *img, vertex_t v1, vertex_t v2,
		vertex_t v3);

/*----------------------------- MINIMAP_TILE.C -------------------------------*/
void	render_minimap_walls(t_minimap *minimap);
void	render_minimap_player(t_minimap *minimap);

/*----------------------------- MINIMAP_UTILS.C ------------------------------*/
bool		is_inside_minimap_circle(int coord[2]);
vertex_t	rotate_point(float x, float y, float angle);
vertex_t	world_to_minimap_vertex(t_minimap *minimap, vertex_t world);

/*-------------------------------- RAYCAST.C ---------------------------------*/
t_rayhit	raycast_world(const t_map *map, vertex_t start, float angle,
				float max_distance);

/*-------------------------------- RAYHIT.C ----------------------------------*/
void	init_rayhit(t_rayhit *rayhit, vertex_t start, vertex_t *direction,
		float angle);

/*--------------------------------- WALLS.C ----------------------------------*/
void	render_walls(t_game *game, t_rayhit *rayhits);

/*-------------------------------- FLOORS.C ----------------------------------*/
void	render_floors(t_game *game);

/*------------------------------- OUTLINES.C --------------------------------*/
void	add_wall_outlines(t_rayhit *rh, mlx_image_t *img, t_map *map);

/*---------------------------- TEXTURE_MAPPING.C -----------------------------*/
uint32_t	sample_texture_pixel(xpm_t *texture, int tex_x, float tex_pos);
void	render_texture_line(t_rayhit rayhit, unsigned int x, int y[2],
			mlx_image_t *img, t_textures *textures);
void 	paint_vertical_line_texture(unsigned int x, int y[2], mlx_image_t *img,
			xpm_t *texture, int tex_x, float tex_pos, float tex_step);
void	paint_horizontal_line_texture(unsigned int y, unsigned int x,
			mlx_image_t *img, xpm_t *texture, int tex_y, float tex_x);

/*--------------------------------- WINDOW.C ---------------------------------*/
bool	window_init(t_game *game);
void	window_free(t_game *game);

/*---------------------------- GAMEPLAY_WINDOW.C -----------------------------*/
void	render_gameplay_window(t_game *game, unsigned int buffer_width);

/*--------------------------------- UTILS.C ----------------------------------*/
void	save_pixel_to_image(mlx_image_t *image, unsigned int x, unsigned int y,
			uint32_t color);
int		t_color_to_int(t_color *color, int alpha);
float	normalize_angle(float angle);
void 	paint_vertical_line_color(unsigned int x, int y[2], mlx_image_t *img,
			uint32_t color);
void	safe_put_pixel(mlx_image_t *img, int x, int y, unsigned int color);

/*--------------------------------- WINDOW.C ---------------------------------*/
bool	world_map_init(t_game *game);
void    set_map_overlay_visible(t_game *game, bool visible);
void    toggle_map_overlay(t_game *game);

/*---------------------------- 2D MAP RENDERING ------------------------------*/
// Texture loading
int		load_map_textures(t_game *game);
void	free_map_textures(t_game *game);

// 2D Map rendering
void	render_map_2d(t_game *game);
void	render_map_2d_initial(t_game *game);
void	render_map_tiles_static(t_game *game);
void	render_player_dynamic(t_game *game);

/*---------------------------- PLAYER.C --------------------------------------*/
void	render_player_overlay(t_game *game);

#endif
