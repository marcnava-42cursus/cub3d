/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:52:19 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 03:18:33 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "animation.h"

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
# define FOG_TILE_START 0.5f
# define FOG_TILE_END 9.5f

//MINIMAP_DIAMETER (2.0f * MINIMAP_TILE_RADIUS + 1.0f)
# define MINIMAP_DIAMETER 9.0f
//MINIMAP_WIDTH (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_WIDTH 288
//MINIMAP_HEIGHT (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE)
# define MINIMAP_HEIGHT 288
//MINIMAP_WNDW_X (MAX_WINDOW_WIDTH - MINIMAP_WIDTH - 2)
# define MINIMAP_WNDW_X 1630
# define MINIMAP_WNDW_Y 2
//MINIMAP_CNTR_X (MINIMAP_WNDW_X + MINIMAP_WIDTH / 2 - MINIMAP_PLAYER_SIZE / 2)
# define MINIMAP_CNTR_X 1766
//MINIMAP_CNTR_Y (MINIMAP_WNDW_Y + MINIMAP_HEIGHT / 2 - MINIMAP_PLAYER_SIZE / 2)
# define MINIMAP_CNTR_Y 138

// Tile size used for 2D map (world tiles are WORLDMAP_TILE_SIZE)
# define TILE_SIZE 64

/*--------------------------- BRESENHAM_CLIPPED.C ----------------------------*/
void		bresenham_clipped(vertex_t *start, vertex_t *end, mlx_image_t *img,
				int color);

/*------------------------- TEXTURE_MAPPING_BONUS.C --------------------------*/
void		render_texture_line_bonus(t_rayhit *rayhit, unsigned int x,
				t_game *game);
void		paint_vertical_line_texture_bonus(unsigned int x, int y[2],
				t_game *game, xpm_t *texture, int tex_x, float tex_pos,
				float tex_step, float fog, t_anim *anim);

/*------------------------ PAINT_PIXEL_COLOR_BONUS.C -------------------------*/
uint32_t	get_pixel_color_bonus(t_anim *anim, xpm_t *texture,
				int tex_coord[2]);
void		paint_pixel_color_bonus(t_game *game, int pixel[2],
				uint32_t pixel_color, float fog);

/*-------------------------------- MINIMAP.C ---------------------------------*/
void		minimap_init(t_game *game);
void		render_minimap_bg(t_minimap *minimap);
void		minimap_free(mlx_t *mlx, t_minimap *minimap);

/*------------------------- MINIMAP_PLAYER_VISION.C --------------------------*/
void		render_minimap_player_vision(t_minimap *minimap);

/*----------------------------- SCANLINE_FILL.C ------------------------------*/
void		fill_triangle_scanline(mlx_image_t *img, vertex_t v1, vertex_t v2,
				vertex_t v3);

/*----------------------------- MINIMAP_TILE.C -------------------------------*/
void		render_minimap_walls(t_minimap *minimap);
void		render_minimap_player(t_minimap *minimap);

/*----------------------------- MINIMAP_UTILS.C ------------------------------*/
bool		is_inside_minimap_circle(int coord_x, int coord_y);
vertex_t	rotate_point(float x, float y, float angle);
vertex_t	world_to_minimap_vertex(t_minimap *minimap, vertex_t world);

/*--------------------------------- DOORS.C ----------------------------------*/
void		render_doors(t_game *game, t_rayhit *rayhits, float center,
				float dist_to_proj_plane);

/*------------------------------- OUTLINES.C --------------------------------*/
void		add_wall_outlines(t_rayhit *rh, mlx_image_t *img, t_map *map,
				float center, float dist_to_proj_plane);

/*------------------------- FLOORS_AND_CEILINGS.C ----------------------------*/
void		render_floors_and_ceilings(t_game *game, float center,
				float ray_dir[4], float dist_to_proj_plane);

/*---------------------------------- ORB.C -----------------------------------*/
void		render_orb(t_game *game, t_rayhit *rayhits, float center,
				float ray_dir[4]);

/*---------------------------------- FOG.C -----------------------------------*/
float		fog_factor(float distance);
uint32_t	rgba_color_lerp(uint32_t color_1, uint32_t color_2, float t);

/*-------------------------------- ABSORB.C ----------------------------------*/
void		render_absorb_effects(t_game *game, t_rayhit *rh, float center,
				float dist_to_proj_plane);

/*------------------------------ LIVING_BLOCK.C ------------------------------*/
void		render_living_block(t_game *game, t_rayhit *rayhits, float center,
				float dist_to_proj_plane);
