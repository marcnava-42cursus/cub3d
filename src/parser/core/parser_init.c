/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/13 17:08:55 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_cub_data(t_cub_data *data)
{
	ft_bzero(data, sizeof(t_cub_data));
	ft_memset(&(data->floor_color), -1, sizeof(t_color));
	ft_memset(&(data->ceiling_color), -1, sizeof(t_color));
	ft_memset(&(data->player), -1.0f, sizeof(t_player));
}
