/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outlines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:35:49 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/23 10:27:36 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static bool	find_neighbor_edge(t_rayhit *current, t_rayhit *neighbor)
{
	float	distance_threshold;

	distance_threshold = fmaxf(8.0f, current->distance * 0.03f);
	if (!neighbor->hit || current->cell[0] != neighbor->cell[0]
		|| current->cell[1] != neighbor->cell[1]
		|| current->side != neighbor->side
		|| fabsf(current->distance - neighbor->distance) > distance_threshold)
		return (true);
	return (false);
}

static bool	is_raycast_edge(t_rayhit *rayhits, unsigned int x, mlx_image_t *img)
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

static void	draw_vertical_outline(unsigned int x, t_rayhit rayhit,
		mlx_image_t *img, int color, float center)
{
	int			slice_height;
	int			slice_bounds[2];
	float		dist_to_proj_plane;

	if (!rayhit.hit)
		return ;
	dist_to_proj_plane = (img->width * 0.5f) / tanf(PLAYER_FOV / 2.0f);
	slice_height = (int)(WORLDMAP_TILE_SIZE * dist_to_proj_plane / rayhit.distance);
	slice_bounds[0] = -slice_height / 2 + center;
	slice_bounds[1] = slice_height / 2 + center;
	paint_vertical_line_color((unsigned int)x, slice_bounds, img, color);
}

static void	draw_top_and_bottom_outline(unsigned int x, t_rayhit rayhit,
		mlx_image_t *img, int color, float center)
{
	int			slice_height;
	int			slice_bounds[2];
	float		dist_to_proj_plane;

	if (!rayhit.hit)
		return ;
	dist_to_proj_plane = (float)img->width / (2.0f * tanf(PLAYER_FOV / 2.0f));
	slice_height = (int)(WORLDMAP_TILE_SIZE * dist_to_proj_plane / rayhit.distance);
	slice_bounds[0] = -slice_height / 2 + center;
	slice_bounds[1] = slice_height / 2 + center;
	if (x >= img->width)
		return ;
	if (slice_bounds[0] >= 0)
		save_pixel_to_image(img, x, slice_bounds[0], color);
	if (slice_bounds[1] < (int)img->height)
		save_pixel_to_image(img, x, slice_bounds[1], color);
}

void	add_wall_outlines(t_rayhit *rh, mlx_image_t *img, t_map *map,
		float center)
{
	unsigned int	i;
	int				c_cell[2];
	t_orientation	c_face;
	int				color;

	i = 0;
	c_cell[X] = rh[img->width / 2].cell[X];
	c_cell[Y] = rh[img->width / 2].cell[Y];
	c_face = rh[img->width / 2].face;
	color = GREEN;
	if (map->grid[c_cell[Y]][c_cell[X]] == '1')
		color = RED;
	while (i < img->width && i < MAX_WINDOW_WIDTH)
	{
		if (rh[i].cell[X] == c_cell[X] && rh[i].cell[Y] == c_cell[Y]
			&& rh[i].face == c_face && rh[img->width / 2].distance <= 300.0f)
		{
			if (is_raycast_edge(rh, i, img))
				draw_vertical_outline(i, rh[i], img, color, center);
			else
				draw_top_and_bottom_outline(i, rh[i], img, color, center);
		}
		i++;
	}
}
