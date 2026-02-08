/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:20:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 14:20:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "paths.h"

int	parse_cub_file_bonus(const char *filename, t_cub_data *data);

bool	load_map_and_textures_bonus(t_game *game, const char *map_file)
{
	if (!parse_cub_file_bonus(map_file, &game->cub_data))
	{
		printf("Error: Failed to parse map file %s\n", map_file);
		return (false);
	}
	if (!load_textures(&game->cub_data.textures))
	{
		printf("Error: Failed to load textures\n");
		free_cub_data(&game->cub_data);
		return (false);
	}
	return (true);
}

void	init_bonus_visual_assets(t_game *game)
{
	atlas_init(&game->cub_data.player.hand_atlas, HAND_ATLAS,
		HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&game->cub_data.player.thumb_atlas, THUMB,
		HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&game->cub_data.effects.orb_atlas, WEAPON,
		WEAPON_TEXTURE_WIDTH, WEAPON_TEXTURE_HEIGHT);
	atlas_init(&game->cub_data.effects.absorb_atlas, ABSORB_32, 128, 128);
	atlas_init(&game->cub_data.effects.door_atlas, DOOR, 128, 128);
	atlas_init(&game->cub_data.block.atlas, LIVING_FLESH, 128, 128);
	init_player_anims(&game->cub_data.player);
	init_living_block_anims(&game->cub_data.block);
	init_absorb_anims(&game->cub_data.effects);
	init_orb_anims(&game->cub_data.effects);
	init_door_anims(&game->cub_data.effects);
}

void	sync_current_floor_textures_bonus(t_game *game)
{
	if (!game->cub_data.current_floor)
		return ;
	game->cub_data.current_floor->textures = game->cub_data.textures;
	game->cub_data.current_floor->textures_loaded = true;
}
