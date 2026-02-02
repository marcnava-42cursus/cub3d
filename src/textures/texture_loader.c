/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:35:00 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/02 16:58:36 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "paths.h"
#include "parser.h"
#include <stdio.h>
#include "render.h"

static int	load_custom_textures(t_custom_texture *custom)
{
	t_custom_texture	*current;
	int					count;

	current = custom;
	count = 0;
	while (current)
	{
		current->texture = mlx_load_xpm42(current->path);
		if (!current->texture)
		{
			printf("Warning: Failed to load custom texture %s: %s\n",
				current->id, current->path);
		}
		else
		{
			printf("Loaded custom texture %s: %s\n", current->id,
				current->path);
			count++;
		}
		current = current->next;
	}
	printf("Total custom textures loaded: %d\n", count);
	return (1);
}

int	load_textures(t_textures *textures)
{
	if (textures->north_path)
	{
		textures->north = mlx_load_xpm42(textures->north_path);
		if (!textures->north)
		{
			printf("Error: Failed to load north texture: %s\n",
				textures->north_path);
			return (0);
		}
	}
	if (textures->south_path)
	{
		textures->south = mlx_load_xpm42(textures->south_path);
		if (!textures->south)
		{
			printf("Error: Failed to load south texture: %s\n",
				textures->south_path);
			return (0);
		}
	}
	if (textures->east_path)
	{
		textures->east = mlx_load_xpm42(textures->east_path);
		if (!textures->east)
		{
			printf("Error: Failed to load east texture: %s\n",
				textures->east_path);
			return (0);
		}
	}
	if (textures->west_path)
	{
		textures->west = mlx_load_xpm42(textures->west_path);
		if (!textures->west)
		{
			printf("Error: Failed to load west texture: %s\n",
				textures->west_path);
			return (0);
		}
	}
	textures->floor = mlx_load_xpm42( "./assets/textures/walls/test_floor_flesh.xpm42");
	textures->ceiling = mlx_load_xpm42( "./assets/textures/walls/test_ceiling_flesh.xpm42");
	textures->fog = mlx_load_xpm42("./assets/textures/walls/test_fog_128.xpm42");
	atlas_init(&textures->living,
			"./assets/textures/walls/test_living_flesh_atlas.xpm42", 128, 128);
	if (textures->custom)
		load_custom_textures(textures->custom);
	return (1);
}

static void	free_custom_textures(t_custom_texture *custom)
{
	t_custom_texture	*current;
	t_custom_texture	*next;

	current = custom;
	while (current)
	{
		next = current->next;
		if (current->texture)
			mlx_delete_xpm42(current->texture);
		if (current->path)
			free(current->path);
		free(current);
		current = next;
	}
}

void	free_textures(t_textures *textures)
{
	if (textures->north)
	{
		mlx_delete_xpm42(textures->north);
		textures->north = NULL;
	}
	if (textures->south)
	{
		mlx_delete_xpm42(textures->south);
		textures->south = NULL;
	}
	if (textures->east)
	{
		mlx_delete_xpm42(textures->east);
		textures->east = NULL;
	}
	if (textures->west)
	{
		mlx_delete_xpm42(textures->west);
		textures->west = NULL;
	}
	if (textures->floor)
	{
		mlx_delete_xpm42(textures->floor);
		textures->floor = NULL;
	}
	if (textures->ceiling)
	{
		mlx_delete_xpm42(textures->ceiling);
		textures->ceiling = NULL;
	}
	if (textures->fog)
	{
		mlx_delete_xpm42(textures->fog);
		textures->fog = NULL;
	}
	atlas_free(&textures->living);
	if (textures->custom)
	{
		free_custom_textures(textures->custom);
		textures->custom = NULL;
	}
}
