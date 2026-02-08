/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 16:24:48 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/08 13:53:36 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <math.h>
# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"
# include "structs.h"
# include "utils.h"
# include "libft.h"

# define MAX_WINDOW_WIDTH 1920
# define MAX_WINDOW_HEIGHT 1080
# define RESOLUTION_SCALE 0.25f
# define WORLDMAP_TILE_SIZE 64.0f
# define MAX_TILE_RENDER 20.0f

//PLAYER_FOV (66.0f * FT_PI / 180.0f)
# define PLAYER_FOV 1.151917f
//TAU (2.0f * FT_PI)
# define TAU 6.290384f

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
# define FOG_COLOR 0xAA3333FF

// Render order for gameplay buffers
typedef enum e_render_order
{
	CURRENT = 0,
	NEXT = 1
}	t_render_order;

// Coordinate identifier for coordinate or dimensional arrays
typedef enum e_coord
{
	X = 0,
	Y = 1,
	U = 2,
	V = 3,
	Z = 4,
	W = 5
}	t_coord;

/*----------------------------- TEXTURE_ATLAS.C ------------------------------*/
void		atlas_init(t_atlas *atlas, char *xpm_path, unsigned int frame_width,
				unsigned int frame_height);
void		atlas_free(t_atlas *atlas);

/*----------------------------- FRAME_TO_IMAGE.C -----------------------------*/
void		frame_to_image(mlx_image_t *img, t_atlas *atlas,
				int coord[2], unsigned int frame[2]);
void		hori_flip_frame_to_image(mlx_image_t *img, t_atlas *atlas,
				int coord[2], unsigned int frame[2]);

/*-------------------------- TEXTURE_ATLAS_UTILS.C ---------------------------*/
uint8_t		*get_pixel_start(unsigned int stride, unsigned int current_frame[2],
				t_atlas *atlas);
uint32_t	get_corrected_color_from_pixel(uint8_t *row, unsigned int x,
				t_atlas *atlas);

/*------------------------------ BACKGROUND.C --------------------------------*/
void		render_bg_color(t_game *game, int sky, int ground);
void		render_bg(t_game *game);

/*----------------------------- DOUBLE_BUFFER.C ------------------------------*/
void		render_double_buffer(t_game *game);

/*-------------------------------- RAYCAST.C ---------------------------------*/
t_rayhit	raycast_world(const t_map *map, vertex_t start, float angle,
				float max_distance);

/*-------------------------------- RAYHIT.C ----------------------------------*/
void		init_rayhit(t_rayhit *rayhit, vertex_t start, vertex_t *direction,
				float angle);

/*--------------------------------- WALLS.C ----------------------------------*/
void		render_walls(t_game *game, t_rayhit *rayhits, float center,
				float dist_to_proj_plane);

/*---------------------------- TEXTURE_MAPPING.C -----------------------------*/
void		render_texture_line(t_rayhit *rayhit, unsigned int x, int y[2],
				t_game *game);
void		paint_vertical_line_texture(int x_y_packed[3], mlx_image_t *img,
				xpm_t *texture, float x_offset_step[3]);
void		paint_texture_pixel(unsigned int pixel[2], mlx_image_t *img,
				int tex_coord[2], xpm_t *texture);

/*--------------------------------- WINDOW.C ---------------------------------*/
bool		window_init(t_game *game);
void		window_free(t_game *game);

/*---------------------------- GAMEPLAY_WINDOW.C -----------------------------*/
void		render_gameplay_window(t_game *game, unsigned int buffer_width);

/*--------------------------------- UTILS.C ----------------------------------*/
void		save_pixel_to_image(mlx_image_t *image, unsigned int x,
				unsigned int y, uint32_t color);
void		pack_x_ys(int x, int y[2], int x_y_packed[3]);
float		normalize_angle(float angle);
void		build_wall_bounds(t_rayhit *rayhit, float center,
				float dist_to_proj_plane);
void		safe_put_pixel(mlx_image_t *img, int x, int y, unsigned int color);

/*--------------------------------- WINDOW.C ---------------------------------*/
// bool		world_map_init(t_game *game);
// void		set_map_overlay_visible(t_game *game, bool visible);
// void		toggle_map_overlay(t_game *game);

/*---------------------------- PLAYER.C --------------------------------------*/
void		render_player_overlay(t_game *game);
#endif
