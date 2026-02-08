/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 01:33:16 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 15:23:46 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>

int	init_game(t_game *game, const char *map_file)
{
	if (!parse_cub_file(map_file, &game->cub_data))
	{
		printf("Error\nFailed to parse map file %s\n", map_file);
		return (0);
	}
	if (!load_textures(&game->cub_data.textures))
	{
		printf("Error\nFailed to load textures\n");
		free_cub_data(&game->cub_data);
		return (0);
	}
	if (game->cub_data.current_floor)
	{
		game->cub_data.current_floor->textures = game->cub_data.textures;
		game->cub_data.current_floor->textures_loaded = true;
	}
	if (!window_init(game))
	{
		printf("Error\nFailed to initialize window\n");
		return (0);
	}
	return (1);
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	free_cub_data(&game->cub_data);
	if (game->mlx)
		window_free(game);
}

static void	render_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	render_double_buffer(game);
}

int	run_game(t_game *game)
{
	INIT_MOVEMENT_SYSTEM(game);
	mlx_loop_hook(game->mlx, render_loop, (void *)game);
	mlx_loop(game->mlx);
	return (1);
}

int	main(int argc, char **argv)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
	{
		printf("Error:\n\tUsage: %s <map_path>\n", argv[0]);
		return (1);
	}
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
