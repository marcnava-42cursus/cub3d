/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/10/02 12:50:05 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "logic.h"

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
				if (game->cub_data.player.angle >= -FT_PI/4 && game->cub_data.player.angle < FT_PI/4)
					printf(">");
				else if (game->cub_data.player.angle >= FT_PI/4 && game->cub_data.player.angle < 3*FT_PI/4)
					printf("v");
				else if (game->cub_data.player.angle >= 3*FT_PI/4 || game->cub_data.player.angle < -3*FT_PI/4)
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

// movement and rotation extracted to separate modules

// Variable global para trackear la última posición de grid
static int last_grid_x = -1;
static int last_grid_y = -1;

// Función para actualizar solo la posición del jugador
void	update_player_position(t_game *game)
{
	// Actualizar solo la posición del jugador (si el overlay está activo)
	if (game->map_2d_visible)
		render_player_dynamic(game);
}

// Función para manejar las teclas presionadas y soltadas
// key hook extracted to input module

// Función de loop continuo para procesar movimiento suave
void	update_game_loop(void *param)
{
    t_game	*game = (t_game *)param;
    bool		moved = false;
	int			current_grid_x;
	int			current_grid_y;

	update_delta_time(game);
	refresh_key_states(game);
	if (game->delta_time <= 0.0)
		return ;
	
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
	
    // Actualizar render si hubo movimiento
    if (moved)
    {
        if (game->map_2d_visible)
            update_player_position(game);
        // Re-renderizar mundo (raycaster + minimapa)
        render_double_buffer(game);

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
		game->cub_data.player.angle = -FT_PI/2;
	else if (game->cub_data.player.orientation == SOUTH)
		game->cub_data.player.angle = FT_PI/2;
	else if (game->cub_data.player.orientation == EAST)
		game->cub_data.player.angle = 0;
	else if (game->cub_data.player.orientation == WEST)
		game->cub_data.player.angle = FT_PI;

	init_player_parameters(game);
	
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
