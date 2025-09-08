/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_2d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agentmode <agentmode@warp.ai>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:36:00 by agentmode         #+#    #+#             */
/*   Updated: 2025/09/06 17:36:00 by agentmode        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <math.h>

// Forward declarations
static void	create_directional_sprites(t_game *game);
static void	create_directional_sprite(mlx_image_t *img, uint32_t color, int direction);
static void	draw_arrow_on_sprite(mlx_image_t *img, int direction, uint32_t arrow_color);

// Función para crear sprites direccionales programáticamente
static void	create_directional_sprites(t_game *game)
{
	uint32_t	size = TILE_SIZE;
	
	// Crear sprites direccionales con diferentes colores/formas
	if (!game->textures_2d.player_north)
	{
		game->textures_2d.player_north = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_north, 0xFF00FFFF, 0); // Magenta, flecha arriba
	}
	if (!game->textures_2d.player_south)
	{
		game->textures_2d.player_south = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_south, 0x00FFFFFF, 1); // Cian, flecha abajo
	}
	if (!game->textures_2d.player_east)
	{
		game->textures_2d.player_east = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_east, 0xFFFF00FF, 2); // Amarillo, flecha derecha
	}
	if (!game->textures_2d.player_west)
	{
		game->textures_2d.player_west = mlx_new_image(game->mlx, size, size);
		create_directional_sprite(game->textures_2d.player_west, 0xFF8000FF, 3); // Naranja, flecha izquierda
	}
}

// Función para crear un sprite direccional con una flecha
static void	create_directional_sprite(mlx_image_t *img, uint32_t color, int direction)
{
	int	x, y;
	int	size = img->width;
	int	center = size / 2;
	
	// Limpiar la imagen (transparente)
	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
			mlx_put_pixel(img, x, y, 0x00000000);
	}
	
	// Dibujar círculo base
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
	
	// Dibujar flecha según la dirección
	draw_arrow_on_sprite(img, direction, 0x000000FF); // Flecha negra
}

// Función para dibujar una flecha en el sprite
static void	draw_arrow_on_sprite(mlx_image_t *img, int direction, uint32_t arrow_color)
{
	int	size = img->width;
	int	center = size / 2;
	int	arrow_size = size / 4;
	
	// Dibujar línea principal de la flecha
	for (int i = 0; i < arrow_size; i++)
	{
		switch (direction)
		{
			case 0: // Norte (arriba)
				mlx_put_pixel(img, center, center - i, arrow_color);
				if (i > arrow_size/2)
				{
					mlx_put_pixel(img, center - (i - arrow_size/2), center - i, arrow_color);
					mlx_put_pixel(img, center + (i - arrow_size/2), center - i, arrow_color);
				}
				break;
			case 1: // Sur (abajo)
				mlx_put_pixel(img, center, center + i, arrow_color);
				if (i > arrow_size/2)
				{
					mlx_put_pixel(img, center - (i - arrow_size/2), center + i, arrow_color);
					mlx_put_pixel(img, center + (i - arrow_size/2), center + i, arrow_color);
				}
				break;
			case 2: // Este (derecha)
				mlx_put_pixel(img, center + i, center, arrow_color);
				if (i > arrow_size/2)
				{
					mlx_put_pixel(img, center + i, center - (i - arrow_size/2), arrow_color);
					mlx_put_pixel(img, center + i, center + (i - arrow_size/2), arrow_color);
				}
				break;
			case 3: // Oeste (izquierda)
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

// Función para llenar una imagen con un color sólido
static void	fill_image_color(mlx_image_t *image, uint32_t color)
{
	uint32_t	x;
	uint32_t	y;
	
	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
		{
			mlx_put_pixel(image, x, y, color);
			x++;
		}
		y++;
	}
}

static int	load_texture(t_game *game, const char *path, mlx_image_t **texture)
{
	mlx_texture_t	*tex;

	tex = mlx_load_png(path);
	if (!tex)
		return (0);
	*texture = mlx_texture_to_image(game->mlx, tex);
	mlx_delete_texture(tex);
	if (!*texture)
		return (0);
	return (1);
}

int	load_map_textures(t_game *game)
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
	
	// Cargar sprite principal del jugador (fallback)
	if (!load_texture(game, "assets/img/player.png", &game->textures_2d.player))
	{
		printf("Error: Could not load player texture\n");
		return (0);
	}
	
	// Intentar cargar sprites direccionales (opcional)
	game->textures_2d.player_north = NULL;
	game->textures_2d.player_south = NULL;
	game->textures_2d.player_east = NULL;
	game->textures_2d.player_west = NULL;
	
	// Si existen sprites direccionales, cargarlos
	load_texture(game, "assets/img/player_north.png", &game->textures_2d.player_north);
	load_texture(game, "assets/img/player_south.png", &game->textures_2d.player_south);
	load_texture(game, "assets/img/player_east.png", &game->textures_2d.player_east);
	load_texture(game, "assets/img/player_west.png", &game->textures_2d.player_west);
	
	printf("Loaded directional sprites: N=%s S=%s E=%s W=%s\n",
		game->textures_2d.player_north ? "Yes" : "No",
		game->textures_2d.player_south ? "Yes" : "No",
		game->textures_2d.player_east ? "Yes" : "No",
		game->textures_2d.player_west ? "Yes" : "No");
	
	// Si no se pudieron cargar sprites direccionales, crearlos programáticamente
	if (!game->textures_2d.player_north || !game->textures_2d.player_south ||
		!game->textures_2d.player_east || !game->textures_2d.player_west)
	{
		printf("Creating programmatic directional sprites...\n");
		create_directional_sprites(game);
	}
	
	return (1);
}

void	free_map_textures(t_game *game)
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

// Función para copiar una textura a una capa específica
static void	copy_texture_to_layer(mlx_image_t *layer, mlx_image_t *texture, int x, int y)
{
	int	tex_x, tex_y;
	uint32_t	pixel;
	int		layer_x, layer_y;

	for (tex_y = 0; tex_y < (int)texture->height && y + tex_y < (int)layer->height; tex_y++)
	{
		for (tex_x = 0; tex_x < (int)texture->width && x + tex_x < (int)layer->width; tex_x++)
		{
			layer_x = x + tex_x;
			layer_y = y + tex_y;
			pixel = ((uint32_t*)texture->pixels)[tex_y * texture->width + tex_x];
			((uint32_t*)layer->pixels)[layer_y * layer->width + layer_x] = pixel;
		}
	}
}

static void	draw_tile_to_layer(t_game *game, int x, int y, mlx_image_t *texture)
{
	int	screen_x;
	int	screen_y;

	screen_x = x * TILE_SIZE;
	screen_y = y * TILE_SIZE;
	copy_texture_to_layer(game->map_layer, texture, screen_x, screen_y);
}

static void	render_map_tiles(t_game *game)
{
	int		y;
	int		x;
	char	tile;

	y = 0;
	while (y < game->cub_data.map.height)
	{
		x = 0;
		while (x < game->cub_data.map.width)
		{
			tile = game->cub_data.map.grid[y][x];
			if (tile == '1')
				draw_tile_to_layer(game, x, y, game->textures_2d.wall);
			else if (tile == '0' || tile == 'N' || tile == 'S' 
				 || tile == 'E' || tile == 'W')
				draw_tile_to_layer(game, x, y, game->textures_2d.floor);
			x++;
		}
		y++;
	}
}

// Función para obtener el sprite correcto del jugador según su ángulo
static mlx_image_t	*get_player_sprite(t_game *game)
{
	float		angle = game->cub_data.player.angle;
	
	// Normalizar el ángulo para facilitar la comparación
	while (angle < 0)
		angle += 2 * M_PI;
	while (angle >= 2 * M_PI)
		angle -= 2 * M_PI;
	
	// Determinar dirección basado en el ángulo
	// Norte: -π/2 o 3π/2, Sur: π/2, Este: 0, Oeste: π
	if (angle >= 7 * M_PI / 4 || angle < M_PI / 4)
	{
		// Mirando hacia el Este (derecha)
		return (game->textures_2d.player_east ? game->textures_2d.player_east : game->textures_2d.player);
	}
	else if (angle >= M_PI / 4 && angle < 3 * M_PI / 4)
	{
		// Mirando hacia el Sur (abajo)
		return (game->textures_2d.player_south ? game->textures_2d.player_south : game->textures_2d.player);
	}
	else if (angle >= 3 * M_PI / 4 && angle < 5 * M_PI / 4)
	{
		// Mirando hacia el Oeste (izquierda)
		return (game->textures_2d.player_west ? game->textures_2d.player_west : game->textures_2d.player);
	}
	else
	{
		// Mirando hacia el Norte (arriba)
		return (game->textures_2d.player_north ? game->textures_2d.player_north : game->textures_2d.player);
	}
}

// Función para limpiar la capa del jugador
static void	clear_player_layer(t_game *game)
{
	unsigned int	total_pixels;
	uint32_t		*pixels;
	unsigned int	i;

	total_pixels = game->player_layer->width * game->player_layer->height;
	pixels = (uint32_t*)game->player_layer->pixels;
	for (i = 0; i < total_pixels; i++)
		pixels[i] = 0x00000000; // Transparente
}

static void	render_player(t_game *game)
{
	int			player_x;
	int			player_y;
	mlx_image_t	*player_sprite;

	// Limpiar la capa anterior del jugador
	clear_player_layer(game);

	// Convertir coordenadas flotantes a posición en pantalla
	player_x = (int)(game->cub_data.player.x * TILE_SIZE) - TILE_SIZE/2;
	player_y = (int)(game->cub_data.player.y * TILE_SIZE) - TILE_SIZE/2;

	// Obtener el sprite correcto según la orientación
	player_sprite = get_player_sprite(game);

	// Copiar el sprite del jugador a la capa del jugador
	copy_texture_to_layer(game->player_layer, player_sprite, player_x, player_y);
}

void	render_map_2d(t_game *game)
{
	// Renderizar mapa en su capa
	render_map_tiles(game);
	// Mostrar la capa del mapa en pantalla
	mlx_image_to_window(game->mlx, game->map_layer, 0, 0);
	
	// Renderizar jugador en su capa
	render_player(game);
	// Mostrar la capa del jugador encima
	mlx_image_to_window(game->mlx, game->player_layer, 0, 0);
}

// Función para renderizado inicial completo
void	render_map_2d_initial(t_game *game)
{
	// Renderizar todo el mapa por primera vez en su capa
	render_map_tiles(game);
	mlx_image_to_window(game->mlx, game->map_layer, 0, 0);
	
	// Renderizar jugador inicial en su capa
	render_player(game);
	mlx_image_to_window(game->mlx, game->player_layer, 0, 0);
}

// Renderizar solo los tiles del mapa (se llama una sola vez)
void	render_map_tiles_static(t_game *game)
{
	render_map_tiles(game);
	mlx_image_to_window(game->mlx, game->map_layer, 0, 0);
}

// Renderizar solo el jugador (se llama cada vez que se mueve)
void	render_player_dynamic(t_game *game)
{
	// Solo renderizar si la posición o ángulo han cambiado significativamente
	if (game->cub_data.player.x != game->last_player_x ||
		game->cub_data.player.y != game->last_player_y ||
		game->cub_data.player.angle != game->last_player_angle)
	{
		// Actualizar el jugador en su capa
		render_player(game);
		mlx_image_to_window(game->mlx, game->player_layer, 0, 0);
		
		// Actualizar variables de seguimiento
		game->last_player_x = game->cub_data.player.x;
		game->last_player_y = game->cub_data.player.y;
		game->last_player_angle = game->cub_data.player.angle;
	}
}
