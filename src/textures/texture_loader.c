/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:35:00 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/06 22:43:56 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "paths.h"
#include "parser.h"
#include <stdio.h>
#include "render.h"

static bool	load_texture(xpm_t **xpm, char *path)
{
	if (!path)
	{
		printf("Error: Path not found: %s\n", path);
		return (false);
	}
	*xpm = mlx_load_xpm42(path);
	if (!*xpm)
	{
		printf("Error: Failed to load texture: %s\n", path);
		return (false);
	}
	return (true);
}

static int	load_custom_textures(t_custom_texture *custom)
{
	t_custom_texture	*current;
	int					count;

	if (!custom)
	{
			printf("Warning: No custom texture\n");
		return (0);
	}
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

bool	load_textures(t_textures *textures)
{
	if (!load_texture(&textures->north, textures->north_path))
		return (false);
	if (!load_texture(&textures->south, textures->south_path))
		return (false);
	if (!load_texture(&textures->east, textures->east_path))
		return (false);
	if (!load_texture(&textures->west, textures->west_path))
		return (false);
	if (textures->floor_path)
		load_texture(&textures->floor, textures->floor_path);
	else
		textures->floor = mlx_load_xpm42(
			"./assets/textures/walls/test_floor_flesh.xpm42");
	if (textures->ceiling_path)
		load_texture(&textures->ceiling, textures->ceiling_path);
	else
		textures->ceiling = mlx_load_xpm42(
			"./assets/textures/walls/test_ceiling_flesh.xpm42");
	if (textures->fog_path)
		load_texture(&textures->fog, textures->fog_path);
	else
		textures->fog = mlx_load_xpm42(
			"./assets/textures/walls/test_fog_128.xpm42");
	load_custom_textures(textures->custom);
	return (true);
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
