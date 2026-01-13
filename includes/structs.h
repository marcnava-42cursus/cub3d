/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:42:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/10 01:10:13 by marcnava         ###   ########.fr       */
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

// Forward declaration for custom texture entries
typedef struct s_custom_texture	t_custom_texture;

// Custom texture entry for bonus mode
struct s_custom_texture
{
	char				id[3];		// 2-char identifier (e.g., "DN", "dN")
	char				*path;		// Texture file path
	xpm_t				*texture;	// Loaded texture
	t_custom_texture	*next;		// Next in linked list
};

// Estructura para texturas
typedef struct s_textures
{
	char				*north_path;	// NO - file path
	char				*south_path;	// SO - file path
	char				*west_path;		// WE - file path
	char				*east_path;		// EA - file path
	xpm_t				*north;			// Loaded north texture
	xpm_t				*south;			// Loaded south texture
	xpm_t				*west;			// Loaded west texture
	xpm_t				*east;			// Loaded east texture
	t_custom_texture	*custom;		// List of custom textures (bonus)
}	t_textures;

//Player texture collection
typedef struct s_player_textures
{
	char	*left_hand_path;
	char	*left_thumb_path;
	char	*right_hand_path;
	char	*weapon_path;
	xpm_t	*left_hand;
	xpm_t	*left_thumb;
	xpm_t	*right_hand;
	xpm_t	*weapon;
}	t_player_textures;

// Estructura para la posición del jugador
typedef struct s_player
{
	float				x;
	float				y;
	float				angle;
	char				inventory;
	t_orientation		orientation;
	t_player_textures	textures;
}	t_player;

// Estructura para el mapa
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

// Floor node for bonus multi-level maps
typedef struct s_floor
{
	char			*path;			// File path for this floor
	t_map			map;			// Map data
	char			*up_path;		// Path to upper floor
	char			*down_path;		// Path to lower floor
	struct s_floor	*up;			// Pointer to upper floor node
	struct s_floor	*down;			// Pointer to lower floor node
	struct s_floor	*next;			// Singly-linked list chain (for cleanup)
	int				index;			// Relative index (0 = entry map)
	int				has_player;		// 1 if this floor contains the player
	int				parsed_neighbors;// 1 if neighbors already expanded
	t_player		player;			// Player data if present on this floor
	t_textures		textures;		// Textures for this floor
	t_color			floor_color;
	t_color			ceiling_color;
	bool			textures_loaded;
	int				elevator_count;	// Elevators present in this floor
	char			elevator_ids[16];
	int				elevator_x[16];
	int				elevator_y[16];
}	t_floor;

// Estructura principal que contiene todos los datos parseados
typedef struct s_cub_data
{
	t_textures	textures;
	t_color		floor_color;
	t_color		ceiling_color;
	char		*up_path;
	char		*down_path;
	t_map		map;
	t_player	player;
	t_floor		*floors;
	t_floor		*current_floor;
	int			floor_count;
	int			player_floor_index;
	char		*player_floor_path;
	int			elevator_id_count;
	char		elevator_ids[16];
	t_floor		*elevator_floor_a[16];
	t_floor		*elevator_floor_b[16];
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
	int				side;
	t_orientation	face;
	vertex_t		position;
	float			distance;
	int				wall_bounds[2];
}	t_rayhit;

# ifdef BONUS
typedef struct s_config_options
{
	bool	crosshair;
	bool	minimap;
	bool	fullscreen;
	int		selected;
}	t_config_options;
# endif

// Estructura principal del juego que contiene TODOS los datos
typedef struct s_game
{
	t_cub_data	cub_data;		// Datos parseados del archivo .cub
	// Ventana MLX
	mlx_t		*mlx;

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
	double		movement_lock_until;
	double		last_teleport_time;
	char		last_teleport_id;
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
	float		mouse_delta_accumulated;
	float		mouse_sensitivity;

	// Datos de renderizado (raycast, sprites, etc.)
	mlx_image_t	*double_buffer[2];
	float		resolution_scale;
	t_minimap	minimap;

	// Estado de UI
	bool		map_2d_visible;

# ifdef BONUS
	// Config modal UI (prototype)
	bool		config_modal_open;
	bool		config_modal_attached;
	int			config_current_tab; // 0 = Settings, 1 = Controls
	mlx_image_t	*config_modal;
	int32_t		config_quit_x;
	int32_t		config_quit_y;
	int32_t		config_quit_w;
	int32_t		config_quit_h;
	bool		config_quit_hover;
	double		config_quit_hold_time;
	mlx_image_t	*config_quit_label;
	mlx_image_t	*config_option_labels[3];
	int			config_panel_x;
	int			config_panel_y;
	int			config_panel_w;
	int			config_panel_h;
	t_config_options	config_options;
# endif

	// Debug overlay
	mlx_image_t	*crosshair;
}	t_game;

#endif
