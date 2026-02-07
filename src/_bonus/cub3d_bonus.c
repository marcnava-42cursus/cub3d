/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 05:48:37 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "config_bonus.h"
#include "logic_bonus.h"
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
	bonus_audio_shutdown();

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
	atlas_init(&game->cub_data.player.hand_atlas,
		"./assets/textures/player/test_hand_atlas.xpm42",
		HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&game->cub_data.player.thumb_atlas,
		"./assets/textures/player/test_thumb.xpm42",
		HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&game->cub_data.effects.orb_atlas,
		"./assets/textures/player/test_weapon.xpm42",
		WEAPON_TEXTURE_WIDTH, WEAPON_TEXTURE_HEIGHT);
	atlas_init(&game->cub_data.effects.absorb_atlas,
		"./assets/textures/effects/absorb_effect_32_colors.xpm42", 128, 128);
	atlas_init(&game->cub_data.effects.door_atlas,
			"./assets/textures/walls/door.xpm42", 128, 128);
	atlas_init(&game->cub_data.block.atlas,
			"./assets/textures/walls/test_living_flesh_atlas.xpm42", 128, 128);
	init_player_anims(&game->cub_data.player);
	init_living_block_anims(&game->cub_data.block);
	init_absorb_anims(&game->cub_data.effects);
	init_orb_anims(&game->cub_data.effects);
	init_door_anims(&game->cub_data.effects);	
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
	if (!load_map_textures_bonus(game))
	{
		printf("Error: Failed to load 2D map textures\n");
		return (0);
	}
	if (!bonus_audio_init())
		printf("Warning: Audio bonus disabled (assets/audio/scream.mp3)\n");

	return (1);
}

static void render_loop(void *param)
{
	t_game	*game;
	bool	orb_moved;

	game = (t_game *)param;
	orb_moved = orb_projectile_update(game, game->mlx->delta_time);
	update_player_anims(&game->cub_data.player, game->mlx->delta_time);
	update_living_block_anims(&game->cub_data.block, game->mlx->delta_time);
	update_creating_block_state(game);
	update_absorb_anims(&game->cub_data.player, &game->cub_data.effects,
		game->mlx->delta_time);
	update_orb_anims(&game->orb, &game->cub_data.effects,
		game->mlx->delta_time);
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
