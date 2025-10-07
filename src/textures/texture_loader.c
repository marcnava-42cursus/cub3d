/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 02:35:00 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/21 03:27:21 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "parser.h"
#include <stdio.h>

int	load_textures(t_textures *textures)
{
    if (textures->north_path)
    {
        textures->north = mlx_load_xpm42(textures->north_path);
        if (!textures->north)
        {
            printf("Error: Failed to load north texture: %s\n", textures->north_path);
            return (0);
        }
    }
    if (textures->south_path)
    {
        textures->south = mlx_load_xpm42(textures->south_path);
        if (!textures->south)
        {
            printf("Error: Failed to load south texture: %s\n", textures->south_path);
            return (0);
        }
    }
    if (textures->east_path)
    {
        textures->east = mlx_load_xpm42(textures->east_path);
        if (!textures->east)
        {
            printf("Error: Failed to load east texture: %s\n", textures->east_path);
            return (0);
        }
    }
    if (textures->west_path)
    {
        textures->west = mlx_load_xpm42(textures->west_path);
        if (!textures->west)
        {
            printf("Error: Failed to load west texture: %s\n", textures->west_path);
            return (0);
        }
    }

    return (1);
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
}
