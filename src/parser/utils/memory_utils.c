/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:15:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 14:53:12 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"
//temporary
#include "render.h"

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

static void	free_texture_paths(t_textures *textures)
{
	if (!textures)
		return ;
	free_texture_path(&textures->north_path);
	free_texture_path(&textures->south_path);
	free_texture_path(&textures->west_path);
	free_texture_path(&textures->east_path);
}

static void	free_floor_list(t_floor *head)
{
	t_floor	*current;
	t_floor	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_texture_paths(&current->textures);
		free_textures(&current->textures);
		free_texture_path(&current->up_path);
		free_texture_path(&current->down_path);
		if (current->map.grid)
			free_map(&(current->map));
		free_texture_path(&current->path);
		free(current);
		current = next;
	}
}

void	free_cub_data(t_cub_data *data)
{
	if (!data)
		return ;
	if (data->floors)
	{
		free_floor_list(data->floors);
		data->map.grid = NULL;
	}
	else
	{
		free_texture_path(&data->textures.north_path);
		free_texture_path(&data->textures.south_path);
		free_texture_path(&data->textures.west_path);
		free_texture_path(&data->textures.east_path);
		free_textures(&data->textures);
		free_map(&data->map);
	}
	free_texture_path(&data->up_path);
	free_texture_path(&data->down_path);
	free_texture_path(&data->player_floor_path);
	atlas_free(&data->player.textures.hand);
	atlas_free(&data->player.textures.thumb);
	atlas_free(&data->player.textures.weapon);
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
