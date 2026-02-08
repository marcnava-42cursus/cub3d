/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_data_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:45:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

void	free_texture_paths_bonus(t_textures *textures);
void	free_custom_textures_bonus(t_custom_texture *custom);
void	free_floor_list_bonus(t_floor *head);

void	free_map(t_map *map)
{
	int	i;

	if (!map || !map->grid)
		return ;
	i = 0;
	while (i < map->height)
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
}

static void	reset_cub_links_bonus(t_cub_data *data)
{
	free(data->up_path);
	free(data->down_path);
	free(data->player_floor_path);
	data->up_path = NULL;
	data->down_path = NULL;
	data->player_floor_path = NULL;
	data->floors = NULL;
	data->current_floor = NULL;
	data->floor_count = 0;
	data->map.grid = NULL;
	data->map.width = 0;
	data->map.height = 0;
}

void	free_cub_data(t_cub_data *data)
{
	if (!data)
		return ;
	if (data->floors)
	{
		free_floor_list_bonus(data->floors);
		data->map.grid = NULL;
	}
	else
	{
		free_texture_paths_bonus(&data->textures);
		free_custom_textures_bonus(data->textures.custom);
		data->textures.custom = NULL;
		free_textures(&data->textures);
		free_map(&data->map);
	}
	ft_bzero(&data->textures, sizeof(t_textures));
	reset_cub_links_bonus(data);
}
