/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:20:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 20:41:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	print_colors(const t_cub_data *data)
{
	printf("Colors:\n");
	printf("  Floor: RGB(%d, %d, %d)\n",
		data->floor_color.r, data->floor_color.g, data->floor_color.b);
	printf("  Ceiling: RGB(%d, %d, %d)\n",
		data->ceiling_color.r, data->ceiling_color.g, data->ceiling_color.b);
}

static void	print_player(const t_cub_data *data)
{
	printf("Player:\n");
	printf("  Position: (%.2f, %.2f)\n",
		(double)data->player.x, (double)data->player.y);
	printf("  Orientation: %c\n", data->player.orientation);
}

static void	print_map(const t_cub_data *data)
{
	int	i;

	printf("Map (%dx%d):\n", data->map.width, data->map.height);
	i = 0;
	while (i < data->map.height)
	{
		printf("  %s\n", data->map.grid[i]);
		i++;
	}
}

void	print_cub_data(const t_cub_data *data)
{
	if (!data)
	{
		printf("Error: NULL data\n");
		return ;
	}
	printf("=== CUB FILE DATA ===\n");
	print_textures(data);
	print_colors(data);
	print_player(data);
	print_map(data);
	printf("==================\n");
}
