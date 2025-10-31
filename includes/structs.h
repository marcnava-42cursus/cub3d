/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:42:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/07 19:51:55 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "MLX42/MLX42.h"
# include "MLX42/MLX42_Int.h"

# define MAX_LINE_LEN 1024
# define MAX_PATH_LEN 512


// Orientaciones del jugador
typedef enum e_orientation
{
	NORTH = 'N',
	SOUTH = 'S',
	EAST = 'E',
	WEST = 'W'
}	t_orientation;

// Render order for gameplay buffers
typedef enum e_render_order
{
	CURRENT = 0,
	NEXT = 1
}	t_render_order;

// Estructura para colores RGB
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

// Estructura para texturas
typedef struct s_textures
{
	char		*north_path;	// NO - file path
	char		*south_path;	// SO - file path
	char		*west_path;		// WE - file path
	char		*east_path;		// EA - file path
	xpm_t		*north;			// Loaded north texture
	xpm_t		*south;			// Loaded south texture
	xpm_t		*west;			// Loaded west texture
	xpm_t		*east;			// Loaded east texture
}	t_textures;

// Estructura para la posición del jugador
typedef struct s_player
{
	float			x;            // Coordenada X flotante para movimiento suave
	float			y;            // Coordenada Y flotante para movimiento suave
	float			angle;        // Ángulo de rotación en radianes
	t_orientation	orientation;  // Orientación original del mapa (N, S, E, W)
}	t_player;

// Estructura para el mapa
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

// Estructura principal que contiene todos los datos parseados
typedef struct s_cub_data
{
	t_textures	textures;
	t_color		floor_color;
	t_color		ceiling_color;
	t_map		map;
	t_player	player;
}	t_cub_data;

// Textures for 2D map rendering
typedef struct s_map_textures
{
	mlx_image_t	*wall;          // block.png
	mlx_image_t	*floor;         // floor.png
	mlx_image_t	*player;        // player.png (fallback)
	mlx_image_t	*player_north;  // player facing north
	mlx_image_t	*player_south;  // player facing south
	mlx_image_t	*player_east;   // player facing east
	mlx_image_t	*player_west;   // player facing west
}	t_map_textures;

typedef struct s_minimap
{
	mlx_image_t		*bg;
	mlx_image_t		*player_sprite;
	mlx_image_t		*player_vision;
	mlx_image_t		*tile;
	t_map			*map;
	t_player		*player;
}	t_minimap;

typedef struct s_rayhit
{
	bool			hit;
	int				cell_x;
	int				cell_y;
	int				side;      // 0 = vertical 1 = horizontal
	t_orientation	face;
	vertex_t		position;  // in world pixels
	float			distance;
}	t_rayhit;

// Estructura principal del juego que contiene TODOS los datos
typedef struct s_game
{
    t_cub_data	cub_data;		// Datos parseados del archivo .cub
    // Ventana MLX
    mlx_t		*mlx;
	// Buffers de renderizado 2D/legacy
	mlx_image_t	*rc_buf_zero;
	mlx_image_t	*rc_buf_one;
	mlx_image_t	*bg_buf_zero;
	// Capas separadas para el renderizado 2D
	mlx_image_t	*map_layer;		// Capa estática del mapa
	mlx_image_t	*player_layer;		// Capa dinámica del jugador
	// Texturas 2D
	t_map_textures	textures_2d;
	// Variables para seguimiento de estado del jugador
	float		last_player_x;
	float		last_player_y;
	float		last_player_angle;
	int			last_grid_x;
	int			last_grid_y;
	// Timing y parámetros de movimiento
	double		last_frame_time;
	double		delta_time;
	float		move_speed;
	float		rot_speed;
	float		player_radius;
	bool		bg_layer_attached;
	bool		map_layer_attached;
	bool		player_layer_attached;
	// Estado de teclas para movimiento continuo
	bool		key_w_pressed;
	bool		key_s_pressed;
	bool		key_a_pressed;
	bool		key_d_pressed;
	bool		key_left_pressed;
    bool		key_right_pressed;
	// Variables de control de mouse
	double		last_mouse_x;
	bool		mouse_initialized;
	float		mouse_sensitivity;

    // Datos de renderizado (raycast, sprites, etc.)
    mlx_image_t	*double_buffer[2];
    float		resolution_scale;
    t_minimap	minimap;

    // Estado de UI
    bool        map_2d_visible;
}	t_game;

#endif
