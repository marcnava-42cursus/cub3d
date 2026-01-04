/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/04 17:59:10 by ivmirand         ###   ########.fr       */
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
	ft_memset(&(data->player), -1.0f, sizeof(t_player));

	data->player.inventory = '2';
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
