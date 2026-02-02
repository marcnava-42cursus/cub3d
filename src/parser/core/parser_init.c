/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/02 16:57:18 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"
//temporary
#include "render.h"
#include "paths.h"

void	init_cub_data(t_cub_data *data)
{
	ft_bzero(data, sizeof(t_cub_data));
	ft_memset(&(data->floor_color), -1, sizeof(t_color));
	ft_memset(&(data->ceiling_color), -1, sizeof(t_color));
	data->player.x = -1.0f;
	data->player.y = -1.0f;
	data->player.angle = 0.0f;
	data->player.pitch = 0.0f;
	data->player.orientation = 0;
	data->player.inventory = '2';
	data->player.state = STATE_HOLD;
	atlas_init(&data->player.textures.hand, HAND_ATLAS,
		HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&data->player.textures.thumb, THUMB,
		HAND_TEXTURE_WIDTH, HAND_TEXTURE_HEIGHT);
	atlas_init(&data->player.textures.weapon, WEAPON,
		WEAPON_TEXTURE_WIDTH, WEAPON_TEXTURE_HEIGHT);
}

void	init_cub_data_parser_only(t_cub_data *data)
{
	ft_bzero(data, sizeof(t_cub_data));
	ft_memset(&(data->floor_color), -1, sizeof(t_color));
	ft_memset(&(data->ceiling_color), -1, sizeof(t_color));
	data->player.x = -1.0f;
	data->player.y = -1.0f;
	data->player.angle = 0.0f;
	data->player.pitch = 0.0f;
	data->player.orientation = 0;
	data->player.inventory = '2';
	data->player.state = STATE_HOLD;
}
