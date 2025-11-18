/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:50:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 20:31:35 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_texture_identifier(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	result = (ft_strncmp(trimmed, "NO ", 3) == 0
			|| ft_strncmp(trimmed, "SO ", 3) == 0
			|| ft_strncmp(trimmed, "WE ", 3) == 0
			|| ft_strncmp(trimmed, "EA ", 3) == 0);
	return (result);
}

int	parse_texture_line(const char *line, t_textures *textures)
{
	char	identifier[3];
	char	*path;
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (ft_strlen(trimmed) < 3)
		return (0);
	identifier[0] = trimmed[0];
	identifier[1] = trimmed[1];
	identifier[2] = '\0';
	if (ft_strcmp(identifier, "NO") != 0 && ft_strcmp(identifier, "SO") != 0
		&& ft_strcmp(identifier, "WE") != 0 && ft_strcmp(identifier, "EA") != 0)
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (0);
	}
	path = extract_texture_path(line, identifier);
	if (!path)
	{
		printf("Error: No texture path found for %s\n", identifier);
		return (0);
	}
	if (!validate_texture_path(path))
	{
		printf("Error: Invalid texture path: %s\n", path);
		free(path);
		return (0);
	}
	return (set_texture(textures, identifier, path));
}
