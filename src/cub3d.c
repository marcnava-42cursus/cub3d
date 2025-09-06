/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/07 00:22:48 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	init_game(t_game *game, const char *map_file)
{
	// Parsear el archivo .cub
	if (!parse_cub_file(map_file, &game->cub_data))
	{
		printf("Error: Failed to parse map file %s\n", map_file);
		return (0);
	}

	// Mostrar datos parseados (para debugging)
	print_cub_data(&game->cub_data);

	// Aquí se inicializarían otros componentes del juego
	// como la ventana gráfica, texturas MLX, etc.
	window_init(game);	

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
	window_free(game);
}

int	run_game(t_game *game)
{
	// Por ahora solo muestra que el parsing fue exitoso
	printf("Game initialized successfully!\n");
	printf("Map size: %dx%d\n", game->cub_data.map.width, game->cub_data.map.height);
	printf("Player at (%d, %d) facing %c\n", 
		game->cub_data.player.x, game->cub_data.player.y, game->cub_data.player.orientation);

	// Aquí iría el game loop principal
	// - Procesamiento de input
	// - Lógica del juego
	// - Rendering
	render_bg(game);
	render_atmospheric_perspective(game);
	render_minimap(game);
	mlx_loop(game->mlx);

	return (1);
}

int main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (printf("Error:\n\tUsage: %s <map_path>\n", argv[0]), 1);

	// Inicializar juego
	if (!init_game(&game, argv[1]))
	{
		cleanup_game(&game);
		return (1);
	}

	// Ejecutar juego
	if (!run_game(&game))
	{
		cleanup_game(&game);
		return (1);
	}

	// Limpiar recursos
	cleanup_game(&game);
	return (0);
}
