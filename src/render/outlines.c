/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outlines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:35:49 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/03 01:57:56 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static bool find_neighbor_edge(t_rayhit *current, t_rayhit *neighbor)
{
	float	distance_threshold;

	distance_threshold = fmaxf(8.0f, current->distance * 0.03f);
	if (!neighbor->hit || current->cell_x != neighbor->cell_x
		|| current->cell_y != neighbor->cell_y
		|| current->side != neighbor->side
		|| fabsf(current->distance - neighbor->distance) > distance_threshold)
			return (true);
	return (false);
}

static bool is_raycast_edge(t_rayhit *rayhits, unsigned int x, mlx_image_t *img)
{
	bool		is_edge;
	t_rayhit	current;
	t_rayhit	left;
	t_rayhit	right;
	
	is_edge = false;
	current = rayhits[x];
	if (!current.hit)
		return (false);
	if (x > 0)
	{
		left = rayhits[x - 1];
		is_edge = find_neighbor_edge(&current, &left);	
	}
	if (x < img->width - 1)
	{
		right = rayhits[x + 1];
		if (!is_edge)
			is_edge = find_neighbor_edge(&current, &right);
	}
	return (is_edge);
}

static void draw_vertical_outline(int x, t_rayhit rayhit, mlx_image_t *img)
{
	int			slice_height;
	int			slice_bounds[2];
	float		wall_height;
	float		dist_to_proj_plane;
	float		current_aspect_ratio;
	float		aspect_scale;
	
	if (!rayhit.hit)
		return ;
	
	// Use the same perspective calculation as in walls.c
	// Calculate dynamic aspect ratio scaling to match walls.c
	current_aspect_ratio = (float)img->width / (float)img->height;
	aspect_scale = current_aspect_ratio / BASE_ASPECT_RATIO;
	dist_to_proj_plane = (float)img->height / (2.0f * tanf(PLAYER_FOV / 2.0f));
 	// Dynamic scaling with taller base
	wall_height = WORLDMAP_TILE_SIZE * (1.25f + aspect_scale * 0.5f);
	slice_height = (int)(wall_height * dist_to_proj_plane / rayhit.distance);
	
	slice_bounds[0] = -slice_height / 2 + (int)img->height / 2;
	slice_bounds[1] = slice_height / 2 + (int)img->height / 2;
	
	// Use paint_vertical_line_color for solid white outlines
	paint_vertical_line_color((unsigned int)x, slice_bounds, img, WHITE);
}

static void	draw_top_and_bottom_outline(int x, t_rayhit rayhit, mlx_image_t *img)
{
	int			slice_height;
	int			slice_bounds[2];
	float		wall_height;
	float		dist_to_proj_plane;
	float		current_aspect_ratio;
	float		aspect_scale;
	
	if (!rayhit.hit)
		return ;
	
	// Use the same perspective calculation as in walls.c
	// Calculate dynamic aspect ratio scaling to match walls.c
	current_aspect_ratio = (float)img->width / (float)img->height;
	aspect_scale = current_aspect_ratio / BASE_ASPECT_RATIO;
	dist_to_proj_plane = (float)img->height / (2.0f * tanf(PLAYER_FOV / 2.0f));
 	// Dynamic scaling with taller base
	wall_height = WORLDMAP_TILE_SIZE * (1.25f + aspect_scale * 0.5f);
	slice_height = (int)(wall_height * dist_to_proj_plane / rayhit.distance);
	
	slice_bounds[0] = -slice_height / 2 + (int)img->height / 2;
	slice_bounds[1] = slice_height / 2 + (int)img->height / 2;
		
	if (x >= img->width)
		return ;
	if (slice_bounds[1] >= (int)img->height)
		slice_bounds[1] = (int)img->height - 1;
	if (slice_bounds[0] < 0)
		slice_bounds[0] = 0;
	if (slice_bounds[0] >= slice_bounds[1])
		return ;
	save_pixel_to_image(img, (unsigned int)x, slice_bounds[0], WHITE);
	save_pixel_to_image(img, (unsigned int)x, slice_bounds[1], WHITE);
}

void add_wall_outlines(t_rayhit *rayhits, mlx_image_t *img)
{
	unsigned int i;
	int center_cell_x;
	int center_cell_y;
	
	i = 0;
	center_cell_x = rayhits[img->width / 2].cell_x;
	center_cell_y = rayhits[img->width / 2].cell_y;
	while (i < img->width && i < MAX_WINDOW_WIDTH)
	{
		if (rayhits[i].cell_x == center_cell_x && rayhits[i].cell_y == center_cell_y && rayhits[i].distance <= 300.0f)
		{
			if (is_raycast_edge(rayhits, i, img))
				draw_vertical_outline(i, rayhits[i], img);
			else
				draw_top_and_bottom_outline(i, rayhits[i], img);
		}
		i++;
	}
}
