/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:35:00 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 17:01:49 by marcnava         ###   ########.fr       */
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
		textures->floor = mlx_load_xpm42(FLOOR_FLESH);
	if (textures->ceiling_path)
		load_texture(&textures->ceiling, textures->ceiling_path);
	else
		textures->ceiling = mlx_load_xpm42(CEILING_FLESH);
	if (textures->fog_path)
		load_texture(&textures->fog, textures->fog_path);
	else
		textures->fog = mlx_load_xpm42(FOG_128);
	return (true);
}

static void	free_texture(xpm_t *xpm)
{
	if (xpm)
	{
		mlx_delete_xpm42(xpm);
		xpm = NULL;
	}
}

void	free_textures(t_textures *textures)
{
	free_texture(textures->north);
	free_texture(textures->south);
	free_texture(textures->east);
	free_texture(textures->west);
	free_texture(textures->floor);
	free_texture(textures->ceiling);
	free_texture(textures->fog);
	atlas_free(&textures->living);
}
