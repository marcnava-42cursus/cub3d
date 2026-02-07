/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:10:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 20:37:45 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

int	validate_file_extension(const char *filename)
{
	int	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	return (ft_strcmp((char *)(filename + len - 4), ".cub") == 0);
}

int	validate_texture_path(const char *path)
{
	FILE	*file;

	if (!path || ft_strlen(path) == 0)
		return (0);
	file = fopen(path, "r");
	if (!file)
		return (0);
	fclose(file);
	return (1);
}

int	validate_texture_id(const char *identifier)
{
	if (!identifier || ft_strlen(identifier) != 2)
	{
		printf("Error: Invalid texture identifier\n");
		return (0);
	}
	if (!ft_isalpha(identifier[1])
		|| (!ft_isalpha(identifier[0]) && !ft_isdigit(identifier[0])))
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (0);
	}
	return (1);
}

int	validate_parsed_data(const t_cub_data *data)
{
	if (!data->textures.north || !data->textures.south
		|| !data->textures.west || !data->textures.east)
	{
		printf("Error: Missing texture definitions\n");
		return (0);
	}
	if (data->floor_color.r == -1 || data->ceiling_color.r == -1)
	{
		printf("Error: Missing color definitions\n");
		return (0);
	}
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
	{
		printf("Error: Invalid or missing map\n");
		return (0);
	}
	if (data->player.x == -1 || data->player.y == -1)
	{
		printf("Error: Player position not found\n");
		return (0);
	}
	return (1);
}
