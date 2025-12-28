/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_player_vision.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 21:31:32 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/28 00:26:12 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	sort_vertices(vertex_t *v1, vertex_t *v2, vertex_t *v3)
{
	vertex_t	temp_vertex;

	if (v1->y > v2->y)
	{
		temp_vertex = *v1;
		*v1 = *v2;
		*v2 = temp_vertex;
	}
	if (v2->y > v3->y)
	{
		temp_vertex = *v2;
		*v2 = *v3;
		*v3 = temp_vertex;
	}
	if (v1->y > v2->y)
	{
		temp_vertex = *v1;
		*v1 = *v2;
		*v2 = temp_vertex;
	}
}

// Helper function to fill a triangle using scanline algorithm
static void	fill_triangle_scanline(mlx_image_t *img, vertex_t v1, vertex_t v2,
		vertex_t v3)
{
	int			x;
	int			y;
	float		x_left;
	float		x_right;
	float		t;
	float		temp_float;
	
	sort_vertices(&v1, &v2, &v3);
//	// Fill triangle using scanline algorithm
	y = (int)v1.y;
	while (y <= (int)v3.y)
	{
//		if (y < 0 || y >= (int)img->height)
//			continue;
//		// Calculate intersection points for this scanline
//		if (v3.y != v1.y)
//		{
//			t = (y - v1.y) / (v3.y - v1.y);
//			x_right = v1.x + t * (v3.x - v1.x);
//		}
//		else
//			x_right = v1.x;
//		if (y <= v2.y)
//		{
//			if (v2.y != v1.y)
//			{
//				t = (y - v1.y) / (v2.y - v1.y);
//				x_left = v1.x + t * (v2.x - v1.x);
//			}
//			else
//				x_left = v1.x;
//		}
//		else
//		{
//			if (v3.y != v2.y)
//			{
//				t = (y - v2.y) / (v3.y - v2.y);
//				x_left = v2.x + t * (v3.x - v2.x);
//			}
//			else
//				x_left = v2.x;
//		}
//		// Ensure left <= right
//		if (x_left > x_right)
//		{
//			temp_float = x_left; 
//			x_left = x_right; 
//			x_right = temp_float;
//		}
//		// Fill the scanline
//		x = (int)x_left;
//		while (x <= (int)x_right)
//		{
//			if (x >= 0 && x < (int)img->width &&
//				is_inside_minimap_circle(x, y))
//				save_pixel_to_image(img, x, y, GREEN);
//			x++;
//		}
		y++;
	}
}

void	render_minimap_player_vision(t_minimap *minimap)
{
	t_rayhit	rayhit;
	vertex_t	player_position;
	vertex_t	player_to_minimap;
	int			i;
	float		ray_angle;
	float		angle_start;
	float		increment;
	vertex_t	ray_endpoints[32];

	ft_bzero(minimap->player_vision->pixels, minimap->player_vision->width
			* minimap->player_vision->height * 4);
	player_position.x = (minimap->player->x + 0.25f) * WORLDMAP_TILE_SIZE;
	player_position.y = (minimap->player->y + 0.25f) * WORLDMAP_TILE_SIZE;
	player_to_minimap = world_to_minimap_vertex(minimap, player_position);
	i = 0;
	angle_start = minimap->player->angle - 0.5f * PLAYER_FOV;
	increment = PLAYER_FOV / (float)(MINIMAP_RAYS - 1);
	while (i < MINIMAP_RAYS)
	{
		ray_angle = normalize_angle(angle_start + i * increment);
		rayhit = raycast_world(minimap->map, player_position, ray_angle,
				MAX_RENDER_DISTANCE);
		ray_endpoints[i] = world_to_minimap_vertex(minimap, rayhit.position);
		i++;
	}
	i = 0;
	while (i < MINIMAP_RAYS - 1)
	{
		fill_triangle_scanline(minimap->player_vision, player_to_minimap, 
			ray_endpoints[i], ray_endpoints[i + 1]);
		i++;
	}
	i = 0;
	while (i < MINIMAP_RAYS)
	{
		bresenham_clipped(&player_to_minimap, &ray_endpoints[i],
			minimap->player_vision, GREEN);
		i++;
	}
}
