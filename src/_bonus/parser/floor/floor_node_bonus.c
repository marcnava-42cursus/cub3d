/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_node_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:53 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_floor	*find_floor_by_path_bonus(t_floor *head, const char *path)
{
	t_floor	*current;

	current = head;
	while (current)
	{
		if (current->path && ft_strcmp(current->path, (char *)path) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static void	free_texture_paths_bonus(t_textures *textures)
{
	if (!textures)
		return ;
	if (textures->north_path)
		free(textures->north_path);
	if (textures->south_path)
		free(textures->south_path);
	if (textures->west_path)
		free(textures->west_path);
	if (textures->east_path)
		free(textures->east_path);
	textures->north_path = NULL;
	textures->south_path = NULL;
	textures->west_path = NULL;
	textures->east_path = NULL;
}

void	free_floor_node_bonus(t_floor *floor)
{
	if (!floor)
		return ;
	if (floor->map.grid)
		free_map(&(floor->map));
	if (floor->path)
		free(floor->path);
	if (floor->up_path)
		free(floor->up_path);
	if (floor->down_path)
		free(floor->down_path);
	free_texture_paths_bonus(&floor->textures);
	free_textures(&floor->textures);
	free(floor);
}

void	init_floor_node_fields_bonus(t_floor *floor,
		char *canonical_path, t_cub_data *src, int index)
{
	floor->path = canonical_path;
	floor->map = src->map;
	floor->up_path = src->up_path;
	floor->down_path = src->down_path;
	floor->textures = src->textures;
	floor->floor_color = src->floor_color;
	floor->ceiling_color = src->ceiling_color;
	floor->up = NULL;
	floor->down = NULL;
	floor->next = NULL;
	floor->index = index;
	floor->parsed_neighbors = 0;
	floor->has_player = (src->player.x >= 0.0f && src->player.y >= 0.0f);
	floor->player = src->player;
	floor->textures_loaded = false;
}

void	reset_source_floor_data_bonus(t_cub_data *src)
{
	ft_bzero(&src->map, sizeof(t_map));
	ft_bzero(&src->textures, sizeof(t_textures));
	ft_bzero(&src->floor_color, sizeof(t_color));
	ft_bzero(&src->ceiling_color, sizeof(t_color));
	src->up_path = NULL;
	src->down_path = NULL;
}
