/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/15 13:39:23 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "config_bonus.h"
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
	config_options_init(game);
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

	// Inicializar ventana MLX (Bonus version)
	if (!window_init_bonus(game))
	{
		printf("Error: Failed to initialize window\n");
		return (0);
	}

	return (1);
}

static void render_loop(void *param)
{
	t_game *game = (t_game *)param;
	bool	orb_moved;

	orb_moved = orb_projectile_update(game, game->mlx->delta_time);
	update_player_anims(&game->cub_data.player, game->mlx->delta_time);
	render_double_buffer(game);
	if (orb_moved && game->map_2d_visible)
		render_player_dynamic_bonus(game);
}

int	run_game_bonus(t_game *game)
{
    // Inicializar sistema de movimiento (Bonus)
    init_movement_system_bonus(game);

	// Iniciar loop de MLX
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
