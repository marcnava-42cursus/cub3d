/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/24 10:40:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>

// Prototypes for bonus functions
int		parse_cub_file_bonus(const char *filename, t_cub_data *data);
int		load_map_textures_bonus(t_game *game);
void	init_movement_system_bonus(t_game *game);
void	set_map_overlay_visible_bonus(t_game *game, bool visible);
bool	window_init_bonus(t_game *game);
void	window_free_bonus(t_game *game);

void	cleanup_game_bonus(t_game *game)
{
	if (!game)
		return;

	// Liberar datos del parser
	free_cub_data(&game->cub_data);

	if (game->mlx)
		window_free_bonus(game);
}

int	init_game_bonus(t_game *game, const char *map_file)
{
	// Parsear el archivo .cub (Bonus)
	if (!parse_cub_file_bonus(map_file, &game->cub_data))
	{
		printf("Error: Failed to parse map file %s\n", map_file);
		return (0);
	}

	// Cargar texturas XPM (Shared)
	if (!load_textures(&game->cub_data.textures))
	{
		printf("Error: Failed to load textures\n");
		free_cub_data(&game->cub_data);
		return (0);
	}
	if (game->cub_data.current_floor)
	{
		game->cub_data.current_floor->textures = game->cub_data.textures;
		game->cub_data.current_floor->textures_loaded = true;
	}

	// Mostrar datos parseados (para debugging)
	print_cub_data(&game->cub_data);

	// Inicializar ventana MLX (Bonus version)
	if (!window_init_bonus(game))
	{
		printf("Error: Failed to initialize window\n");
		return (0);
	}

	// Cargar texturas 2D (Bonus)
	if (!load_map_textures_bonus(game))
	{
		printf("Error: Failed to load textures\n");
		return (0);
	}

	return (1);
}

int	run_game_bonus(t_game *game)
{
	printf("Game initialized successfully (BONUS)!\n");
	printf("Map size: %dx%d\n", game->cub_data.map.width, game->cub_data.map.height);
	printf("Player at (%.2f, %.2f) facing %c\n", 
		game->cub_data.player.x, game->cub_data.player.y, game->cub_data.player.orientation);

    // Inicializar sistema de movimiento (Bonus)
    init_movement_system_bonus(game);
    // Por defecto, mantener oculto el overlay 2D; tecla 'M' lo alterna
    set_map_overlay_visible_bonus(game, false);

	// Iniciar loop de MLX
	render_double_buffer(game);
	mlx_loop(game->mlx);

	return (1);
}

static int	validate_args(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Error:\n\tUsage: %s <map_path>\n", argv[0]);
		return (0);
	}
	return (1);
}

int main(int argc, char **argv)
{
	t_game	game;

	memset(&game, 0, sizeof(t_game));
	if (!validate_args(argc, argv))
		return (1);
	if (!init_game_bonus(&game, argv[1]))
	{
		cleanup_game_bonus(&game);
		return (1);
	}
	if (!run_game_bonus(&game))
	{
		cleanup_game_bonus(&game);
		return (1);
	}
	cleanup_game_bonus(&game);
	return (0);
}