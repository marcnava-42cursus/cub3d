/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/27 19:53:09 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>

int	init_game(t_game *game, const char *map_file)
{
	// Parsear el archivo .cub
	if (!parse_cub_file(map_file, &game->cub_data))
	{
		printf("Error: Failed to parse map file %s\n", map_file);
		return (0);
	}

	// Cargar texturas XPM
	if (!load_textures(&game->cub_data.textures))
	{
		printf("Error: Failed to load textures\n");
		free_cub_data(&game->cub_data);
		return (0);
	}

	// Mostrar datos parseados (para debugging)
	print_cub_data(&game->cub_data);

	// Inicializar ventana MLX
	if (!window_init(game))
	{
		printf("Error: Failed to initialize window\n");
		return (0);
	}

	// Cargar texturas 2D
	if (!load_map_textures(game))
	{
		printf("Error: Failed to load textures\n");
		return (0);
	}

	return (1);
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return;

	// Liberar datos del parser
	free_cub_data(&game->cub_data);

	// Aquí se liberarían otros recursos del juego
	// como ventanas MLX, texturas cargadas, etc.
	if (game->mlx)
		window_free(game);
}

int	run_game(t_game *game)
{
	// Por ahora solo muestra que el parsing fue exitoso
	printf("Game initialized successfully!\n");
	printf("Map size: %dx%d\n", game->cub_data.map.width, game->cub_data.map.height);
	printf("Player at (%.2f, %.2f) facing %c\n", 
		game->cub_data.player.x, game->cub_data.player.y, game->cub_data.player.orientation);

    // Inicializar sistema de movimiento
    init_movement_system(game);
    // Por defecto, mantener oculto el overlay 2D; tecla 'M' lo alterna
    set_map_overlay_visible(game, false);

	// Iniciar loop de MLX
	// Aquí iría el game loop principal
	// - Procesamiento de input
	// - Lógica del juego
	// - Rendering
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

	ft_bzero(&game, sizeof(t_game));
	if (!validate_args(argc, argv))
		return (1);
	if (!init_game(&game, argv[1]))
	{
		cleanup_game(&game);
		return (1);
	}
	if (!run_game(&game))
	{
		cleanup_game(&game);
		return (1);
	}
	cleanup_game(&game);
	return (0);
}
