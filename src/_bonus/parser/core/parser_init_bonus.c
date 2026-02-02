/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/02 16:55:59 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"
#include "paths.h"
#include "animation.h"

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
	data->player_floor_index = -1;
	data->player.state = STATE_HOLD;
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
	data->player.pitch = 0.0f;
	data->player.orientation = 0;
	data->player.inventory = '2';
	data->floors = NULL;
	data->current_floor = NULL;
	data->floor_count = 0;
	data->player_floor_index = -1;
	data->player_floor_path = NULL;
	data->elevator_id_count = 0;
	data->player.state = STATE_HOLD;
	ft_bzero(data->elevator_ids, sizeof(data->elevator_ids));
	ft_bzero(data->elevator_floor_a, sizeof(data->elevator_floor_a));
	ft_bzero(data->elevator_floor_b, sizeof(data->elevator_floor_b));
}
