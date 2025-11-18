/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_setter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:50:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/03 19:04:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	set_north_texture(t_textures *textures, char *path)
{
	if (textures->north_path)
	{
		printf("Error: North texture already defined\n");
		free(path);
		return (0);
	}
	textures->north_path = path;
	return (1);
}

static int	set_south_texture(t_textures *textures, char *path)
{
	if (textures->south_path)
	{
		printf("Error: South texture already defined\n");
		free(path);
		return (0);
	}
	textures->south_path = path;
	return (1);
}

static int	set_west_texture(t_textures *textures, char *path)
{
	if (textures->west_path)
	{
		printf("Error: West texture already defined\n");
		free(path);
		return (0);
	}
	textures->west_path = path;
	return (1);
}

static int	set_east_texture(t_textures *textures, char *path)
{
	if (textures->east_path)
	{
		printf("Error: East texture already defined\n");
		free(path);
		return (0);
	}
	textures->east_path = path;
	return (1);
}

int	set_texture(t_textures *textures, const char *identifier, char *path)
{
	if (ft_strcmp(identifier, "NO") == 0)
		return (set_north_texture(textures, path));
	else if (ft_strcmp(identifier, "SO") == 0)
		return (set_south_texture(textures, path));
	else if (ft_strcmp(identifier, "WE") == 0)
		return (set_west_texture(textures, path));
	else if (ft_strcmp(identifier, "EA") == 0)
		return (set_east_texture(textures, path));
	else
	{
		free(path);
		return (0);
	}
}
