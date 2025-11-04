/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:15:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/03 18:56:31 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_map(t_map *map)
{
	int	i;

	if (!map || !map->grid)
		return ;
	i = 0;
	while (i < map->height)
	{
		if (map->grid[i])
			free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
}

static void	free_texture_path(char **texture_path)
{
	if (*texture_path)
	{
		free(*texture_path);
		*texture_path = NULL;
	}
}

void	free_cub_data(t_cub_data *data)
{
	if (!data)
		return ;
	free_texture_path(&data->textures.north_path);
	free_texture_path(&data->textures.south_path);
	free_texture_path(&data->textures.west_path);
	free_texture_path(&data->textures.east_path);
	free_textures(&data->textures);
	free_map(&data->map);
}

void	free_lines(char **lines, int line_count)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (i < line_count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}
