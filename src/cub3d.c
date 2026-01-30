/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 15:20:28 by ivmirand         ###   ########.fr       */
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
	if (game->cub_data.current_floor)
	{
		game->cub_data.current_floor->textures = game->cub_data.textures;
		game->cub_data.current_floor->textures_loaded = true;
	}

	// Inicializar ventana MLX
	if (!window_init(game))
	{
		printf("Error: Failed to initialize window\n");
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

static void render_loop(void *param)
{
	t_game *game = (t_game *)param;

	render_double_buffer(game);
}

int	run_game(t_game *game)
{
    // Inicializar sistema de movimiento
	INIT_MOVEMENT_SYSTEM(game);

	// Iniciar loop de MLX
	// Aquí iría el game loop principal
	// - Procesamiento de input
	// - Lógica del juego
	// - Rendering
	mlx_loop_hook(game->mlx, render_loop, (void *)game);
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
