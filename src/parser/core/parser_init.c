/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/03 01:50:22 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_cub_data(t_cub_data *data)
{
	ft_bzero(data, sizeof(t_cub_data));
	ft_memset(&(data->floor_color), -1, sizeof(t_color));
	ft_memset(&(data->ceiling_color), -1, sizeof(t_color));
	ft_memset(&(data->player), -1.0f, sizeof(t_player));

	data->player.inventory = '2';
	data->player.textures.left_hand_path = "./assets/textures/player/test_hand.xpm42";
	data->player.textures.left_thumb_path = "./assets/textures/player/test_thumb.xpm42";
	data->player.textures.weapon_path = "./assets/textures/player/test_weapon.xpm42";
	data->player.textures.left_hand = mlx_load_xpm42(data->player.textures.left_hand_path);
	data->player.textures.left_thumb = mlx_load_xpm42(data->player.textures.left_thumb_path);
	data->player.textures.weapon = mlx_load_xpm42(data->player.textures.weapon_path);
}
