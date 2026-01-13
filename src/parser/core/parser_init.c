/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/13 15:28:37 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
//temporary
#include "render.h"

void	init_cub_data(t_cub_data *data)
{
	ft_bzero(data, sizeof(t_cub_data));
	ft_memset(&(data->floor_color), -1, sizeof(t_color));
	ft_memset(&(data->ceiling_color), -1, sizeof(t_color));
	data->player.x = -1.0f;
	data->player.y = -1.0f;
	data->player.angle = 0.0f;
	data->player.orientation = 0;
	data->player.inventory = '2';
	//data->player.textures.left_hand_path
	//	= "./assets/textures/player/test_hand.xpm42";
	//data->player.textures.left_thumb_path
	//	= "./assets/textures/player/test_thumb.xpm42";
	//data->player.textures.weapon_path
	//	= "./assets/textures/player/test_weapon.xpm42";
	//data->player.textures.left_hand = mlx_load_xpm42(
	//		data->player.textures.left_hand_path);
	//data->player.textures.left_thumb = mlx_load_xpm42(
	//		data->player.textures.left_thumb_path);
	//data->player.textures.weapon = mlx_load_xpm42(
	//		data->player.textures.weapon_path);
	data->player_floor_index = -1;
	ft_bzero(data->elevator_ids, sizeof(data->elevator_ids));
	ft_bzero(data->elevator_floor_a, sizeof(data->elevator_floor_a));
	ft_bzero(data->elevator_floor_b, sizeof(data->elevator_floor_b));
}

void	init_cub_data_parser_only(t_cub_data *data)
{
	ft_bzero(data, sizeof(t_cub_data));
	ft_memset(&(data->floor_color), -1, sizeof(t_color));
	ft_memset(&(data->ceiling_color), -1, sizeof(t_color));
	data->player.x = -1.0f;
	data->player.y = -1.0f;
	data->player.angle = 0.0f;
	data->player.orientation = 0;
	data->player.inventory = '2';
	data->floors = NULL;
	data->current_floor = NULL;
	data->floor_count = 0;
	data->player_floor_index = -1;
	data->player_floor_path = NULL;
	data->elevator_id_count = 0;
	ft_bzero(data->elevator_ids, sizeof(data->elevator_ids));
	ft_bzero(data->elevator_floor_a, sizeof(data->elevator_floor_a));
	ft_bzero(data->elevator_floor_b, sizeof(data->elevator_floor_b));
	atlas_init(&data->player.textures.hand,
			"./assets/textures/player/test_hand_atlas.xpm42",
			HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&data->player.textures.thumb,
			"./assets/textures/player/test_thumb.xpm42",
			HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&data->player.textures.weapon,
			"./assets/textures/player/test_weapon.xpm42",
			WEAPON_TEXTURE_WIDTH, WEAPON_TEXTURE_HEIGHT);
}
