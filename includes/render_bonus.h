/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:52:19 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 01:09:08 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "animation.h"

/*--------------------------- BRESENHAM_CLIPPED.C ----------------------------*/
void		bresenham_clipped(vertex_t *start, vertex_t *end, mlx_image_t *img,
				int color);

/*------------------------- TEXTURE_MAPPING_BONUS.C --------------------------*/
void		render_texture_line_bonus(t_rayhit *rayhit, unsigned int x,
				t_game *game);
void		paint_vertical_line_texture_bonus(unsigned int x, int y[2],
				t_game *game, xpm_t *texture, int tex_x, float tex_pos,
				float tex_step, float fog, t_anim *anim);

/*----------------------- PAINT_TEXTURE_PIXEL_BONUS.C ------------------------*/
void		paint_texture_pixel_bonus(int pixel[2], t_game *game,
				xpm_t *texture, int tex_coord[2], float fog, t_anim *anim);

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
