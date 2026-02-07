/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 00:50:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/23 19:45:19 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

int	is_texture_identifier(const char *line)
{
	char	*trimmed;
	int		result;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed)
		return (0);
	if (is_link_identifier(trimmed))
		return (0);
	if (ft_strlen(trimmed) < 3)
		return (0);
	result = ((ft_isalpha(trimmed[0]) || ft_isdigit(trimmed[0]))
			&& ft_isalpha(trimmed[1])
			&& trimmed[2] == ' ');
	return (result);
}

static int	is_required_texture_id(const char *identifier)
{
	return (ft_strcmp((char *)identifier, "NO") == 0
		|| ft_strcmp((char *)identifier, "SO") == 0
		|| ft_strcmp((char *)identifier, "WE") == 0
		|| ft_strcmp((char *)identifier, "EA") == 0);
}

static char	*get_validated_path(const char *line, const char *identifier)
{
	char	*path;

	if (!ft_isalpha(identifier[1])
		|| (!ft_isalpha(identifier[0]) && !ft_isdigit(identifier[0])))
	{
		printf("Error: Invalid texture identifier: %s\n", identifier);
		return (NULL);
	}
	path = extract_texture_path(line, identifier);
	if (!path)
	{
		printf("Error: No texture path found for %s\n", identifier);
		return (NULL);
	}
	if (!validate_texture_path(path))
	{
		printf("Error: Invalid texture path: %s\n", path);
		free(path);
		return (NULL);
	}
	return (path);
}

int	parse_texture_line(const char *line, t_textures *textures)
{
	char	identifier[3];
	char	*path;
	char	*trimmed;

	trimmed = trim_whitespace((char *)line);
	if (!trimmed || ft_strlen(trimmed) < 3)
		return (0);
	identifier[0] = trimmed[0];
	identifier[1] = trimmed[1];
	identifier[2] = '\0';
	if (!validate_texture_id(identifier))
		return (0);
	if (!is_required_texture_id(identifier))
	{
		printf("Error: Invalid texture identifier in mandatory: %s\n",
			identifier);
		return (0);
	}
	path = get_validated_path(line, identifier);
	if (!path)
		return (0);
	return (set_texture(textures, identifier, path));
}
