/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_data_helpers_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 04:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 04:40:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "render.h"

static void	free_texture_path_bonus(char **texture_path)
{
	if (*texture_path)
	{
		free(*texture_path);
		*texture_path = NULL;
	}
}

void	free_texture_paths_bonus(t_textures *textures)
{
	free_texture_path_bonus(&textures->north_path);
	free_texture_path_bonus(&textures->south_path);
	free_texture_path_bonus(&textures->west_path);
	free_texture_path_bonus(&textures->east_path);
	free_texture_path_bonus(&textures->floor_path);
	free_texture_path_bonus(&textures->ceiling_path);
	free_texture_path_bonus(&textures->fog_path);
}

void	free_custom_textures_bonus(t_custom_texture *custom)
{
	t_custom_texture	*next;

	while (custom)
	{
		next = custom->next;
		if (custom->texture)
			mlx_delete_xpm42(custom->texture);
		free(custom->path);
		free(custom);
		custom = next;
	}
}

static void	free_floor_textures_bonus(t_floor *floor)
{
	free_texture_paths_bonus(&floor->textures);
	free_custom_textures_bonus(floor->textures.custom);
	floor->textures.custom = NULL;
	free_textures(&floor->textures);
}

void	free_floor_list_bonus(t_floor *head)
{
	t_floor	*current;
	t_floor	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free_floor_textures_bonus(current);
		free(current->up_path);
		free(current->down_path);
		free(current->path);
		if (current->map.grid)
			free_map(&current->map);
		free(current);
		current = next;
	}
}
