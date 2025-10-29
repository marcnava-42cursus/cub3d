/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:46:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/25 03:05:27 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	minimap_init(t_game *game)
{
	game->minimap.map = &game->cub_data.map;
	game->minimap.player = &game->cub_data.player;
	game->minimap.bg = mlx_new_image(game->mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);
	game->minimap.player_sprite = mlx_new_image(game->mlx, MINIMAP_PLAYER_SIZE,
			MINIMAP_PLAYER_SIZE);
	game->minimap.player_vision = mlx_new_image(game->mlx, MINIMAP_WIDTH,
			MINIMAP_HEIGHT);
	game->minimap.tile = mlx_new_image(game->mlx, MINIMAP_TILE_SIZE,
			MINIMAP_TILE_SIZE);
	
	// Initialize minimap images to window once (order matters: bg -> vision -> player)
	mlx_image_to_window(game->mlx, game->minimap.bg, MINIMAP_WNDW_X, MINIMAP_WNDW_Y);
	mlx_image_to_window(game->mlx, game->minimap.player_vision, MINIMAP_WNDW_X, MINIMAP_WNDW_Y);
	mlx_image_to_window(game->mlx, game->minimap.player_sprite, MINIMAP_CNTR_X, MINIMAP_CNTR_Y);
}

// Helper function to check if a point is inside the circular minimap
static bool	is_inside_minimap_circle(int x, int y, int center_x, int center_y, int radius)
{
	int	dx = x - center_x;
	int	dy = y - center_y;
	return (dx * dx + dy * dy <= radius * radius);
}

// Rotate point around origin by given angle (counter-clockwise)
static vertex_t	rotate_point(vertex_t point, float angle)
{
	vertex_t	rotated;
	float		cos_a = cosf(angle);
	float		sin_a = sinf(angle);
	
	rotated.x = point.x * cos_a - point.y * sin_a;
	rotated.y = point.x * sin_a + point.y * cos_a;
	return (rotated);
}

// Clipped version of bresenham that only draws within the circle
static void	bresenham_clipped(vertex_t *start, vertex_t *end, mlx_image_t *img, int color)
{
	int	center_x = MINIMAP_WIDTH / 2;
	int	center_y = MINIMAP_HEIGHT / 2;
	int	radius = (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE) / 2;
	
	// Use normal bresenham but check each pixel
	int x0 = (int)start->x, y0 = (int)start->y;
	int x1 = (int)end->x, y1 = (int)end->y;
	int dx = abs(x1 - x0), dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;
	
	while (true)
	{
		// Only draw pixel if it's inside the circle and image bounds
		if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height &&
			is_inside_minimap_circle(x0, y0, center_x, center_y, radius))
		{
			save_pixel_to_image(img, x0, y0, color);
		}
		
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}
}

void	render_minimap_bg(t_minimap *minimap)
{
	unsigned int	x;
	unsigned int	y;
	int			center_x;
	int			center_y;
	int			radius;

	center_x = MINIMAP_WIDTH / 2;
	center_y = MINIMAP_HEIGHT / 2;
	radius = (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE) / 2;
	
	// Clear background to transparent first
	memset(minimap->bg->pixels, 0, minimap->bg->width * minimap->bg->height * 4);
	
	// Only render blue pixels inside the circle
	y = 0;
	while (y < minimap->bg->height)
	{
		x = 0;
		while (x < minimap->bg->width)
		{
			if (is_inside_minimap_circle(x, y, center_x, center_y, radius))
				save_pixel_to_image(minimap->bg, x, y, BLUE);
			// Leave pixels outside circle transparent (don't set them)
			x++;
		}
		y++;
	}
}

void	render_minimap_tile(mlx_image_t *tile, int tile_size, int color)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < (unsigned int)tile_size)
	{
		x = 0;
		while (x < (unsigned int)tile_size)
		{
			save_pixel_to_image(tile, x, y, color);
			x++;
		}
		y++;
	}
}

void	render_minimap_tiles(t_map *map, t_minimap *minimap)
{
	int		pixel_x, pixel_y;
	int		center_x, center_y, radius;
	vertex_t	world_offset, rotated_offset, screen_pos;
	float		player_angle;
	size_t		row_len;

	// Render blue background first
	render_minimap_bg(minimap);
	
	center_x = MINIMAP_WIDTH / 2;
	center_y = MINIMAP_HEIGHT / 2;
	radius = (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE) / 2;
	player_angle = minimap->player->angle + M_PI / 2; // +90° so player points up

	// Render tiles by sampling world space in a rotated grid
	for (int world_y = minimap->player->y - MINIMAP_RADIUS; world_y <= minimap->player->y + MINIMAP_RADIUS; world_y++)
	{
		if (world_y < 0 || world_y >= map->height || !map->grid[world_y])
			continue;
		
		row_len = strlen(map->grid[world_y]);
		for (int world_x = minimap->player->x - MINIMAP_RADIUS; world_x <= minimap->player->x + MINIMAP_RADIUS; world_x++)
		{
			if (world_x < 0 || (size_t)world_x >= row_len || map->grid[world_y][world_x] != '1')
				continue;
			
			// Calculate offset from tile center to tile center
			world_offset.x = (world_x - minimap->player->x) * MINIMAP_TILE_SIZE;
			world_offset.y = (world_y - minimap->player->y) * MINIMAP_TILE_SIZE;
			
			// Rotate the offset so player always faces up
			rotated_offset = rotate_point(world_offset, -player_angle);
			
			// Convert to screen position (subtract half tile size to center the tile)
			screen_pos.x = center_x + rotated_offset.x - MINIMAP_TILE_SIZE / 2;
			screen_pos.y = center_y + rotated_offset.y - MINIMAP_TILE_SIZE / 2;
			
			// Draw the tile (as a filled rectangle) if it's inside the circle
			for (int tile_y = 0; tile_y < MINIMAP_TILE_SIZE; tile_y++)
			{
				for (int tile_x = 0; tile_x < MINIMAP_TILE_SIZE; tile_x++)
				{
					pixel_x = (int)(screen_pos.x + tile_x);
					pixel_y = (int)(screen_pos.y + tile_y);
					
					if (pixel_x >= 0 && pixel_x < (int)minimap->bg->width &&
						pixel_y >= 0 && pixel_y < (int)minimap->bg->height &&
						is_inside_minimap_circle(pixel_x, pixel_y, center_x, center_y, radius))
					{
						save_pixel_to_image(minimap->bg, pixel_x, pixel_y, WHITE);
					}
				}
			}
		}
	}
}

void	render_minimap_player(t_minimap *minimap)
{
	render_minimap_tile(minimap->player_sprite, MINIMAP_PLAYER_SIZE, YELLOW);
	// Image already positioned in minimap_init
}

// Convert world coordinates to rotated minimap coordinates
static vertex_t	world_to_minimap_vertex(t_minimap *minimap, vertex_t world)
{
	vertex_t	player_position;
	vertex_t	world_offset;
	vertex_t	rotated_offset;
	vertex_t	minimap_vertex;
	float		player_angle;
	float		scale;

	player_position.x = (minimap->player->x + 0.5f) * WORLDMAP_TILE_SIZE;
	player_position.y = (minimap->player->y + 0.5f) * WORLDMAP_TILE_SIZE;
	
	// Calculate offset from player in world coordinates
	world_offset.x = world.x - player_position.x;
	world_offset.y = world.y - player_position.y;
	
	// Apply rotation so player faces up
	player_angle = minimap->player->angle + M_PI / 2;
	rotated_offset = rotate_point(world_offset, -player_angle);
	
	// Scale and center on minimap
	scale = (float)MINIMAP_TILE_SIZE / WORLDMAP_TILE_SIZE;
	minimap_vertex.x = (MINIMAP_WIDTH / 2) + rotated_offset.x * scale;
	minimap_vertex.y = (MINIMAP_HEIGHT / 2) + rotated_offset.y * scale;
	
	return (minimap_vertex);
}

// Helper function to fill a triangle using scanline algorithm
static void	fill_triangle_scanline(mlx_image_t *img, vertex_t v1, vertex_t v2, vertex_t v3, int color)
{
	int	center_x = MINIMAP_WIDTH / 2;
	int	center_y = MINIMAP_HEIGHT / 2;
	int	radius = (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE) / 2;
	
	// Sort vertices by y coordinate (v1.y <= v2.y <= v3.y)
	if (v1.y > v2.y) { vertex_t temp = v1; v1 = v2; v2 = temp; }
	if (v2.y > v3.y) { vertex_t temp = v2; v2 = v3; v3 = temp; }
	if (v1.y > v2.y) { vertex_t temp = v1; v1 = v2; v2 = temp; }
	
	// Fill triangle using scanline algorithm
	for (int y = (int)v1.y; y <= (int)v3.y; y++)
	{
		if (y < 0 || y >= (int)img->height)
			continue;
		
		float x_left, x_right;
		
		// Calculate intersection points for this scanline
		if (v3.y != v1.y)
		{
			float t = (y - v1.y) / (v3.y - v1.y);
			x_right = v1.x + t * (v3.x - v1.x);
		}
		else
			x_right = v1.x;
		
		if (y <= v2.y)
		{
			if (v2.y != v1.y)
			{
				float t = (y - v1.y) / (v2.y - v1.y);
				x_left = v1.x + t * (v2.x - v1.x);
			}
			else
				x_left = v1.x;
		}
		else
		{
			if (v3.y != v2.y)
			{
				float t = (y - v2.y) / (v3.y - v2.y);
				x_left = v2.x + t * (v3.x - v2.x);
			}
			else
				x_left = v2.x;
		}
		
		// Ensure left <= right
		if (x_left > x_right)
		{
			float temp = x_left; x_left = x_right; x_right = temp;
		}
		
		// Fill the scanline
		for (int x = (int)x_left; x <= (int)x_right; x++)
		{
			if (x >= 0 && x < (int)img->width &&
				is_inside_minimap_circle(x, y, center_x, center_y, radius))
			{
				save_pixel_to_image(img, x, y, color);
			}
		}
	}
}

void	render_minimap_player_vision(t_minimap *minimap)
{
	const float MAX_DIST = 2000.0f;
	t_rayhit	rayhit;
	vertex_t	player_position;
	vertex_t	tip;
	vertex_t	player_to_minimap;
	int			i;
	float		ray_angle;
	float		angle_start;
	float		increment;
	int			num_rays;
	vertex_t	ray_endpoints[32]; // Store ray endpoints for triangle fill

	// Clear player vision buffer
	memset(minimap->player_vision->pixels, 0, minimap->player_vision->width * minimap->player_vision->height * 4);

	player_position.x = (minimap->player->x + 0.5f) * WORLDMAP_TILE_SIZE;
	player_position.y = (minimap->player->y + 0.5f) * WORLDMAP_TILE_SIZE;
	player_to_minimap = world_to_minimap_vertex(minimap, player_position);

	// Cast rays and store endpoints
	i = 0;
	num_rays = MINIMAP_RAYS / 8;
	angle_start = minimap->player->angle - 0.5f * PLAYER_FOV;
	increment = PLAYER_FOV / (float)(num_rays - 1);
	while (i < num_rays)
	{
		ray_angle = normalize_angle(angle_start + i * increment);
		rayhit = raycast_world(minimap->map, player_position, ray_angle, MAX_DIST);
		ray_endpoints[i] = world_to_minimap_vertex(minimap, rayhit.position);
		i++;
	}
	
	// Fill triangles between adjacent rays
	for (i = 0; i < num_rays - 1; i++)
	{
		fill_triangle_scanline(minimap->player_vision, player_to_minimap, 
						   ray_endpoints[i], ray_endpoints[i + 1], GREEN);
	}
	
	// Draw ray outlines for better visibility (optional)
	for (i = 0; i < num_rays; i++)
	{
		bresenham_clipped(&player_to_minimap, &ray_endpoints[i], minimap->player_vision, GREEN);
	}

	// Simple player direction indicator (always points up in rotated space)
	tip.x = MINIMAP_WIDTH / 2;
	tip.y = MINIMAP_HEIGHT / 2 - 30; // Points straight up
	bresenham_clipped(&player_to_minimap, &tip, minimap->player_vision, RED);
}

void	minimap_free(mlx_t *mlx, t_minimap *minimap)
{
	mlx_delete_image(mlx, minimap->tile);
	mlx_delete_image(mlx, minimap->player_vision);
	mlx_delete_image(mlx, minimap->player_sprite);
	mlx_delete_image(mlx, minimap->bg);
}
