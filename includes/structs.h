/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:42:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/18 12:44:47 by ivmirand         ###   ########.fr       */
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
	char	*north;		// NO
	char	*south;		// SO
	char	*west;		// WE
	char	*east;		// EA
}	t_textures;

// Estructura para la posición del jugador
typedef struct s_player
{
	int				x;
	int				y;
	float			angle;
	t_orientation	orientation;
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
	bool		hit;
	int			cell_x;
	int			cell_y;
	int			side;	//0 = vertical 1 = horizontal
	vertex_t	position; // in world pixels
	float		distance; 
}	t_rayhit;

// Estructura principal del juego que contiene TODOS los datos
typedef struct s_game
{
	t_cub_data	cub_data;		// Datos parseados del archivo .cub
	// Aquí se pueden añadir más campos según se desarrolle el juego:
	// - Ventana y gráficos (MLX, texturas cargadas, etc.)
	mlx_t		*mlx;
	// - Datos de renderizado (raycast, sprites, etc.)
	mlx_image_t	*bg_buf_zero;
	mlx_image_t	*atmos_buf_zero;
	mlx_image_t	*rc_buf_zero;
	mlx_image_t	*rc_buf_one;
	t_minimap	minimap;
	// - Estado del juego (paused, running, etc.)
	// - Input del jugador (teclas presionadas, mouse, etc.)
	// - Audio, animaciones, etc.
}	t_game;

#endif
