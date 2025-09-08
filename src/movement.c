/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/07 12:01:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include <stdio.h>

#define MOVE_SPEED 0.05f
#define ROT_SPEED 0.03f

// Función para verificar si una posición es válida (no es pared)
static bool	is_valid_position(t_game *game, float x, float y)
{
	int	map_x;
	int	map_y;
	
	map_x = (int)floor(x);
	map_y = (int)floor(y);
	
	if (map_x < 0 || map_x >= game->cub_data.map.width ||
		map_y < 0 || map_y >= game->cub_data.map.height)
		return (false);
	
	return (game->cub_data.map.grid[map_y][map_x] != '1');
}

// Función para imprimir el mapa 2D en terminal con la posición del jugador
void	print_map_2d(t_game *game)
{
	int	y;
	int	x;
	int	player_x;
	int	player_y;
	
	player_x = (int)floor(game->cub_data.player.x);
	player_y = (int)floor(game->cub_data.player.y);
	
	printf("\033[2J\033[H"); // Limpiar pantalla y mover cursor al inicio
	printf("Player position: (%.2f, %.2f) - Grid: (%d, %d) - Angle: %.2f\n", 
		game->cub_data.player.x, game->cub_data.player.y, 
		player_x, player_y, game->cub_data.player.angle);
	printf("Map:\n");
	
	y = 0;
	while (y < game->cub_data.map.height)
	{
		x = 0;
		while (x < game->cub_data.map.width)
		{
			if (x == player_x && y == player_y)
			{
				// Mostrar dirección del jugador
				if (game->cub_data.player.angle >= -M_PI/4 && game->cub_data.player.angle < M_PI/4)
					printf(">");
				else if (game->cub_data.player.angle >= M_PI/4 && game->cub_data.player.angle < 3*M_PI/4)
					printf("v");
				else if (game->cub_data.player.angle >= 3*M_PI/4 || game->cub_data.player.angle < -3*M_PI/4)
					printf("<");
				else
					printf("^");
			}
			else
			{
				printf("%c", game->cub_data.map.grid[y][x]);
			}
			x++;
		}
		printf("\n");
		y++;
	}
	printf("\n");
}

// Mover hacia adelante/atrás en la dirección que mira el jugador
void	move_forward(t_game *game, bool forward)
{
	float	new_x;
	float	new_y;
	float	direction;
	
	direction = forward ? 1.0f : -1.0f;
	new_x = game->cub_data.player.x + cos(game->cub_data.player.angle) * MOVE_SPEED * direction;
	new_y = game->cub_data.player.y + sin(game->cub_data.player.angle) * MOVE_SPEED * direction;
	
	if (is_valid_position(game, new_x, game->cub_data.player.y))
		game->cub_data.player.x = new_x;
	if (is_valid_position(game, game->cub_data.player.x, new_y))
		game->cub_data.player.y = new_y;
}

// Mover hacia los lados (strafe)
void	move_strafe(t_game *game, bool right)
{
	float	new_x;
	float	new_y;
	float	strafe_angle;
	float	direction;
	
	direction = right ? 1.0f : -1.0f;
	strafe_angle = game->cub_data.player.angle + M_PI/2 * direction;
	new_x = game->cub_data.player.x + cos(strafe_angle) * MOVE_SPEED;
	new_y = game->cub_data.player.y + sin(strafe_angle) * MOVE_SPEED;
	
	if (is_valid_position(game, new_x, game->cub_data.player.y))
		game->cub_data.player.x = new_x;
	if (is_valid_position(game, game->cub_data.player.x, new_y))
		game->cub_data.player.y = new_y;
}

// Rotar el jugador
void	rotate_player(t_game *game, bool right)
{
	float	direction;
	
	direction = right ? 1.0f : -1.0f;
	game->cub_data.player.angle += ROT_SPEED * direction;
	
	// Mantener el ángulo en el rango [-π, π]
	if (game->cub_data.player.angle > M_PI)
		game->cub_data.player.angle -= 2 * M_PI;
	else if (game->cub_data.player.angle < -M_PI)
		game->cub_data.player.angle += 2 * M_PI;
}

// Variable global para trackear la última posición de grid
static int last_grid_x = -1;
static int last_grid_y = -1;

// Variable estática para trackear si necesitamos redibujar el mapa
static bool map_drawn = false;

// Función para actualizar solo la posición del jugador
void	update_player_position(t_game *game)
{
	// Solo dibujar el mapa una vez
	if (!map_drawn)
	{
		render_map_tiles_static(game);
		map_drawn = true;
	}
	
	// Actualizar solo la posición del jugador
	render_player_dynamic(game);
}

// Función para manejar las teclas presionadas y soltadas
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game = (t_game *)param;
	
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(game->mlx);
	
	// Actualizar estado de teclas
	if (keydata.action == MLX_PRESS)
	{
		if (keydata.key == MLX_KEY_W)
			game->key_w_pressed = true;
		else if (keydata.key == MLX_KEY_S)
			game->key_s_pressed = true;
		else if (keydata.key == MLX_KEY_A)
			game->key_a_pressed = true;
		else if (keydata.key == MLX_KEY_D)
			game->key_d_pressed = true;
		else if (keydata.key == MLX_KEY_LEFT)
			game->key_left_pressed = true;
		else if (keydata.key == MLX_KEY_RIGHT)
			game->key_right_pressed = true;
	}
	else if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
			game->key_w_pressed = false;
		else if (keydata.key == MLX_KEY_S)
			game->key_s_pressed = false;
		else if (keydata.key == MLX_KEY_A)
			game->key_a_pressed = false;
		else if (keydata.key == MLX_KEY_D)
			game->key_d_pressed = false;
		else if (keydata.key == MLX_KEY_LEFT)
			game->key_left_pressed = false;
		else if (keydata.key == MLX_KEY_RIGHT)
			game->key_right_pressed = false;
	}
}

// Función de loop continuo para procesar movimiento suave
void	update_game_loop(void *param)
{
	t_game	*game = (t_game *)param;
	bool		moved = false;
	int			current_grid_x;
	int			current_grid_y;
	
	// Procesar movimiento basado en el estado de las teclas
	if (game->key_w_pressed)
	{
		move_forward(game, true);
		moved = true;
	}
	if (game->key_s_pressed)
	{
		move_forward(game, false);
		moved = true;
	}
	if (game->key_a_pressed)
	{
		move_strafe(game, false);
		moved = true;
	}
	if (game->key_d_pressed)
	{
		move_strafe(game, true);
		moved = true;
	}
	if (game->key_left_pressed)
	{
		rotate_player(game, false);
		moved = true;
	}
	if (game->key_right_pressed)
	{
		rotate_player(game, true);
		moved = true;
	}
	
	// Solo actualizar si hubo movimiento
	if (moved)
	{
		update_player_position(game);
		
		// Imprimir en terminal cada vez que cambien de celda
		current_grid_x = (int)floor(game->cub_data.player.x);
		current_grid_y = (int)floor(game->cub_data.player.y);
		
		if (current_grid_x != last_grid_x || current_grid_y != last_grid_y)
		{
			print_map_2d(game);
			last_grid_x = current_grid_x;
			last_grid_y = current_grid_y;
		}
		else
		{
			// Solo mostrar posición actual sin el mapa completo
			printf("Player: (%.2f, %.2f) - Grid: (%d, %d) - Angle: %.2f\r",
				game->cub_data.player.x, game->cub_data.player.y,
				current_grid_x, current_grid_y, game->cub_data.player.angle);
			fflush(stdout);
		}
	}
}

// Inicializar el sistema de movimiento
void	init_movement_system(t_game *game)
{
	// Convertir la orientación inicial a ángulo en radianes
	if (game->cub_data.player.orientation == NORTH)
		game->cub_data.player.angle = -M_PI/2;
	else if (game->cub_data.player.orientation == SOUTH)
		game->cub_data.player.angle = M_PI/2;
	else if (game->cub_data.player.orientation == EAST)
		game->cub_data.player.angle = 0;
	else if (game->cub_data.player.orientation == WEST)
		game->cub_data.player.angle = M_PI;
	
	// Renderizar el estado inicial completo (solo una vez)
	render_map_2d_initial(game);
	
	// Imprimir el mapa inicial en terminal
	print_map_2d(game);
	printf("\n=== CONTROLES ===\n");
	printf("WASD: Movimiento\n");
	printf("Flechas: Rotación\n");
	printf("ESC: Salir\n\n");
	
	// Configurar el hook de teclas
	mlx_key_hook(game->mlx, key_hook, game);
	
	// Configurar el loop de actualización continua
	mlx_loop_hook(game->mlx, update_game_loop, game);
}
