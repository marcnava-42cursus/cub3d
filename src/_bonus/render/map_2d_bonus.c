/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_2d_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:36:00 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 22:18:35 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"
#include "logic_bonus.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "libft.h"

/*
// Modal constants matching config_modal_bonus.c
# define MAP_MODAL_BG_COLOR 0x000000C0
# define MAP_MODAL_PANEL_COLOR 0x1E1E1ED0
# define MAP_MODAL_BORDER_COLOR 0xCCCCCCFF
# define MAP_MODAL_BORDER_THICKNESS 2
# define ORB_MINIMAP_COLOR 0xFF00FFFF

// Forward declarations
static void	create_directional_sprites(t_game *game);
static void	create_directional_sprite(mlx_image_t *img, uint32_t color, int direction);
static void	draw_arrow_on_sprite(mlx_image_t *img, int direction, uint32_t arrow_color);

// --- Drawing Helpers ---

static void	map_draw_rect(mlx_image_t *img, int x, int y, int w, int h,
		uint32_t color)
{
	int		yy;
	int		xx;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
	uint8_t	*row;

	if (!img || w <= 0 || h <= 0)
		return ;
	if (x < 0)
	{
		w += x;
		x = 0;
	}
	if (y < 0)
	{
		h += y;
		y = 0;
	}
	if (x >= (int)img->width || y >= (int)img->height)
		return ;
	if (x + w > (int)img->width)
		w = (int)img->width - x;
	if (y + h > (int)img->height)
		h = (int)img->height - y;
	if (w <= 0 || h <= 0)
		return ;
	r = (uint8_t)(color >> 24);
	g = (uint8_t)(color >> 16);
	b = (uint8_t)(color >> 8);
	a = (uint8_t)(color & 0xFF);
	yy = 0;
	while (yy < h)
	{
		row = &img->pixels[((y + yy) * img->width + x) * 4];
		xx = 0;
		while (xx < w)
		{
			row[0] = r;
			row[1] = g;
			row[2] = b;
			row[3] = a;
			row += 4;
			xx++;
		}
		yy++;
	}
}

static void	map_draw_border(mlx_image_t *img, int x, int y, int w, int h,
		int thickness, uint32_t color)
{
	if (thickness <= 0)
		return ;
	map_draw_rect(img, x, y, w, thickness, color);
	map_draw_rect(img, x, y + h - thickness, w, thickness, color);
	map_draw_rect(img, x, y, thickness, h, color);
	map_draw_rect(img, x + w - thickness, y, thickness, h, color);
}

// --- Directional Sprites ---

static void	create_directional_sprites(t_game *game)
{
	uint32_t	size = TILE_SIZE;
	
	if (!game->textures_2d.player_north)
	{
		game->textures_2d.player_north = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_north, 0xFF00FFFF, 0);
	}
	if (!game->textures_2d.player_south)
	{
		game->textures_2d.player_south = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_south, 0x00FFFFFF, 1);
	}
	if (!game->textures_2d.player_east)
	{
		game->textures_2d.player_east = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_east, 0xFFFF00FF, 2);
	}
	if (!game->textures_2d.player_west)
	{
		game->textures_2d.player_west = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_west, 0xFF8000FF, 3);
	}
}

static void	create_directional_sprite(mlx_image_t *img, uint32_t color, int direction)
{
	int		x, y;
	int		size = img->width;
	int		center = size / 2;
	
	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
			mlx_put_pixel(img, x, y, 0x00000000);
	}
	
	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
		{
			int dx = x - center;
			int dy = y - center;
			if (dx * dx + dy * dy <= (size/3) * (size/3))
				mlx_put_pixel(img, x, y, color);
		}
	}
	draw_arrow_on_sprite(img, direction, 0x000000FF);
}

static void	draw_arrow_on_sprite(mlx_image_t *img, int direction, uint32_t arrow_color)
{
	int		size = img->width;
	int		center = size / 2;
	int		arrow_size = size / 4;
	
	for (int i = 0; i < arrow_size; i++)
	{
		switch (direction)
		{
			case 0:
				mlx_put_pixel(img, center, center - i, arrow_color);
				if (i > arrow_size/2)
				{
					mlx_put_pixel(img, center - (i - arrow_size/2), center - i, arrow_color);
					mlx_put_pixel(img, center + (i - arrow_size/2), center - i, arrow_color);
				}
				break;
			case 1:
				mlx_put_pixel(img, center, center + i, arrow_color);
				if (i > arrow_size/2)
				{
					mlx_put_pixel(img, center - (i - arrow_size/2), center + i, arrow_color);
					mlx_put_pixel(img, center + (i - arrow_size/2), center + i, arrow_color);
				}
				break;
			case 2:
				mlx_put_pixel(img, center + i, center, arrow_color);
				if (i > arrow_size/2)
				{
					mlx_put_pixel(img, center + i, center - (i - arrow_size/2), arrow_color);
					mlx_put_pixel(img, center + i, center + (i - arrow_size/2), arrow_color);
				}
				break;
			case 3:
				mlx_put_pixel(img, center - i, center, arrow_color);
				if (i > arrow_size/2)
				{
					mlx_put_pixel(img, center - i, center - (i - arrow_size/2), arrow_color);
					mlx_put_pixel(img, center - i, center + (i - arrow_size/2), arrow_color);
				}
				break;
		}
	}
}

// --- Texture Loading ---

static int	load_texture(t_game *game, const char *path, mlx_image_t **texture)
{
	mlx_texture_t		*tex;

	tex = mlx_load_png(path);
	if (!tex)
		return (0);
	*texture = mlx_texture_to_image(game->mlx, tex);
	mlx_delete_texture(tex);
	if (!*texture)
		return (0);
	return (1);
}

int	load_map_textures_bonus(t_game *game)
{
	if (!load_texture(game, "assets/img/block.png", &game->textures_2d.wall))
	{
		printf("Error: Could not load wall texture\n");
		return (0);
	}
	if (!load_texture(game, "assets/img/floor.png", &game->textures_2d.floor))
	{
		printf("Error: Could not load floor texture\n");
		return (0);
	}
	
	if (!load_texture(game, "assets/img/player.png", &game->textures_2d.player))
	{
		printf("Error: Could not load player texture\n");
		return (0);
	}
	
	game->textures_2d.player_north = NULL;
	game->textures_2d.player_south = NULL;
	game->textures_2d.player_east = NULL;
	game->textures_2d.player_west = NULL;
	
	load_texture(game, "assets/img/player_north.png", &game->textures_2d.player_north);
	load_texture(game, "assets/img/player_south.png", &game->textures_2d.player_south);
	load_texture(game, "assets/img/player_east.png", &game->textures_2d.player_east);
	load_texture(game, "assets/img/player_west.png", &game->textures_2d.player_west);
	
	printf("Loaded directional sprites: N=%s S=%s E=%s W=%s\n",
		game->textures_2d.player_north ? "Yes" : "No",
		game->textures_2d.player_south ? "Yes" : "No",
		game->textures_2d.player_east ? "Yes" : "No",
		game->textures_2d.player_west ? "Yes" : "No");
	
	if (!game->textures_2d.player_north || !game->textures_2d.player_south ||
		!game->textures_2d.player_east || !game->textures_2d.player_west)
	{
		printf("Creating programmatic directional sprites...\n");
		create_directional_sprites(game);
	}
	
	return (1);
}

void	free_map_textures_bonus(t_game *game)
{
	if (game->textures_2d.wall)
		mlx_delete_image(game->mlx, game->textures_2d.wall);
	if (game->textures_2d.floor)
		mlx_delete_image(game->mlx, game->textures_2d.floor);
	if (game->textures_2d.player)
		mlx_delete_image(game->mlx, game->textures_2d.player);
	if (game->textures_2d.player_north)
		mlx_delete_image(game->mlx, game->textures_2d.player_north);
	if (game->textures_2d.player_south)
		mlx_delete_image(game->mlx, game->textures_2d.player_south);
	if (game->textures_2d.player_east)
		mlx_delete_image(game->mlx, game->textures_2d.player_east);
	if (game->textures_2d.player_west)
		mlx_delete_image(game->mlx, game->textures_2d.player_west);
}

// --- Rendering ---

static void	get_map_panel_bounds(int *x, int *y, int *w, int *h)
{
	int	panel_w = (int)(MAX_WINDOW_WIDTH * 0.70f);
	int	panel_h = (int)(MAX_WINDOW_HEIGHT * 0.70f);
	
	*x = (MAX_WINDOW_WIDTH - panel_w) / 2;
	*y = (MAX_WINDOW_HEIGHT - panel_h) / 2;
	*w = panel_w;
	*h = panel_h;
}

static void	get_map_offset(t_game *game, int *offset_x, int *offset_y)
{
	int	panel_x, panel_y, panel_w, panel_h;
	int	map_w_pixels, map_h_pixels;

	get_map_panel_bounds(&panel_x, &panel_y, &panel_w, &panel_h);
	map_w_pixels = game->cub_data.map.width * TILE_SIZE;
	map_h_pixels = game->cub_data.map.height * TILE_SIZE;

	// Center the map in the panel
	*offset_x = panel_x + (panel_w - map_w_pixels) / 2;
	*offset_y = panel_y + (panel_h - map_h_pixels) / 2;
}

static void	copy_texture_to_layer(mlx_image_t *layer, mlx_image_t *texture, int x, int y)
{
	int		tex_x, tex_y;
	uint32_t	pixel;
	int		layer_x, layer_y;
	int		panel_x, panel_y, panel_w, panel_h;

	// Clipping against modal panel
	get_map_panel_bounds(&panel_x, &panel_y, &panel_w, &panel_h);

	for (tex_y = 0; tex_y < (int)texture->height; tex_y++)
	{
		for (tex_x = 0; tex_x < (int)texture->width; tex_x++)
		{
			layer_x = x + tex_x;
			layer_y = y + tex_y;

			// Basic bounds checking against image
			if (layer_x < 0 || layer_x >= (int)layer->width ||
				layer_y < 0 || layer_y >= (int)layer->height)
				continue;

			// Clipping against panel area to keep map inside
			if (layer_x < panel_x || layer_x >= panel_x + panel_w ||
				layer_y < panel_y || layer_y >= panel_y + panel_h)
				continue;

			pixel = ((uint32_t*)texture->pixels)[tex_y * texture->width + tex_x];
			if (pixel != 0) // Simple transparency check if alpha is 0
				((uint32_t*)layer->pixels)[layer_y * layer->width + layer_x] = pixel;
		}
	}
}

static void	draw_tile_to_layer(t_game *game, int x, int y, mlx_image_t *texture)
{
	int	screen_x;
	int	screen_y;
	int	offset_x, offset_y;

	get_map_offset(game, &offset_x, &offset_y);
	screen_x = offset_x + x * TILE_SIZE;
	screen_y = offset_y + y * TILE_SIZE;
	copy_texture_to_layer(game->map_layer, texture, screen_x, screen_y);
}

static void	render_map_modal_background(t_game *game)
{
	int	panel_x, panel_y, panel_w, panel_h;
	
	// Draw full screen semi-transparent background
	map_draw_rect(game->map_layer, 0, 0, MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT, MAP_MODAL_BG_COLOR);

	// Draw panel
	get_map_panel_bounds(&panel_x, &panel_y, &panel_w, &panel_h);
	map_draw_rect(game->map_layer, panel_x, panel_y, panel_w, panel_h, MAP_MODAL_PANEL_COLOR);
	map_draw_border(game->map_layer, panel_x, panel_y, panel_w, panel_h, MAP_MODAL_BORDER_THICKNESS, MAP_MODAL_BORDER_COLOR);
}

static void	render_map_tiles(t_game *game)
{
	int		y;
	int		x;
	char	tile;

	// First, draw the modal background
	render_map_modal_background(game);

	// Then draw the tiles
	y = 0;
	while (y < game->cub_data.map.height)
	{
		x = 0;
		while (x < game->cub_data.map.width)
		{
			tile = game->cub_data.map.grid[y][x];
			if (tile == '1' || tile == '2')
				draw_tile_to_layer(game, x, y, game->textures_2d.wall);
			else if (tile == '0' || tile == 'N' || tile == 'S' 
				 || tile == 'E' || tile == 'W')
				draw_tile_to_layer(game, x, y, game->textures_2d.floor);
			x++;
		}
		y++;
	}
}

static void	clear_player_layer(t_game *game)
{
	unsigned int	total_pixels;
	uint32_t		*pixels;
	unsigned int	i;

	total_pixels = game->player_layer->width * game->player_layer->height;
	pixels = (uint32_t*)game->player_layer->pixels;
	for (i = 0; i < total_pixels; i++)
		pixels[i] = 0x00000000;
}

static void	draw_rotated_player(t_game *game, int dst_x, int dst_y)
{
	mlx_image_t		*texture;
	mlx_image_t		*layer;
	float			angle;
	float			cos_a;
	float			sin_a;
	int			center_x;
	int			center_y;
	int			tex_width;
	int			tex_height;
	int			layer_w;
	int			layer_h;
	uint8_t		*src_pixels;
	uint32_t	*dst_pixels;
	int			x;
	int			y;
	int			panel_x, panel_y, panel_w, panel_h;

	texture = game->textures_2d.player;
	layer = game->player_layer;
	if (!texture || !layer)
		return ;
	
	get_map_panel_bounds(&panel_x, &panel_y, &panel_w, &panel_h);

	angle = game->cub_data.player.angle + (float)FT_PI_2;
	cos_a = cosf(angle);
	sin_a = sinf(angle);
	tex_width = (int)texture->width;
	tex_height = (int)texture->height;
	layer_w = (int)layer->width;
	layer_h = (int)layer->height;
	src_pixels = texture->pixels;
	dst_pixels = (uint32_t *)layer->pixels;
	center_x = tex_width / 2;
	center_y = tex_height / 2;
	y = 0;
	while (y < tex_height)
	{
		int dest_y = dst_y + y;
		if (dest_y < 0 || dest_y >= layer_h)
		{
			y++;
			continue ;
		}
		x = 0;
		while (x < tex_width)
		{
			int dest_x = dst_x + x;
			float rel_x;
			float rel_y;
			float src_rel_x;
			float src_rel_y;
			float src_xf;
			float src_yf;
			int src_x;
			int src_y;
			uint8_t *src_px;
			uint32_t color;

			if (dest_x < 0 || dest_x >= layer_w)
			{
				x++;
				continue ;
			}
			
			// Clipping against panel area
			if (dest_x < panel_x || dest_x >= panel_x + panel_w ||
						dest_y < panel_y || dest_y >= panel_y + panel_h)
			{
				x++;
				continue;
			}

			rel_x = (float)x - (float)center_x;
			rel_y = (float)y - (float)center_y;
			src_rel_x = rel_x * cos_a + rel_y * sin_a;
			src_rel_y = -rel_x * sin_a + rel_y * cos_a;
			src_xf = src_rel_x + (float)center_x;
			src_yf = src_rel_y + (float)center_y;
			src_x = (int)roundf(src_xf);
			src_y = (int)roundf(src_yf);
			if (src_x < 0 || src_x >= tex_width
				|| src_y < 0 || src_y >= tex_height)
			{
				x++;
				continue ;
			}
			src_px = &src_pixels[(src_y * tex_width + src_x) * 4];
			if (src_px[3] == 0)
			{
				x++;
				continue ;
			}
			ft_memcpy(&color, src_px, sizeof(uint32_t));
			dst_pixels[dest_y * layer_w + dest_x] = color;
			x++;
		}
		y++;
	}
}

static void	draw_orb_minimap(t_game *game)
{
	int	offset_x;
	int	offset_y;
	int	orb_size;
	int	screen_x;
	int	screen_y;

	if (!game || !game->orb.active)
		return ;
	get_map_offset(game, &offset_x, &offset_y);
	orb_size = TILE_SIZE / 6;
	if (orb_size < 4)
		orb_size = 4;
	screen_x = offset_x + (int)roundf(game->orb.x * TILE_SIZE) - orb_size / 2;
	screen_y = offset_y + (int)roundf(game->orb.y * TILE_SIZE) - orb_size / 2;
	map_draw_rect(game->player_layer, screen_x, screen_y,
		orb_size, orb_size, ORB_MINIMAP_COLOR);
}

static void	render_player(t_game *game)
{
	int	player_x;
	int	player_y;
	int	offset_x, offset_y;

	clear_player_layer(game);

	get_map_offset(game, &offset_x, &offset_y);
	
	// Convertir coordenadas flotantes a posición en pantalla + offset
	player_x = offset_x + (int)(game->cub_data.player.x * TILE_SIZE) - TILE_SIZE/2;
	player_y = offset_y + (int)(game->cub_data.player.y * TILE_SIZE) - TILE_SIZE/2;

	draw_rotated_player(game, player_x, player_y);
	draw_orb_minimap(game);
}

void	render_map_2d_bonus(t_game *game)
{
	render_map_tiles(game);
	render_player(game);
	orb_projectile_mark_drawn(game);
}

void	render_map_2d_initial_advanced(t_game *game)
{
	render_map_tiles(game);
	render_player(game);
	orb_projectile_mark_drawn(game);
}

void	render_map_tiles_static_bonus(t_game *game)
{
	render_map_tiles(game);
}

void	render_player_dynamic_advanced(t_game *game)
{
	bool	player_changed;
	bool	orb_changed;

	player_changed = (game->cub_data.player.x != game->last_player_x ||
			game->cub_data.player.y != game->last_player_y ||
			game->cub_data.player.angle != game->last_player_angle);
	orb_changed = orb_projectile_needs_redraw(game);
	if (player_changed || orb_changed)
	{
		render_player(game);
		
		game->last_player_x = game->cub_data.player.x;
		game->last_player_y = game->cub_data.player.y;
		game->last_player_angle = game->cub_data.player.angle;
		orb_projectile_mark_drawn(game);
	}
}
*/